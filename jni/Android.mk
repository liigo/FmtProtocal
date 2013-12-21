LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := fmt-native
LOCAL_SRC_FILES := com_tianxunnet_fmt_Fmt.c com_tianxunnet_fmt_FmtParser.c protocol/protocol.c protocol/buffmtparser.c protocol/lib/automem.c protocol/lib/linkhash.c protocol/lib/arraylist.c
include $(BUILD_SHARED_LIBRARY)
