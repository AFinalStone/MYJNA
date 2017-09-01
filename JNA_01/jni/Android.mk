LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := test
LOCAL_SRC_FILES := basictypes.cpp data_packet.cpp packet_process_assist.cpp packet_process_function.cpp

LOCAL_LDLIBS += -lz
LOCAL_LDLIBS += -llog

#LOCAL_MODULE    := test
#LOCAL_SRC_FILES := test.c

include $(BUILD_SHARED_LIBRARY)