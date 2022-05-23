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


# Default Source Path 
vpath %.c $(TARGET_ROOT)
vpath %.c $(TARGET_ROOT)/src


# Defafult Header Path
INCLUDES := -I$(TARGET_ROOT)/inc

LIBRARY := -lpthread

# Default Macro define
DEFINES := -D__DBG_ENABLE__ -DDEBUG


# Define Source 
SRCS := main.c 						\
		file_mgm.c list_manager.c 	\
		pump_socket_common.c 		\
		capture_thread.c xfer_thread.c


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


include ./unittest_config.mk



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














