var snapshots = {
0: {
"solution": {
  "type": "Initial Round",
  "score": 0,
  "size": 1,
  "round": 1,
"psgraph": {

    "init": false,
    "goal": "0"
  },
  "policy": "Coming soon...",
  "ps2fs": {
  },
  "prpsearchnodes": {
  0: {
      name: "(0)",
      open: 1,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
  ],
},
},
1: {
"solution": {
  "type": "(case-5) New Path [node 0]",
  "score": 0.243,
  "size": 5,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "-1"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "-1"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "-1"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, },
    2: {2: 2, },
    4: {0: 0, },
    3: {1: 1, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
  ],
},
},
2: {
"solution": {
  "type": "(case-4) Hooking Up [node 11]",
  "score": 0.243,
  "size": 5,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "-1"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, },
    2: {2: 2, },
    0: {11: 11, },
    4: {0: 0, },
    3: {1: 1, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
  ],
},
},
3: {
"solution": {
  "type": "(case-3) Predefined Path [node 10]",
  "score": 0.243,
  "size": 5,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "-1"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, },
    2: {2: 2, },
    0: {10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
  ],
},
},
4: {
"solution": {
  "type": "(case-5) New Path [node 9]",
  "score": 0.509,
  "size": 6,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "-1"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["5", ">", "0"],
        ["5", ">", "-1"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, },
    2: {2: 2, },
    0: {10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {9: 9, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
  ],
},
},
5: {
"solution": {
  "type": "(case-4) Hooking Up [node 13]",
  "score": 0.509,
  "size": 6,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["5", ">", "0"],
        ["5", ">", "0"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, },
    2: {2: 2, },
    0: {13: 13, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {9: 9, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
  ],
},
},
6: {
"solution": {
  "type": "(case-3) Predefined Path [node 12]",
  "score": 0.509,
  "size": 6,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "-1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["5", ">", "0"],
        ["5", ">", "0"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {9: 9, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
  ],
},
},
7: {
"solution": {
  "type": "(case-4) Hooking Up [node 8]",
  "score": 0.509,
  "size": 6,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "-1"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["5", ">", "0"],
        ["5", ">", "0"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {9: 9, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
  ],
},
},
8: {
"solution": {
  "type": "(case-4) Hooking Up [node 7]",
  "score": 0.509,
  "size": 6,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["5", ">", "0"],
        ["5", ">", "0"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {7: 7, 9: 9, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
  ],
},
},
9: {
"solution": {
  "type": "(case-2) Matched complete state\n -- No modification [node 6]",
  "score": 0.509,
  "size": 6,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "-1"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["5", ">", "0"],
        ["5", ">", "0"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {7: 7, 9: 9, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
  ],
},
},
10: {
"solution": {
  "type": "(case-5) New Path [node 5]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "-1"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "-1"],
        ["6", ">", "-1"],
        ["6", ">", "-1"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {7: 7, 9: 9, },
    6: {5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
11: {
"solution": {
  "type": "(case-4) Hooking Up [node 17]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "-1"],
        ["6", ">", "-1"],
        ["6", ">", "5"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {7: 7, 9: 9, 17: 17, },
    6: {5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
12: {
"solution": {
  "type": "(case-4) Hooking Up [node 16]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "-1"],
        ["6", ">", "1"],
        ["6", ">", "5"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, 16: 16, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {7: 7, 9: 9, 17: 17, },
    6: {5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
13: {
"solution": {
  "type": "(case-4) Hooking Up [node 15]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "5"],
        ["6", ">", "1"],
        ["6", ">", "5"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, 16: 16, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {15: 15, 7: 7, 9: 9, 17: 17, },
    6: {5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
14: {
"solution": {
  "type": "(case-3) Predefined Path [node 14]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "-1"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "5"],
        ["6", ">", "1"],
        ["6", ">", "5"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, 14: 14, 16: 16, },
    2: {2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {15: 15, 7: 7, 9: 9, 17: 17, },
    6: {5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
15: {
"solution": {
  "type": "(case-4) Hooking Up [node 4]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 0,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "-1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "-1"],
        ["3", ">", "2"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "5"],
        ["6", ">", "1"],
        ["6", ">", "5"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, 14: 14, 16: 16, },
    2: {4: 4, 2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {15: 15, 7: 7, 9: 9, 17: 17, },
    6: {5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
16: {
"solution": {
  "type": "(case-4) Hooking Up [node 3]",
  "score": 1,
  "size": 7,
  "round": 1,
"psgraph": {

    "init": "4",
    "goal": "0",
    "nodes" : {
      "1": {
        "expected_successor": "0",
        "action": "move-forward-last-door-open l3 l4 d4",
        "state": "0x1ccf170",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-open_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "2": {
        "expected_successor": "1",
        "action": "move-forward-door-open l2 l3 d3 d4",
        "state": "0x1cceaa0",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      },
      "0": {
        "expected_successor": false,
        "action": "---",
        "state": "0x1cb01a0",
        "distance": 0,
        "is_relevant": 1,
        "is_goal": 1,
        "is_sc": 1,
        "successors": [

         ]
      },
      "4": {
        "expected_successor": "3",
        "action": "pick-key l1",
        "state": "0x1cce140",
        "distance": 4,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "pick-key l1",
              "successor_id": "3"
          }
         ]
      },
      "3": {
        "expected_successor": "2",
        "action": "move-forward-door-open l1 l2 d2 d3",
        "state": "0x1cce490",
        "distance": 3,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-open_DETDUP_1 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_2 l1 l2 d2 d3",
              "successor_id": "6"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_3 l1 l2 d2 d3",
              "successor_id": "2"
          },
          {
              "outcome_label": "move-forward-door-open_DETDUP_4 l1 l2 d2 d3",
              "successor_id": "6"
          }
         ]
      },
      "5": {
        "expected_successor": "0",
        "action": "move-forward-last-door-closed l3 l4 d4",
        "state": "0x1cd04a0",
        "distance": 1,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_1 l3 l4 d4",
              "successor_id": "0"
          },
          {
              "outcome_label": "move-forward-last-door-closed_DETDUP_2 l3 l4 d4",
              "successor_id": "0"
          }
         ]
      },
      "6": {
        "expected_successor": "1",
        "action": "move-forward-door-closed l2 l3 d3 d4",
        "state": "0x1ce3c80",
        "distance": 2,
        "is_relevant": 0,
        "is_goal": 0,
        "is_sc": 1,
        "successors": [
          {
              "outcome_label": "move-forward-door-closed_DETDUP_1 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_2 l2 l3 d3 d4",
              "successor_id": "5"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_3 l2 l3 d3 d4",
              "successor_id": "1"
          },
          {
              "outcome_label": "move-forward-door-closed_DETDUP_4 l2 l3 d3 d4",
              "successor_id": "5"
          }
         ]
      }
    },
    "edges" : [
        ["1", ">", "0"],
        ["1", ">", "0"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["2", ">", "1"],
        ["2", ">", "5"],
        ["4", ">", "3"],
        ["3", ">", "2"],
        ["3", ">", "6"],
        ["3", ">", "2"],
        ["3", ">", "6"],
        ["5", ">", "0"],
        ["5", ">", "0"],
        ["6", ">", "1"],
        ["6", ">", "5"],
        ["6", ">", "1"],
        ["6", ">", "5"]
    ],
    "states" : {
      "0x1ccf170": [
        "Atom open(d4)",
        "Atom player-at(l3)"
      ],
      "0x1cceaa0": [
        "Atom hold-key()",
        "Atom open(d3)",
        "Atom player-at(l2)"
      ],
      "0x1cb01a0": [
        "Atom player-at(l4)"
      ],
      "0x1cce140": [
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cce490": [
        "Atom hold-key()",
        "Atom open(d2)",
        "Atom player-at(l1)"
      ],
      "0x1cd04a0": [
        "Atom hold-key()",
        "Atom closed(d4)",
        "Atom player-at(l3)"
      ],
      "0x1ce3c80": [
        "Atom hold-key()",
        "Atom closed(d3)",
        "Atom player-at(l2)"
      ]
     }
  },
  "policy": "Coming soon...",
  "ps2fs": {
    1: {6: 6, 8: 8, 14: 14, 16: 16, },
    2: {4: 4, 2: 2, },
    0: {13: 13, 12: 12, 10: 10, 11: 11, },
    4: {0: 0, },
    3: {1: 1, },
    5: {15: 15, 7: 7, 9: 9, 17: 17, },
    6: {3: 3, 5: 5, },
  },
  "prpsearchnodes": {
  0: {
      name: "(0) pick-key l1",
      open: 0,
      init: 1,
      poisoned: 0,
      subsumed: 0,
  },
  1: {
      name: "(1) move-forward-door-open l1 l2 d2 d3",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  2: {
      name: "(2) move-forward-door-open l2 l3 d3 d4",
      open: 1,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  3: {
      name: "(3)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  4: {
      name: "(4)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  5: {
      name: "(5) move-forward-door-closed l2 l3 d3 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  6: {
      name: "(6) move-forward-last-door-open l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  7: {
      name: "(7)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  8: {
      name: "(8)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  9: {
      name: "(9) move-forward-last-door-closed l3 l4 d4",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  10: {
      name: "(10)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  11: {
      name: "(11)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  12: {
      name: "(12)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  13: {
      name: "(13)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  14: {
      name: "(14)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  15: {
      name: "(15)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  16: {
      name: "(16)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  17: {
      name: "(17)",
      open: 0,
      init: 0,
      poisoned: 0,
      subsumed: 0,
  },
  },
  "prpsearchnodelinks": [
    [0,1],
    [1,2],
    [1,3],
    [1,4],
    [1,5],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [6,10],
    [6,11],
    [9,12],
    [9,13],
    [5,14],
    [5,15],
    [5,16],
    [5,17],
  ],
},
},
};