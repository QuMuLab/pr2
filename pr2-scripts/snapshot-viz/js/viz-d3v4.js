
function init() {
    $('#snapshotList').html('');
    for (s in snapshots) {
        if (snapshots[s].solution.psgraph.init)
            $('#snapshotList').append('<li><a href="#" onclick="drawSnapshot(' +
             s + ')">Snapshot ' + s + ' (' + snapshots[s].solution.score +
             ')</a></li>');
        else
            $('#snapshotList').append('<li>Snapshot ' + s + ' (-)</li>');
    }

    $(function() {
        $('#toggle-graph').change(function() {
            if ($(this).prop('checked'))
                drawSnapshot = drawPSGraph;
            else
                drawSnapshot = drawFullGraph;
            if (currentSnapshot)
                drawSnapshot(currentSnapshot);
        });
    });

    drawSnapshot = drawPSGraph;
    initializeGraph();
};

var nodes, links, states, node2id, graph_type, drawSnapshot, currentSnapshot;

var svg, force, node, link, width, height, linkedByIndex, container, drag;









        /*
            Work on integrating this for the full graph:
            - http://bl.ocks.org/d3noob/8375092
        */








function drawFullGraph(s) {
    currentSnapshot = s;

    nodes = [];
    links = [];
    states = {};
    node2id = {};

    var i = 0;
    for (var nid in snapshots[s].solution.prpsearchnodes) {
        n = snapshots[s].solution.prpsearchnodes[nid]; // not doing anything with this for now
        nodes.push({name: n.name, id: i, weight: 1});
        node2id[nid] = i;
        i++;
    }

    for (i = 0; i < snapshots[s].solution.prpsearchnodelinks.length; ++i) {
        e = snapshots[s].solution.prpsearchnodelinks[i];
        links.push({source:node2id[e[0]], target:node2id[e[1]], id: i});
    }

    states = false;

    displayGraph(nodes, links, states, node2id);

}

function drawPSGraph(s) {
    currentSnapshot = s;

    nodes = [{name:'undefined', weight: 0}];
    links = [];
    states = {};
    node2id = {'-1':0};

    var i = 1;
    for (var nid in snapshots[s].solution.psgraph.nodes) {
        n = snapshots[s].solution.psgraph.nodes[nid];
        nname = n.action + ' ('+n.distance+')';
        init = false;
        if (n.distance == 0)
            nname = 'Goal';
        if (nid == snapshots[s].solution.psgraph.init)
            init = true;
        nodes.push({id: i, name: nname, data: n, init: init, weight: n.distance});
        node2id[''+nid] = i;
        i++;
    }

    for (i = 0; i < snapshots[s].solution.psgraph.edges.length; ++i) {
        e = snapshots[s].solution.psgraph.edges[i];

        if (">" === e[1])
            links.push({source:node2id[''+e[0]], target:node2id[''+e[2]]});
        else if ("<" === e[1])
            links.push({source:node2id[''+e[2]], target:node2id[''+e[0]]});
    }

    states = snapshots[s].solution.psgraph.states;

    displayGraph(nodes, links, states, node2id);

}

function initializeGraph() {

    //var margin = {top: -5, right: -5, bottom: -5, left: -5};
    var margin = {top: 0, right: 0, bottom: 0, left: 0};

    width = $('#graph').width() - margin.left - margin.right;
    height = $('#graph').height() - margin.top - margin.bottom;

    console.log("w/h: " + width + "/" + height);

    svg = d3.select("#graph");

    force = d3.forceSimulation()
              .force("charge", d3.forceManyBody().strength(-1000))
              //.force("center", d3.forceCenter(width / 2, height / 2))
              .force("links", d3.forceLink([]).distance(180).strength(0.5))
              //.force("collide",d3.forceCollide( function(d){return d.r + 8 }).iterations(16) )
              .force("x", d3.forceX())
              .force("y", d3.forceY())
              .alphaTarget(1)
              .on("tick", ticked);

    g = svg.append("g").attr("transform", "translate(" + width / 2 + "," + height / 2 + ")"),
    link = g.append("g").attr("stroke", "#000").attr("stroke-width", 1.5).selectAll(".link"),
    node = g.append("g").attr("stroke", "#fff").attr("stroke-width", 1.5).selectAll(".node");

    /*var zoom = d3.zoom()
      .scaleExtent([1, 10])
      .on("zoom", zoomed);

    drag = d3.drag()
      .subject(function(d) {
          return d;
      })
      .on("start", dragstarted)
      .on("drag", dragged)
      .on("end", dragended);*/




    /*svg = d3.select("#graph").append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.right + ")")
      .call(zoom);*/

    /*container = svg.append("g");
    link = container.append("g").selectAll(".link");
    node = container.append("g").selectAll(".node");*/

}

