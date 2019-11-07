LOCAL_PATH := $(call my-dir)

###########################
#
# AngelScript shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := angelscript

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
        $(wildcard $(LOCAL_PATH)/source/*.S) \
	$(wildcard $(LOCAL_PATH)/source/*.cpp) \
	$(wildcard $(LOCAL_PATH)/add_on/scriptstdstring/*.cpp) \
	$(wildcard $(LOCAL_PATH)/add_on/scriptdictionary/*.cpp) \
	$(wildcard $(LOCAL_PATH)/add_on/scriptmath/*.cpp) \
	$(wildcard $(LOCAL_PATH)/add_on/scriptarray/*.cpp))

LOCAL_CPP_FEATURES += rtti exceptions
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)
