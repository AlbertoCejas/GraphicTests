##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (VKRenderer VERSION 0.1 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake; ${PROJECT_SOURCE_DIR}/../../Engine/cmake)
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

set(BUILD_SHARED_LIBS ON)

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
    include/${PROJECT_NAME}/Core/Types.h

    include/${PROJECT_NAME}/Core/interface/VKICommandQueue.h
    include/${PROJECT_NAME}/Core/interface/VKIFactory.h
    include/${PROJECT_NAME}/Core/interface/VKIFence.h
    include/${PROJECT_NAME}/Core/interface/VKIRenderDevice.h
    include/${PROJECT_NAME}/Core/interface/VKIRenderContext.h
    include/${PROJECT_NAME}/Core/interface/VKIRenderPass.h
    include/${PROJECT_NAME}/Core/interface/VKIShader.h
    include/${PROJECT_NAME}/Core/interface/VKIPipelineState.h
    include/${PROJECT_NAME}/Core/interface/VKISwapChain.h

    include/${PROJECT_NAME}/Core/include/Utils/VKConversions.h
    include/${PROJECT_NAME}/Core/include/VKCommandBuffer.h
    include/${PROJECT_NAME}/Core/include/VKCommandBufferPool.h
    include/${PROJECT_NAME}/Core/include/VKCommandQueue.h
    include/${PROJECT_NAME}/Core/include/VKFactory.h
    include/${PROJECT_NAME}/Core/include/VKFence.h
    include/${PROJECT_NAME}/Core/include/VKFencePool.h
    include/${PROJECT_NAME}/Core/include/VKInstance.h
    include/${PROJECT_NAME}/Core/include/VKLogicalDevice.h
    include/${PROJECT_NAME}/Core/include/VKObjectWrapper.h
    include/${PROJECT_NAME}/Core/include/VKPipelineState.h
    include/${PROJECT_NAME}/Core/include/VKPhysicalDevice.h
    include/${PROJECT_NAME}/Core/include/VKRenderDevice.h
    include/${PROJECT_NAME}/Core/include/VKRenderContext.h
    include/${PROJECT_NAME}/Core/include/VKRenderPass.h
    include/${PROJECT_NAME}/Core/include/VKShader.h
    include/${PROJECT_NAME}/Core/include/VKShaderResourceLayout.h
    include/${PROJECT_NAME}/Core/include/VKSwapChain.h

    include/${PROJECT_NAME}/Debug/VKDebug.h
)

set(PROJECT_SOURCES 
    source/VKRendererPlugin.cpp

    source/Core/Utils/VKConversions.cpp
    source/Core/VKCommandBuffer.cpp
    source/Core/VKCommandBufferPool.cpp
    source/Core/VKCommandQueue.cpp
    source/Core/VKFactory.cpp
    source/Core/VKFence.cpp
    source/Core/VKFencePool.cpp
    source/Core/VKInstance.cpp
    source/Core/VKLogicalDevice.cpp
    source/Core/VKPipelineState.cpp
    source/Core/VKPhysicalDevice.cpp
    source/Core/VKRenderDevice.cpp
    source/Core/VKRenderContext.cpp
    source/Core/VKRenderPass.cpp
    source/Core/VKShader.cpp
    source/Core/VKShaderResourceLayout.cpp
    source/Core/VKSwapChain.cpp

    source/Debug/VKDebug.cpp
)

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

# Potato Engine
find_package(PotatoEngine CONFIG REQUIRED)

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} ${PROJECT_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC PotatoEngine)
target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    #  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>  
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}> 
    source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES} ${CMAKE_DL_LIBS} ${Vulkan_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(${PROJECT_NAME} 
  EXPORT_FILE_NAME     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Export.h
  EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC
)

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