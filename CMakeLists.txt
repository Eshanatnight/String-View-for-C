cmake_minimum_required(VERSION 3.20)
project(String_View_for_C C)

set(CMAKE_C_STANDARD                                             11)
set(BUILD_SHARED                                                 On)
set(BUILD_TESTS                                                  ON)
set(SRC_FILES                           src/string_view.c src/string_view.h)
set(TEST_TARGET                                   string_view_tests)
set(LIB_NAME                                      string_view_for_c)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)


# Build Libraries
# SHARED
if(${BUILD_SHARED})
    add_library(${LIB_NAME} SHARED
        ${SRC_FILES}
    )
endif()
if() # STATIC
    add_library(${LIB_NAME} STATIC
        ${SRC_FILES}
    )
endif()

# tests
if(${BUILD_TESTS})
    message("Building Tests...")
    add_executable(${TEST_TARGET}
        test/test.c
    )
    set_target_properties(${TEST_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(${TEST_TARGET} ${LIB_NAME})
    
    enable_testing()

    add_test(NAME ${TEST_TARGET}
    COMMAND $<TARGET_FILE:string_view_tests>
    )
endif()
