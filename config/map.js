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