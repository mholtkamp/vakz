LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -Wno-write-strings -Wno-error=format-security
LOCAL_MODULE    := Vakz
FILE_LIST := $(wildcard E:/Projects/vakz/project/Windows/Engine/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:%=%)
#FILE_LIST := $(wildcard $(LOCAL_PATH)/[DIRECTORY]/*.cpp)
#LOCAL_SRC_FILES := Vakz.cpp ../src/*
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)