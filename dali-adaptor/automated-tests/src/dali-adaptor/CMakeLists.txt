SET(PKG_NAME "dali-adaptor")

SET(EXEC_NAME "tct-${PKG_NAME}-core")
SET(RPM_NAME "core-${PKG_NAME}-tests")

SET(CAPI_LIB "dali-adaptor")
SET(TC_SOURCES
    utc-Dali-Key.cpp
    utc-Dali-PixmapImage.cpp
    utc-Dali-SingletonService.cpp
    utc-Dali-Window.cpp
    utc-Dali-Timer.cpp
    utc-Dali-TtsPlayer.cpp
    utc-Dali-Application.cpp
    utc-Dali-FileLoader.cpp
)

LIST(APPEND TC_SOURCES
    dali-test-suite-utils/test-harness.cpp
    dali-test-suite-utils/dali-test-suite-utils.cpp
    dali-test-suite-utils/test-application.cpp
    dali-test-suite-utils/test-gesture-manager.cpp
    dali-test-suite-utils/test-gl-abstraction.cpp
    dali-test-suite-utils/test-gl-sync-abstraction.cpp
    dali-test-suite-utils/test-native-image.cpp
    dali-test-suite-utils/test-platform-abstraction.cpp
    dali-test-suite-utils/test-render-controller.cpp
    dali-test-suite-utils/test-trace-call-stack.cpp
)

PKG_CHECK_MODULES(${CAPI_LIB} REQUIRED
    dali-core
    dali
    ecore
)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -ggdb --coverage -Wall -Werror=return-type")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${${CAPI_LIB}_CFLAGS_OTHER}")

ADD_DEFINITIONS(-DTEST_RESOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../../resources\" )

FOREACH(directory ${${CAPI_LIB}_LIBRARY_DIRS})
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -L${directory}")
ENDFOREACH(directory ${CAPI_LIB_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(
    ../../../
    ${${CAPI_LIB}_INCLUDE_DIRS}
    dali-test-suite-utils
)

ADD_EXECUTABLE(${EXEC_NAME} ${EXEC_NAME}.cpp ${TC_SOURCES})
TARGET_LINK_LIBRARIES(${EXEC_NAME}
    ${${CAPI_LIB}_LIBRARIES}
)

INSTALL(PROGRAMS ${EXEC_NAME}
    DESTINATION ${BIN_DIR}/${EXEC_NAME}
)
