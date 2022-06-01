##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (PotatoImGui VERSION 0.1 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/../../../Engine/cmake)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 20)
set                   (CMAKE_CXX_VISIBILITY_PRESET hidden)
set                   (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

include               (set_max_warning_level)
set_max_warning_level ()
include               (build_type)

# This include defines standard install locations.
# https://cmake.org/cmake/help/v3.11/module/GNUInstallDirs.html
include(GNUInstallDirs)

# This include provides some helper functions that simplify packaging.
# https://cmake.org/cmake/help/v3.11/module/CMakePackageConfigHelpers.html
include(CMakePackageConfigHelpers)

# If no build type was specified, set it to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# If no installation prefix is given manually, install locally.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING
            "The install location"
            FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(IMGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../imgui")

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." OFF)
option(BUILD_TESTS "Build tests." OFF)

###########################################    Platform Definitions     ############################################

#[[
set(PLATFORM_WINDOWS FALSE CACHE INTERNAL "")
set(PLATFORM_LINUX FALSE CACHE INTERNAL "")

if(WIN32)
  set(PLATFORM_WIN32 TRUE CACHE INTERNAL "Target platform: Win32") #WIN32 is a variable, so we cannot use string "WIN32"
  message("Target platform: Win32. SDK Version: " ${CMAKE_SYSTEM_VERSION})
  list(APPEND PROJECT_COMPILE_DEFINITIONS PLATFORM_WIN32=1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(PLATFORM_LINUX TRUE CACHE INTERNAL "Target platform: Linux")
  message("Target Platform: Linux")
  list(APPEND PROJECT_COMPILE_DEFINITIONS PLATFORM_LINUX=1)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()
]]

##################################################    Sources     ##################################################
# file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
# file(GLOB_RECURSE PROJECT_SOURCES source/*.c source/*.cpp)

set(IMGUI_INCLUDES
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/misc/cpp/imgui_stdlib.h
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/backends/imgui_impl_glfw.h
)

set(IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
)


set(PROJECT_HEADERS 
    include/${PROJECT_NAME}/ImGuiImplPotato.h
    include/${PROJECT_NAME}/ImGuiPotatoRenderer.h
)

set(PROJECT_SOURCES 
    source/ImGuiImplPotato.cpp
    source/ImGuiPotatoRenderer.cpp
)


if(PLATFORM_WIN32)
   # list(APPEND IMGUI_SOURCES ${IMGUI_PATH}/backends/imgui_impl_win32.cpp)
   # list(APPEND IMGUI_INCLUDES ${IMGUI_PATH}/backends/imgui_impl_win32.h)
elseif(PLATFORM_LINUX)
   # list(APPEND PROJECT_SOURCES
   #      source/Core/Linux/SharedLibrary.cpp
   # )
endif()


file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_EXPORT_HPP include/${PROJECT_NAME}/${PROJECT_NAME}Export.h)
set (PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES} 
  ${IMGUI_HEADERS} 
  ${IMGUI_SOURCES} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_MISC} 
  ${PROJECT_EXPORT_HPP})


include            (assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(conan)
include(conan_utility)
include(import_library)

add_conan_remote(bincrafter      https://api.bintray.com/conan/bincrafters/public-conan)
add_conan_remote(conan-community https://api.bintray.com/conan/conan-community/conan   )

# ADD NON-CONAN LIBRARIES HERE.
# Examples:
# - Header Only:
#   find_package  (GLM REQUIRED)
#   import_library(GLM_INCLUDE_DIRS)
# - Identical Debug and Release:
#   find_package  (OpenGL REQUIRED)
#   import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
# - Separate Debug and Release:
#   find_package  (Boost REQUIRED iostreams)
#   import_library(Boost_INCLUDE_DIRS Boost_IOSTREAMS_LIBRARY_DEBUG Boost_IOSTREAMS_LIBRARY_RELEASE)
# - You may also set the PROJECT_INCLUDE_DIRS and PROJECT_LIBRARIES instead of using import_library.

find_package(PotatoEngine CONFIG REQUIRED)

conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP BUILD missing)
list           (APPEND PROJECT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS_RELEASE})
list           (APPEND PROJECT_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_RELEASE})
foreach        (_LIB ${CONAN_LIBS_DEBUG})
  list         (APPEND PROJECT_LIBRARIES debug ${_LIB})
endforeach     ()  
foreach        (_LIB ${CONAN_LIBS_RELEASE})
  list         (APPEND PROJECT_LIBRARIES optimized ${_LIB})
endforeach     ()

############################################    Compile Definitions     ############################################

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} ${PROJECT_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../imgui>
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>   
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}> 
    source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES} PotatoEngine)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(${PROJECT_NAME} 
  EXPORT_FILE_NAME     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Export.h
  EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC
)

##################################################    Testing     ##################################################

##################################################  Installation  ##################################################
# Define the configuration file locations.
set(INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_BINDIR})
set(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_LIBDIR})
set(INSTALL_ARCHIVE_DIR ${CMAKE_INSTALL_LIBDIR})
set(INSTALL_CONFIG_DIR  ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)
set(PROJECT_CONFIG_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)

configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
         ${PROJECT_CONFIG_FILE}
         INSTALL_DESTINATION ${INSTALL_CONFIG_DIR})

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
  RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR})
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  ${PROJECT_NAME}Config DESTINATION ${INSTALL_CONFIG_DIR})
# export (TARGETS ${PROJECT_NAME}       FILE        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)

# Register the package in the user's package registry.
# https://cmake.org/cmake/help/v3.11/command/export.html
export(PACKAGE ${PROJECT_NAME})