##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (PotatoEngine VERSION 0.1 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
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

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." OFF)
option(BUILD_TESTS "Build tests." OFF)

###########################################    Platform Definitions     ############################################

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

##################################################    Sources     ##################################################
# file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
# file(GLOB_RECURSE PROJECT_SOURCES source/*.c source/*.cpp)

set(PROJECT_HEADERS 
    include/${PROJECT_NAME}/Collections/SortedVector.h

    include/${PROJECT_NAME}/Core/Application.h
    include/${PROJECT_NAME}/Core/Engine.h
    include/${PROJECT_NAME}/Core/PluginManager.h
    include/${PROJECT_NAME}/Core/Plugin.h
    include/${PROJECT_NAME}/Core/RendererPlugin.h
    include/${PROJECT_NAME}/Core/SharedLibrary.h
    include/${PROJECT_NAME}/Core/WindowManager.h
    include/${PROJECT_NAME}/Core/Window.h

    include/${PROJECT_NAME}/File/File.h
    include/${PROJECT_NAME}/File/FileSystem.h

    include/${PROJECT_NAME}/Math/Vector2.h

    include/${PROJECT_NAME}/Util/Assert.h
    include/${PROJECT_NAME}/Util/Macros.h

    include/${PROJECT_NAME}/Render/interface/IGPUDriverFactory.h
    include/${PROJECT_NAME}/Render/interface/IFence.h
    include/${PROJECT_NAME}/Render/interface/IPipelineState.h
    include/${PROJECT_NAME}/Render/interface/IRenderDevice.h
    include/${PROJECT_NAME}/Render/interface/IRenderContext.h
    include/${PROJECT_NAME}/Render/interface/IRenderPass.h
    include/${PROJECT_NAME}/Render/interface/IShader.h
    include/${PROJECT_NAME}/Render/interface/ISwapChain.h

    include/${PROJECT_NAME}/Render/include/BaseDeviceObject.h
    include/${PROJECT_NAME}/Render/include/BaseFence.h
    include/${PROJECT_NAME}/Render/include/BaseGPUDriverFactory.h
    include/${PROJECT_NAME}/Render/include/BasePipelineState.h
    include/${PROJECT_NAME}/Render/include/BaseRenderDevice.h
    include/${PROJECT_NAME}/Render/include/BaseRenderDeviceModern.h
    include/${PROJECT_NAME}/Render/include/BaseRenderContext.h
    include/${PROJECT_NAME}/Render/include/BaseRenderContextModern.h
    include/${PROJECT_NAME}/Render/include/BaseRenderPass.h
    include/${PROJECT_NAME}/Render/include/BaseShader.h
    include/${PROJECT_NAME}/Render/include/BaseSwapChain.h
    include/${PROJECT_NAME}/Render/include/GraphicsAccessories.h
    include/${PROJECT_NAME}/Render/include/GraphicsTypes.h
    include/${PROJECT_NAME}/Render/include/InputLayout.h
)

set(PROJECT_SOURCES 
    source/Core/Application.cpp
    source/Core/Engine.cpp
    source/Core/PluginManager.cpp
    source/Core/Plugin.cpp
    source/Core/WindowManager.cpp
    source/Core/Window.cpp

    source/File/FileSystem.cpp
    source/File/File.cpp

    source/Util/Assert.cpp
)

if(PLATFORM_WIN32)
   list(APPEND PROJECT_SOURCES
        source/Core/Windows/SharedLibrary.cpp
   )
elseif(PLATFORM_LINUX)
   list(APPEND PROJECT_SOURCES
        source/Core/Linux/SharedLibrary.cpp
   )
endif()

file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_EXPORT_HPP include/${PROJECT_NAME}/${PROJECT_NAME}Export.h)
set (PROJECT_FILES 
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_MISC} 
  ${PROJECT_EXPORT_HPP})


include            (assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(conan)
include(conan_utility)
include(import_library)

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

conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP BUILD missing)

list           (APPEND PROJECT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS_RELEASE})
list           (APPEND PROJECT_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_RELEASE})
foreach        (_LIB ${CONAN_LIBS_DEBUG})
  list         (APPEND PROJECT_LIBRARIES debug ${_LIB})
endforeach     ()  
foreach        (_LIB ${CONAN_LIBS_RELEASE})
  list         (APPEND PROJECT_LIBRARIES optimized ${_LIB})
endforeach     ()

list         (APPEND PROJECT_LIBRARIES ${CONAN_LIBS})


############################################    Compile Definitions     ############################################
include(json_parser)
file(READ config/defines.json defines_config_file)
sbeParseJson(define_entries defines_config_file)

# process actual compile defines only
set(entries_to_skip 0)

foreach(define_entry ${define_entries})
  # if not an actual compile define, skip
  if(entries_to_skip GREATER 0)
    MATH(EXPR entries_to_skip "${entries_to_skip}-1")
    continue()
  endif()
  # for each build type
  foreach(entry_num ${${define_entry}})
    # get uppercase buiild type
    set(uppercase_define_build_type ${${define_entry}_${entry_num}})
    string(TOUPPER ${uppercase_define_build_type} uppercase_define_build_type)
    # get compile definition without "define_entries" prefix
    string(REGEX REPLACE "[^.]*\\." "" final_define_entry ${define_entry})
    # add compile definition to cxx flags per build type
    set(CMAKE_CXX_FLAGS_${uppercase_define_build_type} "${CMAKE_CXX_FLAGS_${uppercase_define_build_type}} -D${final_define_entry}")
    # there will be a non-actual compile define, so skip one
    MATH(EXPR entries_to_skip "${entries_to_skip}+1")
  endforeach()
endforeach()

##################################################    Targets     ##################################################

add_library(${PROJECT_NAME} ${PROJECT_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CONAN_INCLUDE_DIRS}
  #  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include> 
  # INTERFACE
  #  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
    source)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_directories   (${PROJECT_NAME} PUBLIC ${CONAN_LIB_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES} ${CMAKE_DL_LIBS})
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
if(BUILD_TESTS)
  enable_testing       ()
  set                  (TEST_MAIN_NAME test_main)
  set                  (TEST_MAIN_SOURCES tests/internal/main.cpp)
  add_library          (${TEST_MAIN_NAME} OBJECT ${TEST_MAIN_SOURCES})
  target_link_libraries(${TEST_MAIN_NAME} ${PROJECT_NAME})
  set_property         (TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
  assign_source_group  (${TEST_MAIN_SOURCES})

  file(GLOB PROJECT_TEST_CPPS tests/*.cpp)
  foreach(_SOURCE ${PROJECT_TEST_CPPS})
    get_filename_component(_NAME ${_SOURCE} NAME_WE)
    add_executable        (${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
    target_link_libraries (${_NAME} ${PROJECT_NAME})
    add_test              (${_NAME} ${_NAME})
    set_property          (TARGET ${_NAME} PROPERTY FOLDER tests)
    assign_source_group   (${_SOURCE})
  endforeach()
endif()

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