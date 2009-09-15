LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	external/libshcodecs/include \

LOCAL_CFLAGS := -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B 

LOCAL_SRC_FILES := \
        m4driverif.c \
        shcodecs_decoder.c \
        shcodecs_encoder.c \
        encoder_common.c \
        general_accessors.c \
        h264_accessors.c \
        mpeg4_accessors.c \
        property_accessors.c \
        h264_encode.c \
        mpeg4_encode.c \
        QuantMatrix.c

LOCAL_SHARED_LIBRARIES := libstdc++ libm4dec libm

LOCAL_MODULE := libshcodecs
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)

