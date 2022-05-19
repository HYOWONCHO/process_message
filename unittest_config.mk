do_test_memcmp?=y
do_test_server?=n
do_test_client?=n

do_test_ipcsvr?=no
do_test_ipccli?=no


# Use the intel safcelib 
ifeq ($(USE_SAFECLIB), yes)
vpath %.c $(TARGET_ROOT)/3rdparty/safeclib
INCLUDES += -I$(TARGET_ROOT)/3rdparty/include -I$(TARGET_ROOT)/3rdparty/safeclib
DEFINES += -D__SAFECLIB__
safeclib_SRCS := $(notdir $(wildcard $(TARGET_ROOT)/3rdparty/safeclib/*.c))
SRCS += $(strip $(safeclib_SRCS))
endif


# Proceed the unittest for Module function
ifeq ($(do_test_safestring), yes)
	vpath %.c $(TARGET_ROOT)/unittests/safestring
	INCLUDES += -I$(TARGET_ROOT)/unittests/safestring
# 	Memory Compare testing 
	ifeq ($(do_test_memcmp), y)
		DEFINES += -D_DO_TEST_MEMCMP_	
		SRCS += test_memcmp32_s.c list_test.c
	endif

endif

ifeq ($(do_test_server), yes)
	DEFINES+=-D_APP_SERVER_
	TARGET_BIN := $(TARGET_BIN)_server
endif

ifeq ($(do_test_client), yes)
	DEFINES+=-D_APP_CLIENT_
	TARGET_BIN := $(TARGET_BIN)_client
endif




ifeq ($(do_test_ipcsvr), yes)
	vpath %.c $(TARGET_ROOT)/unittests/safestring
	DEFINES += -D_DO_TEST_IPCSVR_
	SRCS += test_ipc_svr.c
	TARGET_BIN := $(TARGET_BIN)_ipcsvr
endif

ifeq ($(do_test_ipccli), yes)
	DEFINES += -D_DO_TEST_IPCCLI_
	TARGET_BIN := $(TARGET_BIN)_ipccli
endif
