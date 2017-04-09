#include <node.h>
#include <iostream>
#include "graph.h"

namespace graph {

using v8::Context;
using v8::Persistent;
using v8::Local;
using v8::Isolate;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Value;
using v8::Object;
using v8::Array;
using v8::String;
using v8::Number;

using namespace std;

Persistent<Function> Graph::constructor;

Graph::Graph (int v, int edges[][2]) : V(v) {
  adj = new list<int>[V];
  for (int i = 0; i < V; i++) {
    AddEdge(edges[i][0], edges[i][1]) ;
  }
}

Graph::~Graph () {
  delete adj;
}

void Graph::Init (Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Graph"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "topoSort", TopoSort);
  // NODE_SET_PROTOTYPE_METHOD(tpl, "addEdge", _AddEdge);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Graph"),
    tpl->GetFunction());
}

void Graph::New (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Graph(...)`
    int v = args[0]->NumberValue();
    Local<Array> v8Arr = Local<Array>::Cast(args[1]);
    int l = v8Arr->Length();
    int arr[l][2];
    for (int i = 0; i < l; i++) {
      Local<Array> subV8Arr = Local<Array>::Cast(v8Arr->Get(i));
      arr[i][0] = subV8Arr->Get(0)->NumberValue(); 
      arr[i][1] = subV8Arr->Get(1)->NumberValue(); 
    }
    Graph* obj = new Graph(v, arr);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `Graph(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void Graph::TopoSortUtil (int v, bool visited[], stack<int> &Stack) {
  visited[v] = true;
  list<int>::iterator i;
  for (i = adj[v].begin(); i != adj[v].end(); ++i) {
    if (!visited[*i]) {
      TopoSortUtil(*i, visited, Stack);
    }
  }
  Stack.push(v);
}

void Graph::AddEdge (int v, int w) {
  adj[v].push_back(w); // Add w to v’s list
}

stack<int> Graph::TopoSort () {
  stack<int> Stack;
  bool *visited = new bool[V];
  for (int i = 0; i < V; i++)
    visited[i] = false;
  for (int i = 0; i < V; i++)
    if (visited[i] == false)
      TopoSortUtil(i, visited, Stack);
  return Stack;
}

// void Graph::_AddEdge (const FunctionCallbackInfo<Value>& args) {
//   Graph* obj = ObjectWrap::Unwrap<Graph>(args.Holder());
//
//   int v = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
//   int w = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();
//   obj->AddEdge(v, w);
// }

void Graph::TopoSort (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Graph* obj = ObjectWrap::Unwrap<Graph>(args.Holder());

  stack<int> Stack = obj->TopoSort();
  Local<Array> arr = Array::New(isolate);
  int i = 0;
  while (Stack.empty() == false) {
    arr->Set(i, Number::New(isolate, Stack.top()));
    i++;
    Stack.pop();
  }

  args.GetReturnValue().Set(arr);
}

void InitAll (Local<Object> exports) {
  Graph::Init(exports);
}

NODE_MODULE(addon, InitAll);  

} // namespace graph
