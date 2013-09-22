var Sensors = Sensors || {};

(function (Sensors, $) {

  var basemap = new L.StamenTileLayer('terrain');

  var map = L.map('map', {
    center: new L.LatLng(39.952335,-75.163789),
    zoom: 13,
    attributionControl: false,
    touchZoom: true,
    dragging: true,
    layers: basemap
  });

  var dataLayer = new L.FeatureGroup();

  $.getJSON('../data/output.geojson', function (data) {
    var points = [];
    for (var i = 0; i < data.features.length; i = i + 1) {
      points.push(new L.LatLng(data.features[i].geometry.coordinates[1], data.features[i].geometry.coordinates[0]));
    }
    var path = new L.Polyline(points,
      {
        color: '#000',
        weight: 8,
        opacity: 1
      });
    map.addLayer(path);
    map.fitBounds(path.getBounds());

    // Higher or lower than average instead of the first log record
    var origHumidity = data.features[0].properties.humid;
    var origTemp = data.features[0].properties.temp;

    for (var j = 0; j < data.features.length; j = j + 1) {
      var tempDiff = (data.features[j].properties.temp - origTemp) * 5;
      var h = 280 + tempDiff;
      var marker = new L.CircleMarker(
        new L.LatLng(
          data.features[j].geometry.coordinates[1],
          data.features[j].geometry.coordinates[0]),
        {
          opacity: 1,
          fill: true,
          fillOpacity: 1,
          color: 'hsl(' + h + ', 79%, 50%)'
        });
      var HumidDifference = data.features[j].properties.humid - origHumidity;
      marker.setRadius(10 + HumidDifference);
      var html = '<h2><b>TEMP</b>: ' + data.features[j].properties.temp.toFixed(1) + '<br /><b>HUMIDITY</b>: ' + data.features[j].properties.humid.toFixed(1) + '</h2>';

      marker.bindPopup(html);
      dataLayer.addLayer(marker);
    }

    map.addLayer(dataLayer);
  });

})(Sensors, jQuery);