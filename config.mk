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
do_test_safestring ?= yes


TARGET_BIN 	:= pumpmsg
TARGET_ROOT := $(shell pwd)

OBJDIR := build/obj
DEPDIR := build/dep


