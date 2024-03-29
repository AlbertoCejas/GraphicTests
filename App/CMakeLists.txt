##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (PotatoApp VERSION 0.1 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 20)

include               (set_max_warning_level)
set_max_warning_level ()
include               (build_type)

# This include defines standard install locations.
# https://cmake.org/cmake/help/v3.11/module/GNUInstallDirs.html
include(GNUInstallDirs)

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
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
# file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
# file(GLOB_RECURSE PROJECT_SOURCES source/*.c source/*.cpp)

set(PROJECT_HEADERS 
    include/${PROJECT_NAME}/PotatoApp.h
    include/${PROJECT_NAME}/PotatoAppSample.h

    include/${PROJECT_NAME}/Editor/Editor.h
)

set(PROJECT_SOURCES 
    source/main.cpp

    source/PotatoApp.cpp
    source/PotatoAppSample.cpp
    source/Editor/Editor.cpp
)

file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_MISC})

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

#TODO: ERASE, temp test
#find_package(Vulkan REQUIRED)
#include_directories(${Vulkan_INCLUDE_DIRS})

##################################################    Targets     ##################################################
add_executable(${PROJECT_NAME} ${PROJECT_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC PotatoEngine)
target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
#TODO: ERASE, temp test
#target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES} ${CMAKE_DL_LIBS} ${Vulkan_LIBRARIES})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES} ${CMAKE_DL_LIBS})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

##################################################    Postbuild   ##################################################
# Copy application config file (plugins, window properties...)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/config"
        ${CMAKE_CURRENT_BINARY_DIR})

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
    get_filename_component    (_NAME ${_SOURCE} NAME_WE)
    add_executable            (${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
    target_include_directories(${_NAME} PUBLIC 
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
      $<INSTALL_INTERFACE:include> PRIVATE source)
    target_include_directories(${_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
    target_link_libraries     (${_NAME} PUBLIC ${PROJECT_LIBRARIES})
    target_compile_definitions(${_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
    add_test                  (${_NAME} ${_NAME})
    set_property              (TARGET ${_NAME} PROPERTY FOLDER tests)
    assign_source_group       (${_SOURCE})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config RUNTIME DESTINATION bin)
install(DIRECTORY config/ RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
# install(EXPORT  ${PROJECT_NAME}-config DESTINATION cmake)
# export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}-config.cmake)
