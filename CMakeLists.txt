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

ADD_SUBDIRECTORY(lib/sum)
ADD_SUBDIRECTORY(lib/list)
ADD_SUBDIRECTORY(lib/utest)

if($ENV{TEST_RUN})
    ADD_SUBDIRECTORY(test)
endif()