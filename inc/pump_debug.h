
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

#include <string.h>
#include <errno.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __DBG_ENABLE__
#   ifdef __linux__
//#       define debug_printf(fmt,...) printf("(%s:%d): "fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#       define debug_printf(...) fprintf(stdout, ##__VA_ARGS__)
#       define n_print printf
#   else 
#       error "PLS, MUST not used in Bare-metal System or Windows"
#   endif
#else // DBG_ENABLE
#   define debug_printf(fmt,...)
#endif



#ifdef __cplusplus
}
#endif

