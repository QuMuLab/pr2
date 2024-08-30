
/*
    http://marvl.infotech.monash.edu/webcola/
    * Continue from this example: http://marvl.infotech.monash.edu/webcola/examples/unix.html
*/

/************
 * Settings *
 ************/
 var COLLAPSE_PSGRAPH_CHAINS = false;
 var COLLAPSE_FSGRAPH_CHAINS = false;

 /******************************************************/

function toggle_collapse_fsgraph() {
    // Disable the collapse settings until the bug that causes bad caching is fixed
    if (!currentSnapshot) {
        COLLAPSE_FSGRAPH_CHAINS = !COLLAPSE_FSGRAPH_CHAINS;
        if (currentSnapshot)
            drawSnapshot(currentSnapshot, currentRound);
    }
}

function toggle_collapse_psgraph() {
    // Disable the collapse settings until the bug that causes bad caching is fixed
    if (!currentSnapshot) {
        COLLAPSE_PSGRAPH_CHAINS = !COLLAPSE_PSGRAPH_CHAINS;
        if (currentSnapshot)
            drawSnapshot(currentSnapshot, currentRound);
    }
}

function init() {

    $('#snapshot-list').html('<div id="accordion" role="tablist" aria-multiselectable="true"></div>');

    var rounds = {};
    for (s in snapshots)
        rounds[snapshots[s].solution.round] = true;

    for (r in rounds) {
        var html = '';
        html += '<div class="card">\n';
        html += '  <div class="card-header" role="tab" id="heading'+r+'">\n';
        html += '    <h5 class="mb-0">\n';
        html += '      <a data-toggle="collapse" data-parent="#accordion" href="#collapse'+r+'" aria-expanded="true" aria-controls="collapse'+r+'">\n';
        html += '        Round #'+r+'\n';
        html += '      </a>\n';
        html += '    </h5>\n';
        html += '  </div>\n';
        html += '  <div id="collapse'+r+'" class="collapse show" role="tabpanel" aria-labelledby="heading'+r+'">\n';
        html += '    <div class="card-block">\n';
        html += '      <div id="snapshotList'+r+'" class="list-group">\n';
        html += '      </div>\n';
        html += '    </div>\n';
        html += '  </div>\n';
        html += '</div>\n';
        $('#accordion').append(html);
    }
    $('.collapse').collapse('hide');

    for (s in snapshots) {
        if (snapshots[s].solution.psgraph.init) {
            var round = snapshots[s].solution.round;
            $('#snapshotList'+round).append(
                '<a href="#" class="list-group-item list-group-item-action" ' +
                'id="snapshotButton'+round+'-'+s+'" ' +
                'snapshot="'+s+'" round="'+round+'" ' +
                'onclick="drawSnapshot('+s+', '+round+')" title="' + snapshots[s].solution.type +
                '">#' + s + ' (' + snapshots[s].solution.score + ')</a>');
        } else
            $('#snapshotList'+snapshots[s].solution.round).append(
                '<a href="#" class="list-group-item list-group-item-action disabled">' +
                '#' + s + ' (-)</a>');
    }

    document.addEventListener("keydown", function(event) {
        // Make the keys j/k go to the next/prev snapshot
        var curSelection = $('#snapshotButton'+snapshots[currentSnapshot].solution.round+'-'+currentSnapshot);
        if (74 == event.which)
            curSelection.next().click();
        else if (75 == event.which)
            curSelection.prev().click();
    });

    initializeGraphs();
    alert('Warning:\n\n You must pick the collapse settings prior to viewing graphs.\n\nA bug exists that caches nodes incorrectly if you change the settings dynamically.');
};

var currentSnapshot, currentRound;
var psnodes, pslinks, psstates, psnode2id, newpsnodes, newpspaths, pssucc, pspred;
var fsnodes, fslinks, fsstates, fsnode2id, newfsnodes, newfspaths, fssucc, fspred;

var pssvg, psd3cola, pswidth, psheight, psnode, pspath, pslabel, psnodeRadius, pscontainer;
var fssvg, fsd3cola, fswidth, fsheight, fsnode, fspath, fslabel, fsnodeRadius, fscontainer;

function drawSnapshot(s, round) {

    // Disable the collapse settings until the bug that causes bad caching is fixed
    $('#collapse-settings .btn').addClass('disabled');

    $('#snapshot-info').html(snapshots[s].solution.type);

    currentSnapshot = s;
    currentRound = round;

    $('#snapshotList'+round + ' a').removeClass('active');
    $('#snapshotButton'+round+'-'+s).addClass('active');

    drawFullGraph(s);
    drawPSGraph(s);
};

