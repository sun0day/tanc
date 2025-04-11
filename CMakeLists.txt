cmake_minimum_required(VERSION 3.10)

PROJECT(HAC)

# version
SET(LIBRARY_VERSION 0.1.0)

# lib names
SET(LIBS hac_sum)

# local build output path
SET(LIBRARY_OUTPUT_PATH ../hac)

# installation destination
SET(LIBRARY_DEST lib/hac)
SET(LIBRARY_HEADER_DEST include/hac)

# set C11 
SET(CMAKE_C_STANDARD 11)      
SET(CMAKE_C_STANDARD_REQUIRED ON)  
SET(CMAKE_C_EXTENSIONS OFF)   

ADD_SUBDIRECTORY(hac_sum)

if($ENV{TEST_RUN})
    ADD_SUBDIRECTORY(test)
endif()




