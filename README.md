# cxx-graph

Topological Sort C++ addon

## usage

Topological Sort using Depth First Search(DFS)

```
var graph = require('./index');
var e = [
    [0,1],
    [1,2],
    [1,4],
    [2,3],
    [2,5],
    [3,5],
    [3,8],
    [4,7],
    [5,6],
    [6,9],
    [7,9],
    [8,9]
  ];
var inst = new graph.Graph(10, e);
console.log(inst.topoSort()) // [0, 1, 4, 7, 2, 3, 8, 5, 6, 9]
```


## TODO

* Topological Sort using  Breadth First Search（BFS）

