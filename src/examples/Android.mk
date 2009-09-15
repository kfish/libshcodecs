LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := external/libshcodecs/include
LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
LOCAL_SRC_FILES := shcodecs-decode-qvga-mpeg4.c
LOCAL_SHARED_LIBRARIES := libshcodecs
LOCAL_MODULE := shcodecs-decode-qvga-mpeg4 
include $(BUILD_EXECUTABLE)
