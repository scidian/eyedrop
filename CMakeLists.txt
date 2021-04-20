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
#   BEFORE COMPILING, set the following options below!
#
#   EXPORT_TARGET:
#       "auto"      Attempt Auto-Detect
#       "osx"       Apple MacOS                     defines DROP_TARGET_OSX     SOKOL_GLCORE33      DROP_TARGET_APPLE   DROP_TARGET_OSX_MENUS
#       "ios"       Apple iOS                       defines DROP_TARGET_IOS     SOKOL_GLCORE33      DROP_TARGET_APPLE
#       "web"       Emscripten                      defines DROP_TARGET_HTML5   SOKOL_GLES2
#       "android"   Google Android                  defines DROP_TARGET_GOOGLE
#       "windows"   Microsoft Windows               defines DROP_TARGET_WINDOWS SOKOL_D3D11
#       "linux"     Linux                           defines DROP_TARGET_LINUX
#
#   APP_TYPE
#       "editor"    Drop Editor                     defines ENABLE_IMGUI
#       "player"    Drop Runtime                    ...does not have ImGui enabled unless DEBUG_MODE is set to "true"
#
#   DEBUG_MODE
#       "true"                                      defines SOKOL_TRACE_HOOKS, ENABLE_DEBUG, ENABLE_IMGUI (turns on ImGui and Sokol Gfx debug menu by default)
#       "false"                                     
####################################################################################
#################### Build Options
####################################################################################

# Standard editor build:
set(EXPORT_TARGET   "osx")
set(APP_TYPE        "editor")
set(DEBUG_MODE      "true")

# For testing no-gui html5 build:
# set(EXPORT_TARGET   "web")
# set(APP_TYPE        "player")
# set(DEBUG_MODE      "false")  


####################################################################################
####################################################################################
#################### Possible Compile Defitions
#
#   System
#       DROP_TARGET_APPLE                           ##### True when compiling for either MacOS or iOS
#       DROP_TARGET_OSX                             ##### True when compiling for MacOS
#       DROP_TARGET_OSX_MENUS                       ##### Use Mac main menu bar instead of ImGui for main menu
#       DROP_TARGET_IOS                             ##### True when compiling for iOS
#
#       DROP_TARGET_HTML5                           ##### True when compiling for javascript (emscripten)
#       DROP_TARGET_GOOGLE                          ##### True when compiling for Android
#       DROP_TARGET_WINDOWS                         ##### True when compiling for Windows
#       DROP_TARGET_LINUX                           ##### True when compiling for Linux
#
#   Sokol
#       SOKOL_TRACE_HOOKS                           ##### Needed for Sokol Debug Viewer
#       SOKOL_GLCORE33                              ##### OpenGL Core 3.3 Backend   - MacOS, Windows, Linux, Switch, Playstation
#       SOKOL_GLES2                                 ##### OpenGLES 2.0 Backend      - Android, WebAssembly
#       SOKOL_GLES3                                 ##### OpenGLES 3.0 Backend      - Android, WebAssembly
#       SOKOL_D3D11                                 ##### DirectX 11 Backend        - Windows, XBox    
#       SOKOL_METAL                                 ##### Metal Backend             - MacOS, iOS, tvOS
#
#   ImGui
#       ENABLE_IMGUI                                ##### Signal we want ImGui available to application
#
#   Debug
#       ENABLE_DEBUG                                ##### Signal we want Sokol Debug Viewer capability
#
####################################################################################
####################################################################################
#################### Program to Build
####################################################################################
##### SELECT DESIRED EXECUTABLE (MAIN)
if      (APP_TYPE MATCHES "player")
    add_compile_definitions(SOKOL_NO_ENTRY)         ##### STOP SOKOL HIJACK
    file(GLOB_RECURSE MAIN_DIRECTORY_FILES
        "player/*.c**"
    )
    project(player)                                 ##### Project Name for Engine Runtime Only
