var fs = require('fs'),
  GeoJSON = require('geojson');

var toFarenheit = function (celsius) {
  return (celsius * 1.8) + 32;
};

fs.readFile(__dirname + '/data/sample.csv', function(err, data) {
  if (err) {
    console.log(err);
    process.exit(1);
  }

  console.log('Reading file...');

  var records = data.toString().split('\n');

  var points = [];

  var writeRecord = function (i) {

    var record = records[i];

    var recordSplit = record.split('\t');

    var lat = recordSplit[0] / 100;
    var lng = (recordSplit[1] / 100) / -1;
    var temp = toFarenheit(recordSplit[2]);
    var humid = parseInt(recordSplit[3], 10);

    var point = {};
    point.temp = temp;
    point.humid = humid;
    point.lat = lat;
    point.lng = lng;

    points.push(point);

    if (i < records.length - 1) writeRecord(i + 1);
  };

  console.log('Attempting to insert ' + records.length, 'records...');
  writeRecord(0);
  console.log('Done parsing, converting to GeoJSON now...');
  console.log(points);
  GeoJSON.parse(points, {Point: ['lat', 'lng']}, function (geojson) {
    //console.log('Done converting, writing to output file now...');
    fs.writeFile(__dirname + '/data/output.geojson', JSON.stringify(geojson, null, 4), function (err) {
      if (err) return err;

      console.log('Done printing to file!');
    });
  });
});