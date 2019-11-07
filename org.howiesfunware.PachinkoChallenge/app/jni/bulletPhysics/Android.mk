LOCAL_PATH := $(call my-dir)

###########################
#
# AngelScript shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := bulletPhysics

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
        $(wildcard $(LOCAL_PATH)/src/BulletSoftBody/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletDynamics/Character/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletDynamics/ConstraintSolver/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletDynamics/Dynamics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletDynamics/Vehicle/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletDynamics/Featherstone/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletDynamics/MLCPSolvers/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletCollision/BroadphaseCollision/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletCollision/CollisionDispatch/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletCollision/CollisionShapes/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletCollision/Gimpact/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/BulletCollision/NarrowPhaseCollision/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/LinearMath/*.cpp))

include $(BUILD_SHARED_LIBRARY)