elseif  (APP_TYPE MATCHES "editor")  
    add_compile_definitions(SOKOL_NO_ENTRY)         ##### STOP SOKOL HIJACK    
    add_compile_definitions(ENABLE_IMGUI)           ##### Signal we want ImGui available to application
    file(GLOB_RECURSE MAIN_DIRECTORY_FILES
        "editor/*.c**"
    )
    project(drop)                                   ##### Project Name for Editor
endif()
####################################################################################
####################################################################################
#################### Target Definitions
####################################################################################
##### DEBUG??
if     (DEBUG_MODE MATCHES "true") 
    add_compile_definitions(SOKOL_TRACE_HOOKS)      ##### Needed for Sokol Debug Viewer
    add_compile_definitions(ENABLE_DEBUG)           ##### Signal we want Sokol Debug Viewer capability
    add_compile_definitions(ENABLE_IMGUI)           ##### Signal we want ImGui available to application
endif()

##### TARGET??
if     (EXPORT_TARGET MATCHES "auto") 
    if     (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(EXPORT_TARGET "osx")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(EXPORT_TARGET "windows")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(EXPORT_TARGET "linux")
    endif()
endif()
if     (EXPORT_TARGET MATCHES "osx")
    add_compile_definitions(DROP_TARGET_APPLE)
    add_compile_definitions(DROP_TARGET_OSX)
    add_compile_definitions(DROP_TARGET_OSX_MENUS)
    add_compile_definitions(SOKOL_GLCORE33)         ##### OpenGL Core 3.3 Backend   - MacOS, Windows, Linux, Switch, Playstation
    #add_compile_definitions(SOKOL_METAL)           ##### Metal Backend             - MacOS, iOS, tvOS
elseif (EXPORT_TARGET MATCHES "ios")
    add_compile_definitions(DROP_TARGET_APPLE)    
    add_compile_definitions(DROP_TARGET_IOS)
    add_compile_definitions(SOKOL_GLCORE33)         ##### OpenGL Core 3.3 Backend   - MacOS, Windows, Linux, Switch, Playstation
    #add_compile_definitions(SOKOL_METAL)           ##### Metal Backend             - MacOS, iOS, tvOS
elseif (EXPORT_TARGET MATCHES "web") 
    add_compile_definitions(DROP_TARGET_HTML5)
    add_compile_definitions(SOKOL_GLES2)            ##### OpenGLES 2.0 Backend      - Android, WebAssembly
    #add_compile_definitions(SOKOL_GLES3)           ##### OpenGLES 3.0 Backend      - Android, WebAssembly
    #add_compile_definitions(SOKOL_WGPU)            ##### WebGL Backend             - Next Gen WebAssembly
elseif (EXPORT_TARGET MATCHES "android") 
    add_compile_definitions(DROP_TARGET_GOOGLE)
    add_compile_definitions(SOKOL_GLES3)            ##### OpenGLES 3.0 Backend      - Android, WebAssembly
elseif (EXPORT_TARGET MATCHES "windows") 
    add_compile_definitions(DROP_TARGET_WINDOWS)
    add_compile_definitions(SOKOL_D3D11)            ##### DirectX 11 Backend        - Windows, XBox   
elseif (EXPORT_TARGET MATCHES "linux") 
    add_compile_definitions(DROP_TARGET_LINUX)
    add_compile_definitions(SOKOL_GLCORE33)         ##### OpenGL Core 3.3 Backend   - MacOS, Windows, Linux, Switch, Playstation
endif()
####################################################################################
####################################################################################
#################### Files to Include
####################################################################################
##### Include Directories, allows for easy #includes...
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rd_party)

##### Glob the files, add the executable
file(GLOB_RECURSE 3RD_PARTY_CODE_FILES      "3rd_party/*.c**")
if     (EXPORT_TARGET MATCHES "osx")
    file(GLOB_RECURSE ENGINE_CODE_FILES     "engine/*.c**" "engine/*.mm")
else()
    file(GLOB_RECURSE ENGINE_CODE_FILES     "engine/*.c**")
