SET(PKG_NAME "dali")

SET(EXEC_NAME "tct-${PKG_NAME}-core")
SET(RPM_NAME "core-${PKG_NAME}-tests")

SET(CAPI_LIB "dali")

SET(TC_SOURCES
        utc-Dali-Actor.cpp
        utc-Dali-AlphaFunction.cpp
        utc-Dali-AngleAxis.cpp
        utc-Dali-Animation.cpp
        utc-Dali-Any.cpp
        utc-Dali-BaseHandle.cpp
        utc-Dali-BufferImage.cpp
        utc-Dali-CameraActor.cpp
        utc-Dali-Constraint.cpp
        utc-Dali-ConstraintFunction.cpp
        utc-Dali-Constraints.cpp
        utc-Dali-ConstraintSource.cpp
        utc-Dali-ConnectionTracker.cpp
        utc-Dali-CustomActor.cpp
        utc-Dali-Degree.cpp
        utc-Dali-EncodedBufferImage.cpp
        utc-Dali-FrameBufferImage.cpp
        utc-Dali-Gesture.cpp
        utc-Dali-GestureDetector.cpp
        utc-Dali-Handle.cpp
        utc-Dali-HoverProcessing.cpp
        utc-Dali-Image.cpp
        utc-Dali-ImageActor.cpp
        utc-Dali-IntrusivePtr.cpp
        utc-Dali-KeyEvent.cpp
        utc-Dali-Layer.cpp
        utc-Dali-LocklessBuffer.cpp
        utc-Dali-LongPressGesture.cpp
        utc-Dali-LongPressGestureDetector.cpp
        utc-Dali-MathUtils.cpp
        utc-Dali-Matrix.cpp
        utc-Dali-Matrix3.cpp
        utc-Dali-MeshMaterial.cpp
        utc-Dali-NativeImage.cpp
        utc-Dali-NinePatchImages.cpp
        utc-Dali-ObjectRegistry.cpp
        utc-Dali-PanGesture.cpp
        utc-Dali-PanGestureDetector.cpp
        utc-Dali-Path.cpp
        utc-Dali-PinchGesture.cpp
        utc-Dali-PinchGestureDetector.cpp
        utc-Dali-Pixel.cpp
        utc-Dali-PropertyMap.cpp
        utc-Dali-PropertyArray.cpp
        utc-Dali-PropertyNotification.cpp
        utc-Dali-PropertyTypes.cpp
        utc-Dali-PropertyValue.cpp
        utc-Dali-Quaternion.cpp
        utc-Dali-Radian.cpp
        utc-Dali-Random.cpp
        utc-Dali-Rect.cpp
        utc-Dali-RenderTask.cpp
        utc-Dali-RenderTaskList.cpp
        utc-Dali-ResourceImage.cpp
        utc-Dali-ShaderEffect.cpp
        utc-Dali-SignalTemplatesFunctors.cpp
        utc-Dali-SignalTemplates.cpp
        utc-Dali-Stage.cpp
        utc-Dali-TapGesture.cpp
        utc-Dali-TapGestureDetector.cpp
        utc-Dali-TouchEventCombiner.cpp
        utc-Dali-TouchProcessing.cpp
        utc-Dali-TypeRegistry.cpp
        utc-Dali-Uint16Pair.cpp
        utc-Dali-Vector.cpp
        utc-Dali-Vector2.cpp
        utc-Dali-Vector3.cpp
        utc-Dali-Vector4.cpp
        utc-Dali-WheelEvent.cpp
        utc-Dali-Constrainer.cpp
)

LIST(APPEND TC_SOURCES
        dali-test-suite-utils/mesh-builder.cpp
        dali-test-suite-utils/dali-test-suite-utils.cpp
        dali-test-suite-utils/test-harness.cpp
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
)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${${CAPI_LIB}_CFLAGS_OTHER} -O0 -ggdb --coverage -Wall -Werror=return-type")

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
