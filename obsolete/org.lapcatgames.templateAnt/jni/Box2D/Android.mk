LOCAL_PATH := $(call my-dir)

###########################
#
# Box2D shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := Box2D

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/Box2D/Collision/*.cpp) \
        $(wildcard $(LOCAL_PATH)/Box2D/Collision/Shapes/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Box2D/Common/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Box2D/Dynamics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Box2D/Dynamics/Contacts/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Box2D/Dynamics/Joints/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Box2D/Rope/*.cpp))

include $(BUILD_SHARED_LIBRARY)