endif()
file(GLOB SOURCE_CODE_FILES                 
    ${MAIN_DIRECTORY_FILES}
    ${3RD_PARTY_CODE_FILES}
    ${ENGINE_CODE_FILES}
    "*.c**"
)
add_executable(${PROJECT_NAME} ${SOURCE_CODE_FILES})
####################################################################################
####################################################################################
#################### Pre Build Steps
####################################################################################
##### Change output directory thanks to iCloud constatly backing up builds
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build.nosync/)
    set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR} CACHE PATH "Build directory" FORCE)
endif()

##### Compile Shaders
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    execute_process (
       COMMAND tools/sokol_shdc_bin/osx/./sokol-shdc -i engine/scene3d/shaders/BasicShader.glsl -o engine/scene3d/shaders/BasicShader.glsl.h -l glsl330:glsl100:metal_macos:hlsl4
       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

##### Copy 'assets' directory to 'build' directory
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/output/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/)
####################################################################################
####################################################################################
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
    #    
    # -O#                   Optimze file size in order: -O0, -O1, -O2, -O3, 0s (size), -0fast (speed)
    # -s                    Strip debug info from binary, saves ~1mb
    # -fno-exceptions       Turns off "try {} catch {}" blocks, and std::except(), saves ~500kb
    # -fobjc-arc            MacOS ARC object counting, needed for Sokol
    #
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Os -std=c99   -fno-exceptions -fobjc-arc -xobjective-c")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -std=c++11 -fno-exceptions")
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
    # Flags
    #   -O#                                     Optimization, file size in order: -O1, -O2, -O3, -Os, -Oz (seems smallest)
    #   --shell-file <path>                     Create a html file without emscripten logo and debug shell
    #   -fno-exceptions                         Turns off "try {} catch {}" blocks, and std::except(), saves ~500kb
    #
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c99   -Oz -fno-exceptions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Oz -fno-exceptions --shell-file ../shell_full.html") # -Werror -Wno-error=unused-command-line-argument") 
    set(CMAKE_C_COMPILER "emcc")
    set(CMAKE_CXX_COMPILER "emcc")
    SET(CMAKE_EXECUTABLE_SUFFIX ".html")

    #   -s WASM=0                               Force javascript (no web assembly, more compatiable, but bigger file size) or
    #   -s WASM=1                               Use web assembly (!!!!! #NOTE: MUST RUN ON WEB SERVER FOR IT TO WORK!!!!!) ('.wast' file is human readable, don't need it!)
    #   -s ASSERTIONS=1|2                       Show errors in javascript console
    #   -s ALLOW_MEMORY_GROWTH=1                Allow for dynamic memory access
    #   -s DEMANGLE_SUPPORT=1                   Demangle stack traces, that is, emit human-readable C++ function names instead of _ZN.. ones
    #   -s TOTAL_MEMORY=x                       Embed larger memory up front (default is 16777216... 33554432, 67108864, 134217728, 268435456
    #
    #   --bind                                  Allows javascript code to call c++ functions
    #   --closure 1                             Turn on Googles closure compiler, reduces code size and improves javascript code
    #   --memory-init-file 0                    Embed the usually external .mem file generated along side .js file
    #   --preload-file assets/shapes.png        Include file in compiled code, allows access to files in simulated local filesystem
    #   --embed-file assets/shapes.png          Embed file in compiled code, also allows access through simulated filesystem (not compatible yet with sokol_app)
    #
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-fno-exceptions -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s TOTAL_MEMORY=134217728 -s WASM=1 -std=c++11 --bind")# -s ASSERTIONS=1")

####################################################################################    
elseif (EXPORT_TARGET MATCHES "android")


    ### TODO ###


####################################################################################
elseif (EXPORT_TARGET MATCHES "windows")
    ### Don't need to do anything for MSVC compiler on Windows ###

####################################################################################
elseif (EXPORT_TARGET MATCHES "linux")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    ### TODO ###


endif()


