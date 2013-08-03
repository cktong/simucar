for (i = 0; i < bind.data.length; i++) {    
    var callback = function(bind, i) {
      var chart = nv.models.lineWithFocusChart();
      
      chart.xAxis
          .tickFormat(d3.format(',g'))
          .axisLabel(bind.graphs[i]['x-display']);
      chart.x2Axis
          .tickFormat(d3.format(',g'))


      chart.yAxis
          .tickFormat(d3.format(',g'))
          .axisLabel(bind.graphs[i]['y-display']);
      chart.y2Axis
          .tickFormat(d3.format(',g'))


      d3.select('#charts #chart'+i)
          .datum(bind.data[i])
          .transition().duration(500)
          .call(chart);

      nv.utils.windowResize(chart.update);

      return chart;
    }
    nv.addGraph(callback(bind, i));
    
    
    //#####################################################
    graphCallback = function(bind) {
     var chart = nv.models.discreteBarChart()
    .x(function(d) { return d.label })
    .y(function(d) { return d.value })
    .staggerLabels(false)
    //.staggerLabels(historicalBarChart[0].values.length > 8)
    .tooltips(true)
    .showValues(false)

    d3.select('#chartBar')
    .datum([bind["bar_data"]])
    .transition().duration(500)
    .call(chart);
    nv.utils.windowResize(chart.update);

    return chart;
};
nv.addGraph(graphCallback(bind));
}