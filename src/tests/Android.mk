LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := external/libshcodecs/include
LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
LOCAL_SRC_FILES := noop.c
LOCAL_SHARED_LIBRARIES := libshcodecs
LOCAL_MODULE := noop
include $(BUILD_EXECUTABLE)
