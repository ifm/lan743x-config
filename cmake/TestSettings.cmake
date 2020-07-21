# test helpers
# Catch2 should output in JUnit format, and output successful tests, too
set (CATCH2_DEFAULT_PARAMETERS -r junit)
macro (add_catch2_test)
    set(prefix ADD_CATCH2_TEST)
    set(oneValueArgs TARGET WORKING_DIR)
    cmake_parse_arguments(${prefix} "" "${oneValueArgs}" "" ${ARGN} )
    if(NOT DEFINED ${prefix}_WORKING_DIR)
        set(${prefix}_WORKING_DIR ${CMAKE_BINARY_DIR})
    endif()
    add_test(
        NAME ${${prefix}_TARGET}
        COMMAND ${${prefix}_TARGET} -o ${${prefix}_TARGET}.xml ${CATCH2_DEFAULT_PARAMETERS}
        WORKING_DIRECTORY ${${prefix}_WORKING_DIR}
    )
endmacro(add_catch2_test)
