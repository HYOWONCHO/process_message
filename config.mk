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


all: 

check_tools:
	-@tool_path=''; 	\
		tool_path=$(which $CROSS_CC); 	\
		echo "${tool_path} | ${CROSS_CC}" 