function confirmPS() {
    var lookup = {};
    for (var i=0; i<psnodes.length; i++)
        lookup[psnodes[i]] = true;

    for (var i=0; i < pslinks.length; i++) {

        var u = pslinks[i].source;
        var v = pslinks[i].target;

        if (!(u in lookup))
            alert("Missing edge endpoint: " + u.id);
        if (!(v in lookup))
            alert("Missing edge endpoint: " + v.id);
    }
};


function initializeGraphs() {

    /* Partial State Graph */
    pswidth = $('#psgraph').width();
    psheight = $('#psgraph').height();
    psnodeRadius = 8;
    var margin = 10, pad = 12;

    psd3cola = cola.d3adaptor(d3)
                   .avoidOverlaps(true)
                   .convergenceThreshold(1e-3)
                   .size([pswidth, psheight])
                   .flowLayout("y", 20)
                   .jaccardLinkLengths(84);

    pssvg = d3.select("#psgraph");
    pscontainer = pssvg.append('g');

    pssvg.call(d3.zoom().on("zoom", function() {
        pscontainer.attr("transform", d3.event.transform);
    }));

    // define arrow markers for graph links
    pssvg.append('svg:defs').append('svg:marker')
         .attr('id', 'end-arrow')
         .attr('viewBox', '0 -5 10 10')
         .attr('refX', 6)
         .attr('markerWidth', 3)
         .attr('markerHeight', 3)
         .attr('orient', 'auto')
      .append('svg:path')
         .attr('d', 'M0,-5L10,0L0,5')
         .attr('fill', '#000');

    var lineFunction = d3.line()
                         .x(function (d) { return d.x; })
                         .y(function (d) { return d.y; });

    var routeEdges = function () {
        psd3cola.prepareEdgeRouting();
        pspath.attr("d", function (d) {return lineFunction(psd3cola.routeEdge(d));});
    }


    psd3cola.on("tick", function () {
        psnode.each(function (d) { d.innerBounds = d.bounds.inflate(-margin); })
              .attr("cx", function (d) { return d.innerBounds.x; })
              .attr("cy", function (d) { return d.innerBounds.y; })
              .attr("width", function (d) {
                  return d.innerBounds.width();
              })
              .attr("height", function (d) { return d.innerBounds.height(); });

        pspath.attr("d", function (d) {
            var route = cola.makeEdgeBetween(d.source.innerBounds, d.target.innerBounds, 5);
            return lineFunction([route.sourceIntersection, route.arrowStart]);
        });

        pslabel.attr("x", function (d) { return d.x })
               .attr("y", function (d) { return d.y + (margin + pad) / 2 });
    }).on("end", routeEdges);



    /*  Full State Graph */
    fswidth = $('#fsgraph').width();
    fsheight = $('#fsgraph').height();

    fsnodeRadius = 8;

    fsd3cola = cola.d3adaptor(d3)
                   .avoidOverlaps(true)
                   .size([fswidth, fsheight])
                   .flowLayout("y", 42)
                   .symmetricDiffLinkLengths(8);

    fssvg = d3.select("#fsgraph");
    fscontainer = fssvg.append('g');

    fssvg.call(d3.zoom().on("zoom", function() {
        fscontainer.attr("transform", d3.event.transform);
    }));

    // define arrow markers for graph links
    fssvg.append('svg:defs').append('svg:marker')
         .attr('id', 'end-arrow')
         .attr('viewBox', '0 -5 10 10')
         .attr('refX', 6)
         .attr('markerWidth', 3)
         .attr('markerHeight', 3)
         .attr('orient', 'auto')
      .append('svg:path')
         .attr('d', 'M0,-5L10,0L0,5')
         .attr('fill', '#000');

    fsd3cola.on("tick", function () {
        // draw directed edges with proper padding from node centers
        fspath.attr('d', function (d) {
            var deltaX = d.target.x - d.source.x,
                deltaY = d.target.y - d.source.y,
                dist = Math.sqrt(deltaX * deltaX + deltaY * deltaY),
                normX = deltaX / dist,
                normY = deltaY / dist,
                sourcePadding = fsnodeRadius,
                targetPadding = fsnodeRadius + 2,
                sourceX = d.source.x + (sourcePadding * normX),
                sourceY = d.source.y + (sourcePadding * normY),
                targetX = d.target.x - (targetPadding * normX),
                targetY = d.target.y - (targetPadding * normY);
            return 'M' + sourceX + ',' + sourceY + 'L' + targetX + ',' + targetY;
        });

        fsnode.attr("cx", function (d) { return d.x; })
              .attr("cy", function (d) { return d.y; });

        fslabel.attr("x", function (d) { return d.x; })
               .attr("y", function (d) { return d.y; });
    });
};



