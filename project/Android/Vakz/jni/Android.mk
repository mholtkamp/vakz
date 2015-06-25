LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -Wno-write-strings
LOCAL_MODULE    := Vakz
FILE_LIST := $(wildcard E:/Projects/vakz/project/Windows/Engine/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:%=%)
#FILE_LIST := $(wildcard $(LOCAL_PATH)/[DIRECTORY]/*.cpp)
#LOCAL_SRC_FILES := Vakz.cpp ../src/*

include $(BUILD_STATIC_LIBRARY)
