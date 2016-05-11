LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog

LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := native.cpp

LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include\
${ANDROID_NDK}/platforms/android-21/arch-arm/usr/include/android

#  Enable C++11. However, pthread, rtti and exceptions aren’t enabled
LOCAL_CPPFLAGS += -std=c++11 -fexceptions

include $(BUILD_SHARED_LIBRARY)