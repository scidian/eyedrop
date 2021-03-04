#
#  Copyright (C) 2021 Scidian Software - All Rights Reserved
#
#  Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
#  Proprietary and Confidential
#  Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
#
#
cmake_minimum_required(VERSION 3.10)
####################################################################################
#
#   To compile, set target manually below:
#
#       "osx"       Apple MacOS                     (defines DROP_TARGET_APPLE)
#       "ios"       Apple iOS                       (defines DROP_TARGET_APPLE)
#       "web"       Emscripten                      (defines DROP_TARGET_HTML5)
#       "android"   Google Android                  (defines DROP_TARGET_GOOGLE)
#       "windows"   Microsoft Windows               (defines DROP_TARGET_WINDOWS)
#       "linux"     Linux                           (defines DROP_TARGET_LINUX)
#
####################################################################################


set(EXPORT_TARGET   "web")
set(DEBUG_MODE      "true")         # Options:  "true"    or  "false"
set(APP_TYPE        "player")       # Options:  "editor"  or  "player"


####################################################################################
##### DEBUG
if     (DEBUG_MODE MATCHES "true") 
    add_compile_definitions(DEBUG_ON)               ##### Signal we want Sokol Debug Viewer capability
    add_compile_definitions(SOKOL_TRACE_HOOKS)      ##### Needed for Sokol Debug Viewer
endif()

##### SELECT DESIRED EXECUTABLE (MAIN)
if     (APP_TYPE MATCHES "player")
    add_compile_definitions(SOKOL_NO_ENTRY)         ##### STOP SOKOL HIJACK
    set(MAIN_DIRECTORY "player/*.c**")
    project(player)                                 ##### Project Name for Engine Only
else()
    set(MAIN_DIRECTORY "editor/*.c**")
    project(drop)                                   ##### Project Name for Editor
endif()

##### TARGET
if     (EXPORT_TARGET MATCHES "osx")
    add_compile_definitions(DROP_TARGET_APPLE)
    add_compile_definitions(DROP_TARGET_OSX)
    add_compile_definitions(SOKOL_GLCORE33)         # MacOS, Windows, Linux, Switch, Playstation
    #add_compile_definitions(SOKOL_METAL)           # MacOS, iOS, tvOS
elseif (EXPORT_TARGET MATCHES "ios")
    add_compile_definitions(DROP_TARGET_APPLE)    
    add_compile_definitions(DROP_TARGET_IOS)
    add_compile_definitions(SOKOL_GLCORE33)         # MacOS, Windows, Linux, Switch, Playstation
    #add_compile_definitions(SOKOL_METAL)           # MacOS, iOS, tvOS
elseif (EXPORT_TARGET MATCHES "web") 
    add_compile_definitions(DROP_TARGET_HTML5)
    add_compile_definitions(SOKOL_GLES2)            # Android, WebAssembly
    #add_compile_definitions(SOKOL_GLES3)           # Android, WebAssembly
    #add_compile_definitions(SOKOL_WGPU)            # Next Gen WebAssembly
elseif (EXPORT_TARGET MATCHES "android") 
    add_compile_definitions(DROP_TARGET_GOOGLE)
    add_compile_definitions(SOKOL_GLES3)            # Android, WebAssembly
elseif (EXPORT_TARGET MATCHES "windows") 
    add_compile_definitions(DROP_TARGET_WINDOWS)
    add_compile_definitions(SOKOL_D3D11)            # Windows, XBox    
elseif (EXPORT_TARGET MATCHES "linux") 
    add_compile_definitions(DROP_TARGET_LINUX)
    add_compile_definitions(SOKOL_GLCORE33)         # MacOS, Windows, Linux, Switch, Playstation
endif()


##### Include Directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rd_party)


##### Glob the files, add the executable
file(GLOB SOURCE_CODE_FILES 
    ${MAIN_DIRECTORY}
    "*.c**"
    "3rd_party/*.c**"
    "3rd_party/imgui/*.c**"
    "3rd_party/mesh_optimizer/*.c**"
    "3rd_party/sokol/*.c**"
    "core/*.c**"
    "core/geometry/*.c**"
    "core/imaging/*.c**"
    "engine/*.c**"
    "engine/app/*.c**"
    "engine/app/filesystem.c**"
    "engine/data/*.c**"
    "engine/interface/*.c**"
    "engine/scene2d/*.c**"
    "engine/scene3d/*.c**"
    "engine/sceneui/*.c**"
)
add_executable(${PROJECT_NAME} ${SOURCE_CODE_FILES})


