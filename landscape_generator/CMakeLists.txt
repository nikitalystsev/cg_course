cmake_minimum_required(VERSION 3.5)

project(landscape_generator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        mainwindow.ui
        src/main.cpp
        src/mainwindow.cpp
        inc/mainwindow.h
        inc/point/point3D.h
        inc/point/point2D.h
        inc/vector/vector3D.h
        inc/vector/vector2D.h
        src/light.cpp
        inc/light.h
        inc/transform.h
        inc/landscape.h
        src/landscape.cpp
        inc/perlinNoise.h
        src/perlinNoise.cpp
        inc/plane.h
        src/plane.cpp
        inc/renderer.h
        src/renderer.cpp
        inc/lightManager.h
        src/lightManager.cpp
        inc/landscapeManager.h
        src/landscapeManager.cpp
#        inc/camera.h
#        src/camera.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(landscape_generator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET landscape_generator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(landscape_generator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(landscape_generator
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(landscape_generator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(landscape_generator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS landscape_generator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(landscape_generator)
endif()
