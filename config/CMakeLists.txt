project(qt-maps-config)

set(CONFIG
   google-maps.js
   ol-map.js
   google-maps.example.ini
)

install(FILES ${CONFIG} DESTINATION ext/qt-google-maps/config)

