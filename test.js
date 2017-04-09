var test = require("ava");
var graph = require('./index');

test("toposort dfs", function(t) {
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
  t.deepEqual(inst.topoSort(), [
    0, 1, 4, 7, 2, 3, 8, 5, 6, 9
  ]);
})
