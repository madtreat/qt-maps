var map;
var markers = [];

function initialize() {
   var mapOptions = {
      center: new google.maps.LatLng(__LAT__, __LON__),
      zoom: __ZOOM__,
      mapTypeId: google.maps.MapTypeId.__MAP_TYPE__,
      disableDefaultUI: __DISABLE_MAP_UI__
   };
   map = new google.maps.Map(document.getElementById("map_canvas"), mapOptions);
}
google.maps.event.addDomListener(window, 'load', initialize);

function resize() {
   // Resize the map
}

function panTo(lat, lon) {
   map.panTo(new google.maps.LatLng(lat, lon), 500);
}

function zoomTo(level) {
   map.setZoom(level);
}

function rotate(deg, lat, lon) {
   // Google does not currently provide map rotation through its API
}
