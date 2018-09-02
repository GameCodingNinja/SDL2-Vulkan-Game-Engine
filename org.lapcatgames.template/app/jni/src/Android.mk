LOCAL_PATH := $(call my-dir)

###########################
#
# template shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
ASCRIPT_PATH := $(LOCAL_PATH)/../angelscript

LOCAL_C_INCLUDES := \
	$(SDL_PATH)/include \
	$(LOCAL_PATH)/library \
	$(LOCAL_PATH)/../boost \
	$(ASCRIPT_PATH)/add_on

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_CXXFLAGS := -std=gnu++14 -fexceptions

LOCAL_SRC_FILES := \
    $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/source/game/*.cpp) \
    $(wildcard $(LOCAL_PATH)/source/smartGUI/*.cpp) \
    $(wildcard $(LOCAL_PATH)/source/state/*.cpp) \
    $(wildcard $(LOCAL_PATH)/source/ai/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/gui/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/objectdata/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/system/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/managers/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/strategy/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/physics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/sprite/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/2d/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/3d/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/common/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/utilities/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/slot/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/soil/*.c) \
	$(wildcard $(LOCAL_PATH)/library/script/*.cpp))

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer bulletPhysics Box2D angelscript

LOCAL_LDLIBS := -ldl -llog -landroid

include $(BUILD_SHARED_LIBRARY)

