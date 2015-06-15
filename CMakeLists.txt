project(qt-gmaps)

set(SOURCES
   form.cpp
   geocode_data_manager.cpp
   mainwindow.cpp
   mapsettings.cpp
)

set(HEADERS
   form.h
   geocode_data_manager.h
   locationdata.h
   mainwindow.h
   mapsettings.h
)

set(FORMS
   form.ui
   mainwindow.ui
)

set(RESOURCES
   resource.qrc
)
qt5_add_resources(QRC ${RESOURCES})
set_source_files_properties(${QRC} PROPERTIES GENERATED ON)

add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${HEADERS}
   ${QRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Network_LIBRARIES}
   ${Qt5Positioning_LIBRARIES}
   ${Qt5WebKitWidgets_LIBRARIES}
)

# For testing purposes, it is easy to just add an additional executable to the build
add_executable(gmaps-test
   main.cpp
)

target_link_libraries(gmaps-test
   ${PROJECT_NAME}
)
