LOCAL_PATH:= $(call my-dir)

# shcodecs-dec
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := external/libshcodecs/include
LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
LOCAL_SRC_FILES := shcodecs-dec.c
LOCAL_SHARED_LIBRARIES := libshcodecs
LOCAL_MODULE := shcodecs-dec
include $(BUILD_EXECUTABLE)

# shcodecs-enc 
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := external/libshcodecs/include
LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
LOCAL_SRC_FILES := \
        shcodecs-enc.c \
        capture.c \
        veu_colorspace.c \
        ControlFileUtil.c \
        avcbeinputuser.c \
        avcbencsmp_common.c
LOCAL_SHARED_LIBRARIES := libshcodecs
LOCAL_MODULE := shcodecs-enc
include $(BUILD_EXECUTABLE)

# shcodecs-capenc 
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := external/libshcodecs/include
LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
LOCAL_SRC_FILES := \
        shcodecs-capenc.c \
        capture.c \
        veu_colorspace.c \
        ControlFileUtil.c \
        avcbeinputuser.c \
        avcbencsmp_common.c
LOCAL_SHARED_LIBRARIES := libshcodecs
LOCAL_MODULE := shcodecs-capenc
include $(BUILD_EXECUTABLE)

# shcodecs-cap 
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := external/libshcodecs/include
LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
LOCAL_SRC_FILES := capture.c shcodecs-cap.c
LOCAL_SHARED_LIBRARIES := libshcodecs
LOCAL_MODULE := shcodecs-cap
include $(BUILD_EXECUTABLE)

# FIXME: shcodecs-play cannot be compiled in Android because of aio_*()
# shcodecs-play
#include $(CLEAR_VARS)
#LOCAL_C_INCLUDES := external/libshcodecs/include
#LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 
#LOCAL_SRC_FILES := shcodecs-play.c veu_colorspace.c
#LOCAL_SHARED_LIBRARIES := libshcodecs
#LOCAL_MODULE := shcodecs-play
#include $(BUILD_EXECUTABLE)
