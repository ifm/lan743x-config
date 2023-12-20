cmake_minimum_required( VERSION 3.10 )

project(lan7430_lib_test CXX)

set(SOURCES
    shared.hpp
    000-main.cpp
    010-testMac.cpp
    020-testWriteEeprom.cpp
    021-testReadEeprom.cpp
    030-testByte.cpp
)
set(TEST_FILES
    files/00-80-0F-74-30-01-default.bin
    files/00-02-01-23-10-55-pad.bin
    files/00-02-01-23-10-56-pad.bin
    files/00-02-01-23-10-57-pad.bin
    files/00-02-01-23-10-58-pad.bin
    files/00-80-0F-74-30-01-default.bin
    files/00-80-0F-74-30-01-default_DevId.bin
    files/00-80-0F-74-30-01-default_DevId_OTP.bin
    files/00-80-0F-74-30-01-default_MAC.bin
    files/00-80-0F-74-30-01-L1_substates_all_on.bin
    files/00-80-0F-74-30-01-L1_substates_off.bin
    files/00-80-0F-74-30-01-L1_substates_on.bin
    files/00-80-0F-74-30-01-random.bin
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    spdlog::spdlog
    catch2
    lan7430-config-lib
)

add_catch2_test(
    TARGET ${PROJECT_NAME}
    WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR}
)
foreach(TEST_FILE ${TEST_FILES}) 
    configure_file( ${TEST_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE} COPYONLY)
endforeach()
