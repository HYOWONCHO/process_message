##################################################################################################
#
#
#
#
#
#
#
#
#
#
#
#
#
#
##################################################################################################


# Include 
include ./config.mk
include ./unittest_config.mk






# Default Source Path 
vpath %.c $(TARGET_ROOT)
vpath %.c $(TARGET_ROOT)/src


# Defafult Header Path
INCLUDES := -I$(TARGET_ROOT)/inc

LIBRARY :=

# Default Macro define
DEFINES := -D__DBG_ENABLE__


# Define Source 
SRCS := main.c 						\
		file_mgm.c list_manager.c 	\
		pump_socket_common.c 		\


ifeq ($(USE_CURL), yes)
	SRCS += curl_usage_api.c
	LIBRARY += -lcurl
	DEFINES += -D__USED_CURL__ 
endif

ifeq ($(USE_IPC), yes) 
	SRCS += ipc_com.c
	LIBRARY += -lrt
	DEFINES += -D__USED_IPC__

endif


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



SRCS := $(strip $(SRCS))
INCLUDES := $(strip $(INCLUDES))
DEFINES := $(strip $(DEFINES))

OBJS 	:= $(strip $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS)))
DEPS 	:= $(strip $(patsubst %.c,$(OBJDIR)/%.dep,$(SRCS)))


C_FLAGS := $(INCLUDES)  $(LIBRARY)
LD_FLAGS := $(LIBRARY)

.SUFFIXES: .o .c
$(OBJDIR)/%.o : %.c 
	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi
	@echo "C Complie : $<"
	$(V)$(CROSS_CC) $(DEFINES) -c $< -o $@  $(C_FLAGS)
	
#.SUFFIXES: .o .dep
#$(DEPDIR)/%.o : %.dep 
#	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi
#	@echo "C Complie : $<"
#	$(V)$(CROSS_CC) $(C_FLAGS) $(DEFINES) -M $< > $@
	



all:$(OBJS)
	$(CROSS_CC) -o build/$(TARGET_BIN)  $(OBJS) $(LD_FLAGS)

dep: $(DEPS)

clean:
	-rm -rf $(OBJDIR) $(DEPDIR)
	-rm -rf $(TARGET_BIN)

print_path:
	@echo "TARGET_ROOT path : $(TARGET_ROOT)"
	@echo "SRCS : $(SRCS)"
	@echo "OBJS : $(OBJS)"
	@echo "OBJDIR : $(OBJDIR)"
ifeq ($(USE_SAFECLIB), yes)
endif














