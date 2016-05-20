LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog

LOCAL_MODULE    := protocol

# LOCAL_SRC_FILES := native.cpp
PROJECT_FILES := $(wildcard $(LOCAL_PATH)/protocol/*.cpp)
PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES := $(PROJECT_FILES)


LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include\
${ANDROID_NDK}/platforms/android-21/arch-arm/usr/include/android

#  Enable C++11. However, pthread, rtti and exceptions aren’t enabled
LOCAL_CPPFLAGS += -std=c++11 -fexceptions

include $(BUILD_SHARED_LIBRARY)