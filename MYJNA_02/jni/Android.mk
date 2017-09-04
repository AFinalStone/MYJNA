LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := check
LOCAL_SRC_FILES := check.cpp


include $(BUILD_SHARED_LIBRARY)