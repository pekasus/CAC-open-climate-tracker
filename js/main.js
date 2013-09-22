var Sensors = Sensors || {};

(function (Sensors, $, _) {

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

  $.getJSON('septa-climate-tracker/data/output.geojson', function (data) {

    var humiditySum = _.reduce(data.features, function (memo, point) { return memo + point.properties.humid; }, 0);
    var humidityAvg = humiditySum / data.features.length;

    var tempSum = _.reduce(data.features, function (memo, point) { return memo + point.properties.temp; }, 0);
    var tempAvg = tempSum / data.features.length;

    for (var j = 0; j < data.features.length; j = j + 1) {
      var tempDiff = (data.features[j].properties.temp - tempAvg) * 5;
      var h = 280 + tempDiff;
      var marker = new L.CircleMarker(
        new L.LatLng(
          data.features[j].geometry.coordinates[1],
          data.features[j].geometry.coordinates[0]),
        {
          opacity: 0.5,
          fill: true,
          fillOpacity: 0.7,
          color: 'hsl(' + h + ', 79%, 50%)'
        });
      var HumidDifference = data.features[j].properties.humid - humidityAvg;
      marker.setRadius(10 + HumidDifference);
      var html = '<h2><b>TEMP</b>: ' + data.features[j].properties.temp.toFixed(1) + '<br /><b>HUMIDITY</b>: ' + data.features[j].properties.humid.toFixed(1) + '</h2>';

      marker.bindPopup(html);
      dataLayer.addLayer(marker);
    }

    map.addLayer(dataLayer);
    map.fitBounds(dataLayer.getBounds());
  });

})(Sensors, jQuery, _);