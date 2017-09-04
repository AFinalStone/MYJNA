LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sayhello
LOCAL_SRC_FILES := sayhello.c


include $(BUILD_SHARED_LIBRARY)