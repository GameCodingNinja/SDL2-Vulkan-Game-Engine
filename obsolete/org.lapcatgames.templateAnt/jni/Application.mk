
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static c++_static gnustl_static
# APP_ABI := arm64-v8a armeabi armeabi-v7a x86 x86_64 mips mips64 or all

APP_STL := gnustl_static

#APP_ABI := all
#APP_ABI := armeabi armeabi-v7a x86
APP_ABI := armeabi

# Needed for debugging
APP_PLATFORM := android-10
