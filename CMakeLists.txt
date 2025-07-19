cmake_minimum_required(VERSION 3.10)

PROJECT(TANC)

# version
SET(LIBRARY_VERSION 0.1.0)

# local build output path
SET(LIBRARY_OUTPUT_PATH ../../lib)

# installation destination
SET(LIBRARY_DEST lib/tanc)
SET(LIBRARY_HEADER_DEST include/tanc)

# set C11 
SET(CMAKE_C_STANDARD 11)      
SET(CMAKE_C_STANDARD_REQUIRED ON)  
SET(CMAKE_C_EXTENSIONS OFF)   

# install shared & static libs
function(INSTALL_LIB targets)
    foreach(target IN LISTS targets)
       INSTALL(
            TARGETS ${LIB_NAME} ${LIB_NAME}_static OPTIONAL
            LIBRARY DESTINATION ${LIBRARY_DEST} COMPONENT ${target}
            ARCHIVE DESTINATION ${LIBRARY_DEST} COMPONENT ${target}
        )
    endforeach()
endfunction()

# install header files
function(INSTALL_HEADER targets)
    foreach(target IN LISTS targets)
        INSTALL(
            FILES ${LIB_HEADER}
            DESTINATION ${LIBRARY_HEADER_DEST}
            COMPONENT ${target}
        )
    endforeach()
endfunction()

# preset lib target
function(SET_LIB_TARGET) 
    SET_TARGET_PROPERTIES(${LIB_NAME}_static PROPERTIES OUTPUT_NAME ${LIB_NAME})
    SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES OUTPUT_NAME ${LIB_NAME})
    SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES VERSION ${LIBRARY_VERSION}) 
endfunction()

# add include dirs for all libs
function(ADD_LIB targets)
    foreach(target IN LISTS targets)
        INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/${target})
        ADD_SUBDIRECTORY(lib/${target})
    endforeach()
endfunction()

# run unit tests
function(UT_RUN)
    if($ENV{TEST_RUN})
        LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)
        ADD_EXECUTABLE(${LIB_TEST_BIN} ${LIB_TEST_SRC})
        TARGET_INCLUDE_DIRECTORIES(${LIB_TEST_BIN} PUBLIC ${CMAKE_SOURCE_DIR}/lib/utest)
        TARGET_COMPILE_DEFINITIONS(${LIB_TEST_BIN} PRIVATE TANC_UT_ON=1)
        TARGET_LINK_LIBRARIES(${LIB_TEST_BIN} PRIVATE utest_static list_static ${LIB_NAME}_static)
        ADD_DEPENDENCIES(${LIB_TEST_BIN} utest_static list_static ${LIB_NAME}_static) 
    endif()
endfunction()

ADD_LIB("common;list;slist;utest")
