var view;
var satLayer;
var hybLayer;
var osmLayer;
var map;

var iconIDs = [];
var iconFeatures = [];
var iconLayers = []; // one layer per icon
var iconElements = []; // one element per icon
var iconPopupOverlays = []; // one popup overlay per icon

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
      view: view,
      interactions: [],
      controls: []
   });
   
   map.on('click', function(evt) {
      var feature = map.forEachFeatureAtPixel(evt.pixel,
      function(feature, layer) {
         return feature;
      });
      if (feature) {
         var id = feature.get('id') - 1; // zero based, but icon 0 is not shown, so -1
         var geometry = feature.getGeometry();
         var coord = geometry.getCoordinates();
         iconPopupOverlays[id].setPosition(coord);
         var element = iconElements[id];
         var content = 'id : '+ feature.get('id') + '\n';
         $(element).popover({
            'placement': 'right',
            'html': true,
            'content': content
         });
         $(element).popover('show');
      } else {
         $(iconElements).each(function(index) {
            $(this).popover('destroy');
         });
      }
   });
   
   // change mouse cursor when over marker
   map.on('pointermove', function(e) {
      if (e.dragging) {
         $(iconElements).each(function(index) {
            $(this).popover('destroy');
         });
         return;
      }
      var pixel = map.getEventPixel(e.originalEvent);
      var hit = map.hasFeatureAtPixel(pixel);
      map.getTarget().style.cursor = hit ? 'pointer' : '';
   });
}

function resize() {
   setTimeout( function() { map.updateSize(); }, 200);
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

function updateIcon(id, lat, lon) {
   var feature = iconFeatures[id-1];
   if (feature) {
      feature.setGeometry(new ol.geom.Point(getCoords(lat, lon)));
      feature.set('id', id);
      
      iconLayers[id-1].refresh({force:true});
   }
}

function addNewIcon(id, lat, lon) {
   if (iconIDs.indexOf(id) >= 0) {
      updateIcon(id, lat, lon);
   }
   else {
      iconIDs.push(id);
      var iconFeature = new ol.Feature({
         geometry: new ol.geom.Point(getCoords(lat, lon)),
         id: id
      });
      var iconStyle = new ol.style.Style({
         image: new ol.style.Icon(/** @type {olx.style.IconOptions} */ ({
            anchor: [0.5, 24],
            anchorXUnits: 'fraction',
            anchorYUnits: 'pixels',
            opacity: 0.75,
            src: '../src/images/icons/airplane.png'
         }))
      });
      iconFeature.setStyle(iconStyle);
      iconFeatures.push(iconFeature);
      
      var vectorSource = new ol.source.Vector({
         features: [iconFeature]
      });
      var vectorLayer = new ol.layer.Vector({
         source: vectorSource
      });
      map.addLayer(vectorLayer);
      iconLayers.push(vectorLayer); // so we can iconcess it later
      
      var element = document.getElementById('popup' + id);
      iconElements.push(element);
      var popup = new ol.Overlay({
         element: element,
         positioning: 'bottom-center',
         stopEvent: false
      });
      map.addOverlay(popup);
      iconPopupOverlays.push(popup);
   }
}
