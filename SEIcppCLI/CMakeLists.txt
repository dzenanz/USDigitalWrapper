cmake_minimum_required(VERSION 3.12)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib")

project(SEIcppCLI)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(Bitness "x64")
else()
  set(Bitness "x86")
endif()

add_library(SEIDrv SEIDrv.cpp)
set_target_properties(SEIDrv PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(SEIDrv PROPERTIES VS_DOTNET_REFERENCE_USDSEI "${CMAKE_CURRENT_LIST_DIR}/${Bitness}/USDSEI.dll")
set_target_properties(SEIDrv PROPERTIES VS_DOTNET_REFERENCE_USDigital "${CMAKE_CURRENT_LIST_DIR}/${Bitness}/USDigital.dll")

# required to display reference assemblies in Visual Studio GUI
set_property(TARGET SEIDrv PROPERTY VS_GLOBAL_CLRSupport "true")

add_executable(ExeTest ExeTest.cpp)
target_link_libraries(ExeTest SEIDrv)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ExeTest)