function applyNodeDetails() {

    node.on("mouseover", function(d) {

            if (d.data) {
                if (states)
                    $('#state').html('<pre>'+states[d.data.state].join("\n")+'</pre>');
                if (d.data.expected_successor) {
                    console.log('#node-'+node2id[d.data.expected_successor]);
                    d3.select('#node-'+node2id[d.data.expected_successor])
                      .transition()
                      .duration(750)
                      .attr("r", 26);
                  }
            }

            node.classed("node-active", function(o) {
                thisOpacity = isConnected(d, o) ? true : false;
                this.setAttribute('fill-opacity', thisOpacity);
                return thisOpacity;
            });

            link.classed("link-active", function(o) {
                return o.source === d || o.target === d ? true : false;
            });

            d3.select(this).classed("node-active", true);
            d3.select(this).select("circle").transition()
                .duration(750)
                .attr("r", (d.weight * 2 + 12) * 1.5);
        })

        .on("mouseout", function(d) {

            if (d.data) {
                $('#state').html('<pre></pre>');
                if (d.data.expected_successor)
                    d3.select('#node-'+node2id[d.data.expected_successor])
                      .transition()
                      .duration(750)
                      .attr("r", 13);
            }

            node.classed("node-active", false);
            link.classed("link-active", false);

            d3.select(this).select("circle").transition()
                .duration(750)
                .attr("r", d.weight * 2 + 12);
        });

    node.append("circle")
        .attr("r", function(d) {
            return d.weight * 2 + 12;
        })
        .style("fill", function(n) {
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


    // add the text
    node.append("text")
        .attr("x", 16)
        .attr("dy", ".35em")
        .text(function(d) { return d.name; });

    node.on("dblclick", function(d) {
        console.log("Node Info:");
        console.log(d);
    });

}

function displayGraph(nodes, links, states, node2id) {

    linkedByIndex = {};
    links.forEach(function(d) {
        linkedByIndex[d.source.index + "," + d.target.index] = 1;
    });

    node = node.data(nodes, function(d) {return d.id;});
    node.exit().remove();
    node = node.enter().append("g")
                       .attr("class", "node")
                       .attr("cx", function(d) {return d.x;})
                       .attr("cy", function(d) {return d.y;})
                       //.call(drag)
                       .merge(node);

    applyNodeDetails();

    link = link.data(links, function(d) {return d.source + "-" + d.target;});

    console.log("Removing: " + node.exit().data().length);

    link.exit().remove();
    link = link.enter().append("line")
                       .attr("class", "link")
                       .style("stroke-width", 3)
               .merge(link);

    force.nodes(node.data());
    force.force("links").links(link.data());//.id(function(d) {return d.index;}));
    force.alpha(1).restart();

}

function ticked() {
    if (node) {
        /*node.attr("transform", function(d) {
            return "translate(" + d.x + "," + d.y + ")";
        });*/
        node.attr("cx", function(d) {return d.x})
            .attr("cy", function(d) {return d.y});
    }
    if (link) {
        link.attr("x1", function(d) {
                return d.source.x;
            })
            .attr("y1", function(d) {
                return d.source.y;
            })
            .attr("x2", function(d) {
                return d.target.x;
            })
            .attr("y2", function(d) {
                return d.target.y;
            });
    }
}

function isConnected(a, b) {
    return linkedByIndex[a.index + "," + b.index];// || linkedByIndex[b.index + "," + a.index];
}

function dottype(d) {
    d.x = +d.x;
    d.y = +d.y;
    return d;
}

function zoomed() {
    container.attr("transform", d3.event.transform);
}

function dragstarted(d) {
    if (!d3.event.active) force.alphaTarget(0.3).restart();
    d3.event.subject.fx = d3.event.subject.x;
    d3.event.subject.fy = d3.event.subject.y;
}

function dragged(d) {
    //d3.select(this).attr("cx", d.x = d3.event.x).attr("cy", d.y = d3.event.y);
    d3.event.subject.fx = d3.event.x;
    d3.event.subject.fy = d3.event.y;
}

function dragended(d) {
    if (!d3.event.active) force.alphaTarget(0);
    d3.event.subject.fx = null;
    d3.event.subject.fy = null;
}
