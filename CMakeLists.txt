cmake_minimum_required(VERSION 3.12)
project(samp-plugin-timerfix VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
set(PLUGIN_SOURCES
    src/main.cpp
    src/natives.cpp
    src/time.cpp
    src/timers.cpp
    src/sdk/amxplugin.cpp
)

# Header files
set(PLUGIN_HEADERS
    src/main.h
    src/natives.h
    src/time.h
    src/timers.h
    src/common.h
    src/sdk/amx/amx.h
    src/sdk/amx/sclinux.h
    src/sdk/plugincommon.h
)

# Create shared library
add_library(timerfix SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

include(CheckIncludeFile)

check_include_file(alloca.h HAVE_ALLOCA_H)
if(HAVE_ALLOCA_H)
    add_definitions(-DHAVE_ALLOCA_H)
endif()

check_include_file(inttypes.h HAVE_INTTYPES_H)
if(HAVE_INTTYPES_H)
    add_definitions(-DHAVE_INTTYPES_H)
endif()

check_include_file(malloc.h HAVE_MALLOC_H)
if(HAVE_MALLOC_H)
    add_definitions(-DHAVE_MALLOC_H)
endif()

check_include_file(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

# Include directories
target_include_directories(timerfix PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/amx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk
)

# Platform-specific settings
if(UNIX)
    # Linux build settings (from Makefile)
    target_compile_options(timerfix PRIVATE
        -m32
        -O3
        -w
        -Wall
        -fPIC
    )

    target_compile_definitions(timerfix PRIVATE LINUX)

    target_link_libraries(timerfix PRIVATE rt)

    set_target_properties(timerfix PROPERTIES
        PREFIX ""
        SUFFIX ".so"
        LINK_FLAGS "-m32"
    )

elseif(WIN32)
    # Use .def file for exports
    set_target_properties(timerfix PROPERTIES
        SUFFIX ".dll"
        LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/src/plugin.def"
    )

    # Release configuration optimization
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(timerfix PRIVATE /O2)
        set_target_properties(timerfix PROPERTIES
            LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/src/plugin.def /OPT:REF /OPT:ICF"
        )
    endif()
endif()

# Installation
install(TARGETS timerfix
    LIBRARY DESTINATION bin
    RUNTIME DESTINATION bin
)

# Optional: Install the include file
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/timerfix.inc")
    install(FILES timerfix.inc DESTINATION include)
endif()
