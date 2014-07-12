#Used to record current mk file's location, its value usualy be calculated from function $(call my-dir).
#Like LOCAL_PATH := $(call my-dir). This variable need to be defined in top line in mk file.
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#Used to save package's name, which will be the pkg file name. 
#LOCAL_PACKAGE_NAME := HelloWorld means final pkg file is HelloWorld.pkg.
LOCAL_PACKAGE_NAME := ListWidgetGroup

#Used to indicate the so that will be in the final pkg file. 
#So file is the excution part of the pkg,current only one so is support in pkg. 
#LOCAL_GAIA_SHARED_LIBRARIES := libHelloWorld means the final pkg will contain libHelloWorld.so in lib sub folder. 
#Usually, LOCAL_GAIA_SHARED_LIBRARIES's value is equal to the value of LOCAL_MODULE in src/Gaia.mk.
LOCAL_GAIA_SHARED_LIBRARIES := libMainPage


#Used to save the certificate file name which is used to sign pkg. All the avaliable certificate is located in $(SDK_ROOT)/build/security.  
#LOCAL_CERTIFICATE := platform means pkg is signed by platform.x509.pem and platform.pk8.
LOCAL_CERTIFICATE := platform

include $(BUILD_GAIA_PACKAGE)

include $(LOCAL_PATH)/src/Gaia.mk
