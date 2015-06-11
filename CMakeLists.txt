PROJECT(qt-google-maps)

SET(SOURCES
   form.cpp
   geocode_data_manager.cpp
   mainwindow.cpp
)

SET(HEADERS
   form.h
   geocode_data_manager.h
   mainwindow.h
)

SET(FORMS
   form.ui
   mainwindow.ui
)
#qt5_wrap_ui(FORMS_WRAPPED ${FORMS})

SET(RESOURCES
   resource.qrc
)
QT5_ADD_RESOURCES(QRC ${RESOURCES})
SET_SOURCE_FILES_PROPERTIES(${QRC} PROPERTIES GENERATED ON)

ADD_EXECUTABLE(${PROJECT_NAME}
   main.cpp
   ${SOURCES}
   ${HEADERS}
#   ${WRAPPED_FORMS}
   ${QRC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Network_LIBRARIES}
   ${Qt5WebKitWidgets_LIBRARIES}
   qjson
)
