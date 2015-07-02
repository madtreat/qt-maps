var map;
var view;

function getCoords(lat, lon) {
   return ol.proj.transform([lon, lat], 'EPSG:4326', 'EPSG:3857');
}

function init() {
   view = new ol.View({
      center: getCoords(__LAT__, __LON__),
      zoom: __ZOOM__
   });
   
   map = new ol.Map({
      target: 'map',
      layers: [
         new ol.layer.Tile({
            source: new ol.source.MapQuest({layer: 'sat'})
         })//,
//         new ol.layer.Tile({
//            source: new ol.source.OSM({
//               crossOrigin: 'anonymous'
//            })
//         })
      ],
      view: view
   });
}

function panTo(lat, lon) {
   view.setCenter(getCoords(lat, lon));
}

function zoomTo(level) {
   view.setZoom(level);
}

function rotate(deg, lat, lon) {
   view.rotate(deg*Math.PI/180, getCoords(lat, lon));
}