/****************************************************************************
 * Copyright (C) 2022 by BNDTek                                             *
 *                                                                          *
 * This file is the Program to Debugging.                                   *
 *                                                                          *
 * All right reserved.                                                      *
 * This software contains confidential information of BNDTEK.               *
 * And Unauthorized distribution of this software, or any portion of it are *
 * prohibited                                                               *
 *****************************************************************************/

/**
 * @file    pump_debug.c
 * @author  BNDTEK
 * @date    12 April 2022
 * @brief   Implement the debugging message
 *
 */

#pragma once

#include "pump_debug.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MEM_RELEASE(x)                              \
    ({                                              \
        if(x != NULL) {                             \
            free(x);                                \
            x = NULL;                               \
        }                                           \
    })

#define FUNC(_type, _name, _param) \
    _type (*_name) _param


#define SYS_ERROR_MSG()		strerror(errno)


//!< \def RETURN_IF_FAIL If given expression is fail, function is terminate and return.
#define RETURN_IF_FAIL(expr)	\
	do { if (!(expr)) { n_print("'%s' FAILED.", #expr); return; }} while(0)


//!< \def RETURN_IF_FAIL If given expression is fail, function is terminate and return.
#define RETURN_VAL_IF_FAIL(expr,val)	\
	do { if (!(expr)) { n_print("'%s' FAILED.", #expr); return(val); }} while(0)


#define __BUF_HEX_PRINT(P,T,L) \
	({	\
	 	n_print("\r\n[HEX]");	\
	 	if((void *)T != NULL) n_print("%-16.16s(%04d): ", (char *)T, (int)L);	\
	 	\
	 	n_print("\r\n%-10s", "Offset(h)");	\
	 	for(int i = 0; i < 0x10; i++) n_print("%02x ",i);	\
	 	\
	 	for(int i = 0; i < (int)L; i++)	{	\
	 		if (i%0x10 == 0) n_print("\r\n%08X  ", i);	\
	 		n_print("%02x ", ((char *)P)[i] & 0xFF);	\
	 	}\
	 	\
	 	n_print("\r\n");	\
	 })

#ifdef __cplusplus
}
#endif

