cmake_minimum_required(VERSION 3.10)

project(lan7430-config LANGUAGES CXX VERSION 1.1.1)

set(_lib lib CACHE STRING "Basename of the library-directory")
set(_bin bin CACHE STRING "Basename of the bin-directory")
set(_include include CACHE STRING "Basename of the include-directory")

# Set the OUTPUT dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(TEST_REPORT_DIRECTORY ${CMAKE_BINARY_DIR}/reports)
file(MAKE_DIRECTORY ${TEST_REPORT_DIRECTORY})

include(cmake/TestSettings.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests.")
endif()

configure_file(
  ${CMAKE_SOURCE_DIR}/src/include/version.hpp.in
  ${CMAKE_BINARY_DIR}/generated/version.hpp
)

include_directories(${CMAKE_BINARY_DIR}/generated)

add_subdirectory(src)
