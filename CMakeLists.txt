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


function(INSTALL_LIB targets)
    INSTALL(
        TARGETS ${LIB_NAME} ${LIB_NAME}_static OPTIONAL
        LIBRARY DESTINATION ${LIBRARY_DEST} COMPONENT ${LIB_NAME}
        ARCHIVE DESTINATION ${LIBRARY_DEST} COMPONENT ${LIB_NAME}
    )
    foreach(target IN LISTS targets)
       INSTALL(
            TARGETS ${LIB_NAME} ${LIB_NAME}_static OPTIONAL
            LIBRARY DESTINATION ${LIBRARY_DEST} COMPONENT ${target}
            ARCHIVE DESTINATION ${LIBRARY_DEST} COMPONENT ${target}
        )
    endforeach()
endfunction()

function(INSTALL_HEADER targets)
    INSTALL(
        FILES ${LIB_HEADER}
        DESTINATION ${LIBRARY_HEADER_DEST}
        COMPONENT ${LIB_NAME}
    )

    foreach(target IN LISTS targets)
        INSTALL(
            FILES ${LIB_HEADER}
            DESTINATION ${LIBRARY_HEADER_DEST}
            COMPONENT ${target}
        )
    endforeach()
endfunction()

ADD_SUBDIRECTORY(lib/sum)
ADD_SUBDIRECTORY(lib/list)
ADD_SUBDIRECTORY(lib/utest)

