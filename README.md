qt-maps
==============
Qt5 API for displaying maps using CMake.

This repo was forked from [https://github.com/YuTinCH/qt-google-map](https://github.com/YuTinCH/qt-google-map).  The first major change was the addition of a CMakeLists.txt file to make this work with CMake in my other projects.  I then decided it could be made significantly more flexible, so I have rather extensively modified and added to the code.

This project now works with [http://openlayers.org](OpenLayers.org) maps as well, which I needed to use for the encapsulating project instead of Google maps, which cannot be rotated, a requirement for that particular project.

NOTE: this project will currently not compile on its own.  This may change in the future, but for now, I only need it in an already-established cmake project.
