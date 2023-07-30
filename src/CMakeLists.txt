cmake_minimum_required(VERSION 3.5)

project(AlgorithmicTrading VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cc
        view/view.cc
        view/view.h
        view/view.ui
        view/qcustomplot.h
        view/qcustomplot.cc
        view/paint_scene.h
        view/paint_scene.cc
        view/helper/generator.cc
        view/helper/generator.h
        controller/controller.h
        controller/controller.cc
        view/helper/pages.h
        model/polynomial_approximation/polynomial_approximation.h
        model/polynomial_approximation/polynomial_approximation.cc
        model/helpers/file_handler.h
        model/helpers/file_handler.cc
        model/helpers/matrix.h
        model/helpers/matrix.cc
        model/cubic_spline/cubic_spline.h
        model/cubic_spline/cubic_spline.cc
        model/facade/facade.h
        model/facade/facade.cc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AlgorithmicTrading
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AlgorithmicTrading APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AlgorithmicTrading SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AlgorithmicTrading
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AlgorithmicTrading PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(AlgorithmicTrading PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AlgorithmicTrading)
endif()