##### Compile Shaders
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    execute_process (
       COMMAND shaders/sokol_shdc_bin/osx/./sokol-shdc -i shaders/basic_shader.glsl -o shaders/basic_shader.glsl.h -l glsl330:glsl100:metal_macos
       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()


##### Copy 'assets' directory to 'build' directory
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/)


####################################################################################
# Sokol Gfx Requires Libraries:
# - on macOS with Metal:    Cocoa, IOKit, QuartzCore, Metal, MetalKit, Foundation
# - on macOS with GL:       Cocoa, IOKit, QuartzCore, OpenGL, Foundation
# - on iOS with Metal:      Foundation, UIKit, Metal, MetalKit
# - on iOS with GL:         Foundation, UIKit, OpenGLES, GLKit
# - on Linux:               X11, Xi, Xcursor, GL, dl, pthread, m(?)
# - on Android:             GLESv3, EGL, log, android
# - on Windows:             no action needed, libs are defined in-source via pragma-comment-lib
#
# Sokol Audio Requires
# - Windows:                WASAPI
# - Linux:                  ALSA (link with asound)
# - macOS/iOS:              CoreAudio (link with AudioToolbox & AVFoundation)
# - emscripten:             WebAudio with ScriptProcessorNode
# - Android:                OpenSLES (link with OpenSLES)
####################################################################################
if (EXPORT_TARGET MATCHES "osx")
    # To optimze file size in order: -O0, -O1, -O2, -O3
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c99 -xobjective-c -fobjc-arc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++11 -fobjc-arc")
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework Foundation")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal -framework MetalKit -framework AudioToolbox")
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
    find_library(METAL_LIBRARY Metal REQUIRED)
    find_library(METALKIT_LIBRARY MetalKit REQUIRED)
    find_library(AUDIOKIT_LIBRARY AudioToolbox REQUIRED)
    message(${COCOA_LIBRARY})
    message(${IOKIT_LIBRARY})
    message(${OPENGL_LIBRARY})
    message(${FOUNDATION_LIBRARY})
    message(${METAL_LIBRARY})
    message(${METALKIT_LIBRARY})
    message(${AUDIOKIT_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${COCOA_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${IOKIT_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${FOUNDATION_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${METAL_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${METALKIT_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${AUDIOKIT_LIBRARY})

####################################################################################
elseif (EXPORT_TARGET MATCHES "ios")

    ### TODO ###


####################################################################################
elseif (EXPORT_TARGET MATCHES "web")
    # To optimze file size in order: -O1, -O2, -O3, -Os, -Oz 
    if (DEBUG_MODE MATCHES "true")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -std=c++11 --shell-file ../shell.html -Werror -Wno-error=unused-command-line-argument") 
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz -std=c++11 --shell-file ../shell.html") 
    endif()
    set(CMAKE_C_COMPILER "emcc")
    set(CMAKE_CXX_COMPILER "emcc")
    SET(CMAKE_EXECUTABLE_SUFFIX ".html")

    #   --preload-file assets/shapes.png"       allow access to local filesystem
    #   --embed-file assets/shapes.png"         embed in js file, not compatible yet with sokol_app
    #
    #   --shell-file <path>                     create a html file without emscripten logo and debug shell
    #   -s WASM=0                               force javascript or
    #   -s WASM=1                               use webassembly
    #   -s ASSERTIONS=1                         show errors in javascript console
    #   -s ALLOW_MEMORY_GROWTH=1                allow for dynamic memory access
    #   -s TOTAL_MEMORY=x                       embed larger memory up front (default is 16777216... 33554432, 67108864, 134217728, 268435456
    if (DEBUG_MODE MATCHES "true")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s WASM=0 -std=c++11 --bind -s ASSERTIONS=2")    
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s WASM=0 -std=c++11 --bind ")
    endif()

    

####################################################################################    
elseif (EXPORT_TARGET MATCHES "android")

    ### TODO ###


####################################################################################
elseif (EXPORT_TARGET MATCHES "windows")

    ### TODO ###


####################################################################################
elseif (EXPORT_TARGET MATCHES "linux")

    ### TODO ###


endif()


