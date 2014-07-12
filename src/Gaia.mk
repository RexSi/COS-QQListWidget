#Used to record current mk file's location, its value usualy be calculated from function $(call my-dir)
#Like LOCAL_PATH := $(call my-dir). This variable need to be defined in top line in mk file.
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#Used to save the name of so file
#For example, LOCAL_MODULE := libHelloWorld means the final lib named libHelloworld.so, which will be packed into pkg file.
LOCAL_MODULE := libMainPage

#If you want to use resource in code, please set its value the same as the LOCAL_PACKAGE_NAME.
LOCAL_GAIA_R_PACKAGES := \
 ListWidgetGroup

#Used to save package's name, which will be the pkg file name. 
#LOCAL_PACKAGE_NAME := HelloWorld means final pkg file is HelloWorld.pkg.
LOCAL_PACKAGE_NAME := ListWidgetGroup

LOCAL_PRELINK_MODULE := false

#Used to save all the c/cpp source file, all the file use relative path compare to $(LOCAL_PATH)
LOCAL_SRC_FILES := \
	MainPage.cpp \
	ExpandableProvider.cpp \

#It used to store all the dependency lib that current module needs, all these libs are under $(SDK_ROOT)/lib.
#You just list the file name in LOCAL_SHARED_LIBRARIES.
LOCAL_SHARED_LIBRARIES := \
	libutils \
	libapi.$(strip $(SDK_VERSION))

#Used to store the head file search path, need absolute path.
LOCAL_C_INCLUDES += \
	$(COMMON_EXTERNAL_DIR)/stlport/stlport \

common_CFLAGS := -gstabs -O0 -g -c
LOCAL_CPPFLAGS += $(common_CFLAGS)

include $(BUILD_SHARED_LIBRARY)


#LOCAL_CPPFLAGS
#Used to add build flag for CPP compile

#LOCAL_CFLAGS
#Used to add build flag for both C & CPP compile

#LOCAL_LDFLAGS
#Used to add link flag for lib while linking

#LOCAL_STATIC_LIBRARIES
#Used to store the needed static lib while building module. it use absolute path.

#LOCAL_GROUP_STATIC_LIBRARIES
#Used to group all static lib when linking. LOCAL_GROUP_STATIC_LIBRARIES := true means group all static lib while linking.

#System function
#CLEAR_VARS 
#Used to clear all user defined variables except LOCAL_PATH
