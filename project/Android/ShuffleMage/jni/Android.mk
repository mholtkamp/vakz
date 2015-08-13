#LOCAL_PATH := $(call my-dir)
#
##include $(CLEAR_VARS)
##LOCAL_MODULE    := Vakz
##LOCAL_SRC_FILES := E:\Projects\vakz\project\Android\Vakz\obj\local\arm64-v8a\libVakz.a
##include $(PREBUILT_STATIC_LIBRARY)
#
#
##Build the library
#include $(CLEAR_VARS)
#LOCAL_CFLAGS := -Wno-write-strings
#LOCAL_MODULE    := Vakz
#FILE_LIST := $(wildcard ../../../Windows/Engine/*.cpp)
#LOCAL_SRC_FILES := $(FILE_LIST:%=%)
#include $(BUILD_STATIC_LIBRARY)
#
##Build the activity
#include $(CLEAR_VARS)
#LOCAL_MODULE    := Island
#LOCAL_SRC_FILES := Main.cpp
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Windows/Engine 
#LOCAL_LDLIBS    := -lm -llog -lGLESv3 -landroid
#LOCAL_STATIC_LIBRARIES := android_native_app_glue Vakz
#include $(BUILD_SHARED_LIBRARY)
#
#$(call import-module,android/native_app_glue)

# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
APP_CFLAGS += -Wno-error=format-security
LOCAL_MODULE    := Vakz
LOCAL_SRC_FILES := E:\Projects\vakz\project\Android\Vakz\obj\local\armeabi\libVakz.a
include $(PREBUILT_STATIC_LIBRARY)

#Build the activity
include $(CLEAR_VARS)
LOCAL_MODULE    := ShuffleMage
FILE_LIST_SM := $(wildcard E:/Projects/vakz/project/Windows/ShuffleMage/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST_SM:%=%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Windows/Engine 
LOCAL_LDLIBS    := -lm -llog -lGLESv3 -landroid -lEGL
LOCAL_STATIC_LIBRARIES := android_native_app_glue Vakz
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
