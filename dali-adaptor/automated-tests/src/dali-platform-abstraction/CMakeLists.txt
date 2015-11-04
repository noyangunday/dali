SET(PKG_NAME "dali-platform-abstraction")

SET(EXEC_NAME "tct-${PKG_NAME}-core")
SET(RPM_NAME "core-${PKG_NAME}-tests")

SET(CAPI_LIB "dali-platform-abstraction")

SET(TC_SOURCES
    utc-image-loading-load-completion.cpp
    utc-image-loading-cancel-all-loads.cpp
    utc-image-loading-cancel-some-loads.cpp
)

LIST(APPEND TC_SOURCES
    resource-collector.cpp
    ../dali-adaptor/dali-test-suite-utils/test-harness.cpp
    ../dali-adaptor/dali-test-suite-utils/dali-test-suite-utils.cpp
    tct-dali-platform-abstraction-core.cpp
    utc-image-loading-common.cpp
)

PKG_CHECK_MODULES(${CAPI_LIB} REQUIRED
    dali-core
    dali
)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -ggdb --coverage -Wall -Werror=return-type" )

# Shouldn't have to do this!
# But CMake's new auto-escape quote policy doesn't work right.
CMAKE_POLICY(SET CMP0005 OLD)
ADD_DEFINITIONS(-DTEST_IMAGE_DIR=\"\\\"${CMAKE_CURRENT_SOURCE_DIR}/../../images\\\"\" )

FOREACH(directory ${${CAPI_LIB}_LIBRARY_DIRS})
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -L${directory}")
ENDFOREACH(directory ${CAPI_LIB_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(
    ../../../
    ../../../adaptors/tizen
    ../../../platform-abstractions/tizen
    ${${CAPI_LIB}_INCLUDE_DIRS}
    ../dali-adaptor/dali-test-suite-utils
    /usr/include/freetype2
)

ADD_EXECUTABLE(${EXEC_NAME} ${EXEC_NAME}.cpp ${TC_SOURCES})
TARGET_LINK_LIBRARIES(${EXEC_NAME}
    ${${CAPI_LIB}_LIBRARIES}
)

INSTALL(PROGRAMS ${EXEC_NAME}
    DESTINATION ${BIN_DIR}/${EXEC_NAME}
)
