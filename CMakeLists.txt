PROJECT(qt-gmaps)

SET(SOURCES
   form.cpp
   geocode_data_manager.cpp
   mainwindow.cpp
   mapsettings.cpp
)

SET(HEADERS
   form.h
   geocode_data_manager.h
   locationdata.h
   mainwindow.h
   mapsettings.h
)

SET(FORMS
   form.ui
   mainwindow.ui
)

SET(RESOURCES
   resource.qrc
)
QT5_ADD_RESOURCES(QRC ${RESOURCES})
SET_SOURCE_FILES_PROPERTIES(${QRC} PROPERTIES GENERATED ON)

ADD_LIBRARY(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${HEADERS}
   ${QRC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Network_LIBRARIES}
   ${Qt5WebKitWidgets_LIBRARIES}
)

# For testing purposes, it is easy to just add an additional executable to the build
ADD_EXECUTABLE(gmaps-test
   main.cpp
   ${SOURCES}
   ${HEADERS}
   ${QRC}
)

TARGET_LINK_LIBRARIES(gmaps-test
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Network_LIBRARIES}
   ${Qt5WebKitWidgets_LIBRARIES}
)
