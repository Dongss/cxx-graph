/* 
 * topological sorting of a DAG
 * */

#ifndef TOPSORT_H
#define TOPSORT_H

#include <node.h>
#include <node_object_wrap.h>
#include <list>
#include <stack>

namespace graph {

using namespace std;

class Graph : public node::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);
  void TopoSortUtil(int v, bool visited[], stack<int> &Stack);
  void AddEdge(int v, int w);
  stack<int> TopoSort();

private:
  explicit Graph(int v, int edges[][2]);
  ~Graph();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void TopoSort(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
  int V; 
  list<int> *adj; 
};

} // namespace graph

# endif