function drawFullGraph(s) {

    fsnodes = []; // The actual nodes of the graph
    fslinks = []; // The actual links of the graph
    fssucc = {}; // Mapping from node to successor nodes
    fspred = {}; // Mapping from node to predecessor nodes
    fsstates = {}; // Mapping from the full state node to the state
    fsnode2id = {}; // Mapping from a node to its id
    newfsnodes = {}; // The nodes that were newly added in this snapshot
    newfspaths = {}; // The links that were newly added in this snapshot
    var fs2ps = {}; // Mapping from the full state graph to the partial state graph

    if ((s-1 in snapshots) && (snapshots[s].solution.round == snapshots[s-1].solution.round))
        for (n in snapshots[s].solution.prpsearchnodes)
            if (!(n in snapshots[s-1].solution.prpsearchnodes))
                newfsnodes[n] = true;

    for (var psid in snapshots[s].solution.ps2fs)
        for (var fsid in snapshots[s].solution.ps2fs[psid])
            fs2ps[fsid] = psid;

    var i = 0;
    for (var nid in snapshots[s].solution.prpsearchnodes) {
        n = snapshots[s].solution.prpsearchnodes[nid]; // not doing anything with this for now
        fsnodes.push({origname: n.name, name: n.name, id: i, weight: 1, nid: nid, psid: fs2ps[nid],
                      open:n.open, subsumed:n.subsumed, poisoned:n.poisoned});
        fsnode2id[nid] = i;
        fssucc[i] = [];
        fspred[i] = [];
        i++;
    }

    for (i = 0; i < snapshots[s].solution.prpsearchnodelinks.length; ++i) {

        e = snapshots[s].solution.prpsearchnodelinks[i];
        var u = fsnode2id[e[0]];
        var v = fsnode2id[e[1]];

        fslinks.push({source:u, target:v, id: i});

        fssucc[u].push(v);
        fspred[v].push(u);
    }

    if ((s-1 in snapshots) && (snapshots[s].solution.round == snapshots[s-1].solution.round)) {
        var oldpaths = {};
        for (i = 0; i < snapshots[s-1].solution.prpsearchnodelinks.length; ++i) {
            e = snapshots[s-1].solution.prpsearchnodelinks[i];
            oldpaths[fsnode2id[e[0]] + "-" + fsnode2id[e[1]]] = true;
        }
        for (i = 0; i < snapshots[s].solution.prpsearchnodelinks.length; ++i) {
            e = snapshots[s].solution.prpsearchnodelinks[i];
            if (!(fsnode2id[e[0]] + "-" + fsnode2id[e[1]] in oldpaths))
                newfspaths[fsnode2id[e[0]] + "-" + fsnode2id[e[1]]] = true;
        }
    }

    // If we want to collapse 1-in/1-out chains, then we rewrite the fsnodes and fslinks
    if (COLLAPSE_FSGRAPH_CHAINS) {
        function gen_path(id) {
            // Base case is when this node isn't a 1-in / 1-out
            if ((1 != fssucc[id].length) || ((1 != fspred[id].length))) {
                // Only use this label if it is the initial search node
                if (0 == fspred[id].length)
                    return [id, '\n'+fsnodes[id].origname];
                else
                    return [id, ''];
            }

            // Otherwise, we recurse and build the new name
            var res = gen_path(fspred[id][0]);
            return [res[0], res[1]+"\n"+fsnodes[id].origname];
        }

        var collapsed_fsnodes = [];
        var collapsed_fslinks = [];
        fsnode2id = {};

        for (var fsnid = 0; fsnid < fsnodes.length; fsnid++) {
            var fsn = fsnodes[fsnid];
            if (fssucc[fsnid].length != 1) {
                // Special case for the init
                if (0 == fspred[fsnid].length) {
                    fsn.name = fsn.origname;
                } else {
                    var res = gen_path(fspred[fsnid][0]);
                    fsn.name = res[1] + "\n" + fsn.origname;
                    // Only add the link if the source isn't a single-successor init
                    if (fssucc[fsnodes[res[0]].nid].length > 1)
                        collapsed_fslinks.push({source:fsnodes[res[0]].nid, target:fsn.nid, id: collapsed_fslinks.length});
                }
                fsnode2id[fsn.nid] = collapsed_fsnodes.length;
                collapsed_fsnodes.push(fsn);
            }
        }

        fsnodes = collapsed_fsnodes;
        fslinks = collapsed_fslinks;
        newpaths = {};

        // Convert the nid's to new id's
        for (var i=0; i<fslinks.length; i++) {
            fslinks[i].source = fsnode2id[fslinks[i].source];
            fslinks[i].target = fsnode2id[fslinks[i].target];
        }

    }

    fsstates = false;

    fsnodes.forEach(function (v) { v.height = v.width = 2 * fsnodeRadius; });

    fspath = fscontainer.selectAll(".fslink").data(fslinks, function(d) {return d.source + "-" + d.target;});
    fspath.exit().remove();
    fspath = fspath.enter().append('svg:path')
        .attr('class', 'fslink')
        .merge(fspath);

    fspath.style("stroke", function (d) {
        if (d.source + "-" + d.target in newfspaths)
            return "#ff2b2b";
        else
            return "#000000";
    }).style("stroke-width", function (d) {
        if (d.source + "-" + d.target in newfspaths)
            return "5px";
        else
            return "2.6px";
    }).style("opacity", function (d) {
        if (d.source + "-" + d.target in newfspaths)
            return "1.0";
        else
            return "0.8";
    });

    fsnode = fscontainer.selectAll(".fsnode").data(fsnodes, function(d) {return d.nid;});
    fsnode.exit().remove();
    fsnode = fsnode.enter().append("circle")
        .attr("id", function(n) {return "fsnode"+n.nid;})
        .attr("r", fsnodeRadius)
        .call(fsd3cola.drag)
        .merge(fsnode);

    fsnode.style("stroke", function (n) {
        if (n.nid in newfsnodes)
            return "#E648A3"; // Hot pink for the new nodes
        else
            return "#000000"; // Just black for the old nodes
    }).attr("class", function(n) {
        return "fsnode pschild"+ n.psid;
    }).style("fill", function (n) {
        if (n.subsumed)
            return "#00f"; // Blue if the node is subsumed
        else if (n.poisoned)
            return "#9f24f2"; // Sickly purple if the node is poisoned
        else if (n.open)
            return "#fff"; // White if the node is open
        else
            return "#828282"; // Gray as a default
    });

    fsnode.on("mouseover", function(d) {
              $('#state').html('<pre></pre>');
              $('#state').html('<h4>Complete State for SearchNode ('+d.nid+')</h4><pre>\nComing soon...</pre>');
              if (COLLAPSE_FSGRAPH_CHAINS)
                  $('#collapsed_path').html('\n<h4>Chain of nodes:</h4>\n<pre>'+d.name+'</pre>\n');
              else
                  $("#fslabel-"+d.nid).show();
              if (-1 != d.psid)
                  d3.select('#psnode'+d.psid)
                    .transition()
                    .duration(750)
                    .attr("r", 2*psnodeRadius);
          })
          .on("mouseout", function(d) {
              $('.fslabel').hide();
              if (-1 != d.psid)
                  d3.select('#psnode'+d.psid)
                    .transition()
                    .duration(750)
                    .attr("r", psnodeRadius);
          })
          .on("dblclick", function(d) {
              console.log("Node Info:");
              console.log(d);
          });


    fslabel = fscontainer.selectAll(".fslabel").data(fsnodes, function(d) {return d.nid;});
    fslabel.exit().remove();
    fslabel = fslabel.enter().append("text")
        .attr("class", "fslabel")
        .attr("dx", 16)
        .attr("dy", ".35em")
        .attr("id", function(d) { return "fslabel-"+d.nid; })
        .text(function(d) { return d.name; })
        .call(fsd3cola.drag)
        .merge(fslabel);

    fsd3cola
        .nodes(fsnode.data())
        .links(fspath.data())
        .start(10,20,20);
};

