add_subdirectory(3rdparty/catch2)
find_package(spdlog QUIET)
find_package(CLI11 QUIET)
if(NOT spdlog_FOUND)
    message("using 3rdparty/spdlog")
    add_definitions(-DFMT_HEADER_ONLY) # for spdlog bundled fmt
    add_subdirectory(3rdparty/spdlog)
    add_library(spdlog::spdlog ALIAS spdlog)
else()
    message("using system spdlog")
endif()
if(NOT CLI11_FOUND)
    message("using 3rdparty/cli11")
    add_subdirectory(3rdparty/cli11)
    add_library(CLI11::CLI11 ALIAS CLI11)
else()
    message("using system cli11")
endif()

add_subdirectory(lib/lan7430conf)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/include/validators.hpp
)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lan7430-config.cpp
)

add_executable(${PROJECT_NAME}
    lan7430-config.cpp

    include/validators.hpp
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        CLI11::CLI11
        lan7430-config-lib
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${_bin}
        LIBRARY DESTINATION ${_lib}
        ARCHIVE DESTINATION ${_lib}
)
