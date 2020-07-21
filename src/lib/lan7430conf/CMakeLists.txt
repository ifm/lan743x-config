project(lan7430-config-lib LANGUAGES CXX VERSION 0.0.1)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lan7430conf/lan7430conf.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lan7430conf/errors.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lan7430conf/byte.hpp
)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lan7430conf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/errors.cpp
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_definitions(-DSPDLOG_HEADER_ONLY) # tell spdlog to be used as header only
add_library( ${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
    EXPORT_FILE_NAME lan7430conf/${PROJECT_NAME}_export.h
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Boost_LIBRARIES}
        spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/> # for lan7430conf/[...]_export.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${_include}>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/> # for lan7430conf/[...]_export.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#------------------
# Tests
#------------------
if(ENABLE_TESTING)
  add_subdirectory(test)
endif()

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION ${_bin}
    LIBRARY DESTINATION ${_lib}
    ARCHIVE DESTINATION ${_lib}
    PUBLIC_HEADER DESTINATION ${_include}/lan7430conf
)
install(FILES
    ${PROJECT_BINARY_DIR}/lan7430conf/${PROJECT_NAME}_export.h DESTINATION ${_include}/lan7430conf
)