function drawPSGraph(s) {

    psnodes = []; // The actual nodes of the graph
    pslinks = []; // The actual links of the graph
    pssucc = {}; // Mapping from node to successor nodes
    pspred = {}; // Mapping from node to predecessor nodes
    psstates = {}; // Mapping from the partial state object to the partial state text
    newpsnodes = {}; // The nodes that were newly added in this snapshot
    newpspaths = {}; // The links that were newly added in this snapshot
    psnode2id = {}; // Mapping from the node to its id

    if ((s-1 in snapshots) && (snapshots[s].solution.round == snapshots[s-1].solution.round))
        for (n in snapshots[s].solution.psgraph.nodes)
            if (!snapshots[s-1].solution.psgraph.init)
                newpsnodes[n] = true;
            else if (!(n in snapshots[s-1].solution.psgraph.nodes))
                newpsnodes[n] = true;

    var ncount = 0;
    for (var nid in snapshots[s].solution.psgraph.nodes) {
        n = snapshots[s].solution.psgraph.nodes[nid];
        nname = n.action + ' ('+n.distance+')';
        init = false;
        if (n.distance == 0)
            nname = 'Goal';
        if (nid == snapshots[s].solution.psgraph.init)
            init = true;
        psnodes.push({id: ncount, origname: nname, name: nname, data: n, init: init, weight: n.distance, nid: nid, sink:false});
        psnode2id[''+nid] = ncount;
        pssucc[ncount] = [];
        pspred[ncount] = [];
        ncount++;
    }

    for (var i = 0; i < snapshots[s].solution.psgraph.edges.length; ++i) {
        e = snapshots[s].solution.psgraph.edges[i];

        if (">" === e[1]) {

            var u, v;

            if (-1 == e[2]) {
                var nid = "undefined"+ncount;
                psnodes.push({id:ncount, origname: "undefined", name: "undefined", weight: 0, nid: nid, sink:true});
                psnode2id[nid] = ncount;
                pssucc[ncount] = [];
                pspred[ncount] = [];
                u = psnode2id[e[0]];
                v = ncount;
                ncount++;
            } else {
                u = psnode2id[e[0]];
                v = psnode2id[e[2]];
            }

            pslinks.push({source:u, target:v, id: u+'-'+v});

            pssucc[u].push(v);
            pspred[v].push(u);

        }
    }

    if ((s-1 in snapshots) && (snapshots[s].solution.round == snapshots[s-1].solution.round)) {
        var oldpaths = {};
        if ('edges' in snapshots[s-1].solution.psgraph) {
            for (i = 0; i < snapshots[s-1].solution.psgraph.edges.length; ++i) {
                e = snapshots[s-1].solution.psgraph.edges[i];
                oldpaths[psnode2id[e[0]] + "-" + psnode2id[e[2]]] = true;
            }
        }
        for (var i = 0; i < snapshots[s].solution.psgraph.edges.length; ++i) {
            e = snapshots[s].solution.psgraph.edges[i];
            if (!(psnode2id[e[0]] + "-" + psnode2id[e[2]] in oldpaths))
                newpspaths[psnode2id[e[0]] + "-" + psnode2id[e[2]]] = true;
        }
    }

    // If we want to collapse 1-in/1-out chains, then we rewrite the psnodes and pslinks
    if (COLLAPSE_PSGRAPH_CHAINS) {
        function gen_path(id) {
            // Base case is when this node isn't a 1-in / 1-out
            if ((1 != pssucc[id].length) || ((1 != pspred[id].length))) {
                return [id, ''];
                // Only use this label if it is the initial search node
                if (0 == pspred[id].length)
                    return [id, '\n'+psnodes[id].origname];
                else
                    return [id, ''];
            }

            // Otherwise, we recurse and build the new name
            var res = gen_path(pspred[id][0]);
            return [res[0], res[1]+"\n"+psnodes[id].origname];
        }

        var collapsed_psnodes = [];
        var collapsed_pslinks = [];
        psnode2id = {};

        for (var psnid = 0; psnid < psnodes.length; psnid++) {
            var psn = psnodes[psnid];

            // We will keep a 1-in / 1-out node if it has a successor that branches (in or out)
            if ((pssucc[psnid].length == 1) && (pspred[psnid].length == 1)) {
                var sid = pssucc[psnid][0];
                if ((pssucc[sid].length != 1) || (pspred[sid].length != 1)) {
                    psnode2id[psn.nid] = collapsed_psnodes.length;
                    collapsed_psnodes.push(psn);

                    var res = gen_path(pspred[psnid][0]);
                    psn.name = res[1] + "\n" + psn.origname;
                    collapsed_pslinks.push({source:psnodes[res[0]].nid, target:psn.nid, id: collapsed_pslinks.length});
                }
            // We will keep every node without a 1-in / 1-out property
            } else {

                psnode2id[psn.nid] = collapsed_psnodes.length;
                collapsed_psnodes.push(psn);

                for (var i=0; i < pspred[psnid].length; i++)
                    collapsed_pslinks.push({source:psnodes[pspred[psnid][i]].nid,
                                            target:psn.nid,
                                            id: collapsed_pslinks.length});
            }
        }

        psnodes = collapsed_psnodes;
        pslinks = collapsed_pslinks;
        newpspaths = {};

        // Convert the nid's to new id's
        for (var i=0; i<pslinks.length; i++) {
            pslinks[i].source = psnode2id[pslinks[i].source];
            pslinks[i].target = psnode2id[pslinks[i].target];
        }

    }

    psstates = snapshots[s].solution.psgraph.states;

    psnodes.forEach(function (v) { v.height = v.width = 2 * psnodeRadius; });

    pspath = pscontainer.selectAll(".pslink").data(pslinks, function(d) {return d.lid;});
    pspath.exit().remove();
    pspath = pspath.enter().append('svg:path')
        .attr('class', 'pslink')
        .merge(pspath);

    pspath.style("stroke", function (d) {
        if (d.source + "-" + d.target in newpspaths)
            return "#ff2b2b";
        else
            return "#000000";
    }).style("stroke-width", function (d) {
        if (d.source + "-" + d.target in newpspaths)
            return "5px";
        else
            return "2.6px";
    }).style("opacity", function (d) {
        if (d.source + "-" + d.target in newpspaths)
            return "1.0";
        else
            return "0.8";
    });

    psnode = pscontainer.selectAll(".psnode").data(psnodes, function(d) {return d.nid;});
    psnode.exit().remove();
    psnode = psnode.enter().append("circle")
        .attr("class", "psnode")
        .attr("id", function(n) {return "psnode"+n.nid;})
        .attr("r", function(n) {
            if (n.sink)
                return 0;
            else
                return psnodeRadius;
        })
        .call(psd3cola.drag)
        .merge(psnode);

    psnode.style("stroke", function (n) {
        if (n.nid in newpsnodes)
            return "#E648A3"; // Hot pink for the new nodes
        else
            return "#000000"; // Just black for the old nodes
    }).style("fill", function (n) {
        if (n.data && (1 == n.data.is_goal))
            return "#e8c100"; // Golden for goals
        else if (n.init)
            return "#067c00"; // Green for the init
        else if (n.data && (1 == n.data.is_sc))
            return "#025ef2"; // Blue for sc
        else if (n.name === "undefined")
            return "#930000"; // Red for undefined
        else
            return "#828282"; // Gray as a default
    });

    psnode.on("mouseover", function(d) {
              $('#state').html('<pre></pre>');
              if (d.data && psstates)
                  $('#state').html('<h4>Partial State for SolutionStep ('+d.nid+")</h4><pre>\n"+
                                    psstates[d.data.state].join("\n")+'</pre>');
              if (COLLAPSE_PSGRAPH_CHAINS) {
                  if (d.name.trim() == d.origname)
                      $('#collapsed_path').html('');
                  else
                      $('#collapsed_path').html('\n<h4>Chain of nodes:</h4>\n<pre>'+d.name+'</pre>\n');
              }
              d3.selectAll('.pschild'+d.nid)
                .transition()
                .duration(750)
                .attr("r", 2*fsnodeRadius);
          })
          .on("mouseout", function(d) {
              d3.selectAll('.pschild'+d.nid)
                .transition()
                .duration(750)
                .attr("r", fsnodeRadius);
          });

    psnode.on("dblclick", function(d) {
        console.log("Node Info:");
        console.log(d);
    });

    pslabel = pscontainer.selectAll(".pslabel").data(psnodes, function(d) {return d.nid;});
    pslabel.exit().remove();
    pslabel = pslabel.enter().append("text")
        .attr("class", "pslabel")
        .attr("dx", 16)
        .attr("dy", ".35em")
        .attr("id", function(d) { return "pslabel-"+d.nid; })
        .text(function(d) {
            if (d.sink)
                return '';
            else {
                if (!COLLAPSE_PSGRAPH_CHAINS || (d.name.trim() === d.origname))
                    return d.name;
                else
                    return "[...]";
            }
        })
        .call(psd3cola.drag)
        .merge(pslabel);

    psd3cola
        .nodes(psnode.data())
        .links(pspath.data())
        .start(10,20,20);

};
