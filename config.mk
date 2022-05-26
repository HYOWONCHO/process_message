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
##################################################################################################


CROSS_ARCH ?= 
CROSS_PREFIX ?=  
CROSS_CC := ${CROSS_PRFIX}${CROSS_ARCH}gcc
USE_SAFECLIB ?= yes
USE_CURL ?= yes
USE_IPC ?= no

V=@


TARGET_BIN 	:= pumpmsg
TARGET_ROOT := $(shell pwd)

OBJDIR := build/obj
DEPDIR := build/dep


