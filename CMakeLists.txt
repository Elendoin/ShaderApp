cmake_minimum_required(VERSION 3.16)
project(App VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

# All your source files together
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    shaderlistwindow.ui
    imagewidget.cpp
    imagewidget.h
)

# Create executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(App
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        models/shadermodel.h
        shaderlistwindow.ui
        shaderlistwindow.h
        shaderlistwindow.cpp
        utils/filehelper.h
        utils/filehelper.cpp


    )
else()
    add_executable(App ${PROJECT_SOURCES})
endif()

# Link Qt libraries
target_link_libraries(App
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        opengl32
)

# Include directories
target_include_directories(App
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Bundle / platform stuff
set_target_properties(App PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_custom_command(TARGET App POST_BUILD
    # Clear old vertexshaders folder
    COMMAND ${CMAKE_COMMAND} -E rm -rf
            ${CMAKE_CURRENT_BINARY_DIR}/vertexshaders
    # Copy fresh version
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/vertexshaders
            ${CMAKE_CURRENT_BINARY_DIR}/vertexshaders

    # Clear old fragmentshaders folder
    COMMAND ${CMAKE_COMMAND} -E rm -rf
            ${CMAKE_CURRENT_BINARY_DIR}/fragmentshaders
    # Copy fresh version
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/fragmentshaders
            ${CMAKE_CURRENT_BINARY_DIR}/fragmentshaders

    COMMAND ${CMAKE_COMMAND} -E touch
                ${CMAKE_CURRENT_BINARY_DIR}/recent.txt
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(App)
endif()
