# Android.mk.  Manually derived by running 'configure' using options
#     --disable-libhal --disable-libusb
# on Ubuntu 9.10 and capturing the actions taken by make.
#

# Copyright (C) 2010  Free Software Foundation, Inc.
# This Android.mk is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


LOCAL_PATH := $(call my-dir)

common_cflags := \
	-DANDROID \
	-DHAVE_CONFIG_H

common_c_includes := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/src/PCSC \
	$(LOCAL_PATH)/src

# ============ build libpcsclite.so ====================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/debug.c \
	src/error.c \
	src/winscard_clnt.c \
	src/simclist.c \
	src/strlcat.c \
	src/strlcpy.c \
	src/sys_unix.c \
	src/utils.c \
	src/winscard_msg.c

LOCAL_CFLAGS		:= $(common_cflags) \
	-DLIBPCSCLITE

LOCAL_C_INCLUDES	:= $(common_c_includes)
LOCAL_PRELINK_MODULE	:= false
LOCAL_MODULE		:= libpcsclite
LOCAL_MODULE_TAGS	:= optional
LOCAL_SHARED_LIBRARIES := liblog libcutils

include $(BUILD_SHARED_LIBRARY)


# ============ build libpcsclite static library ====================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/debug.c \
	src/error.c \
	src/winscard_clnt.c \
	src/simclist.c \
	src/strlcat.c \
	src/strlcpy.c \
	src/sys_unix.c \
	src/utils.c \
	src/winscard_msg.c

LOCAL_CFLAGS		:= $(common_cflags) \
	-DLIBPCSCLITE

LOCAL_C_INCLUDES	:= $(common_c_includes)
LOCAL_PRELINK_MODULE	:= false
LOCAL_MODULE		:= libpcsclite
LOCAL_MODULE_TAGS	:= optional

include $(BUILD_STATIC_LIBRARY)

# ============ build pcscd =============================================

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES += libdl

LOCAL_SRC_FILES := \
	src/atrhandler.c \
	src/configfile.c \
	src/debuglog.c \
	src/dyn_hpux.c \
	src/dyn_macosx.c \
	src/dyn_unix.c \
	src/eventhandler.c \
	src/hotplug_generic.c \
	src/hotplug_libhal.c \
	src/hotplug_libusb.c \
	src/hotplug_linux.c \
	src/hotplug_macosx.c \
	src/ifdwrapper.c \
	src/pcscdaemon.c \
	src/powermgt_generic.c \
	src/prothandler.c \
	src/readerfactory.c \
	src/simclist.c \
	src/strlcat.c \
	src/strlcpy.c \
	src/sys_unix.c \
	src/tokenparser.c \
	src/utils.c \
	src/winscard.c \
	src/winscard_msg.c \
	src/winscard_msg_srv.c \
	src/winscard_svc.c

LOCAL_C_INCLUDES	:= $(common_c_includes)
LOCAL_CFLAGS		:= $(common_cflags) \
	-DPCSCD \
	-DSIMCLIST_NO_DUMPRESTORE
LOCAL_MODULE		:= pcscd
LOCAL_MODULE_TAGS	:= optional

include $(BUILD_EXECUTABLE)

# ============ build testpcsc ==========================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES		:= src/testpcsc.c
LOCAL_C_INCLUDES	:= $(common_c_includes)
LOCAL_CFLAGS		:= $(common_cflags)
LOCAL_STATIC_LIBRARIES	:= libpcsclite
#LOCAL_MODULE_TAGS       := samples
LOCAL_MODULE		:= testpcsc
LOCAL_MODULE_TAGS	:= optional
LOCAL_SHARED_LIBRARIES := liblog libcutils

include $(BUILD_EXECUTABLE)

# ============ build testpcsc_sendapdu ==========================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES		:= src/testpcsc_sendapdu.c
LOCAL_C_INCLUDES	:= $(common_c_includes)
LOCAL_CFLAGS		:= $(common_cflags)
LOCAL_STATIC_LIBRARIES	:= libpcsclite
LOCAL_MODULE		:= testpcsc_sendapdu
LOCAL_MODULE_TAGS	:= optional

include $(BUILD_EXECUTABLE)


