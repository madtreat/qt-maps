var view;
var satLayer;
var hybLayer;
var osmLayer;
var map;

function getCoords(lat, lon) {
   return ol.proj.transform([lon, lat], 'EPSG:4326', 'EPSG:3857');
}

function init() {
   view = new ol.View({
      center: getCoords(__LAT__, __LON__),
      zoom: __ZOOM__
   });
   
   satLayer = new ol.layer.Tile({
      source: new ol.source.MapQuest({layer: 'sat'})
   });
   hybLayer = new ol.layer.Tile({
      source: new ol.source.MapQuest({layer: 'hyb'})
   });
   osmLayer = new ol.layer.Tile({
      source: new ol.source.MapQuest({layer: 'osm'}),
      visible: false
   });
   
   map = new ol.Map({
      target: 'map',
      layers: [satLayer, hybLayer, osmLayer],
      allOverlays: true,
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

function removeSatLayer() {
   satLayer.set('visible', false);
   hybLayer.set('visible', false);
   osmLayer.set('visible', true);
}

function addSatLayer() {
   osmLayer.set('visible', false);
   satLayer.set('visible', true);
   hybLayer.set('visible', true);
}