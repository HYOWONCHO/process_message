/****************************************************************************
 * Copyright (C) 2022 by BNDTek                                             *
 *                                                                          *
 * This file is Message Exchange of Processes.                              *
 *                                                                          *
 *   Processes is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Processes is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A Message ExchangeICULAR PURPOSE.       *
 *   See the GNU Lesser General Public License for more details.            *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Processes.                                          *
 *   If not, see <http://www.gnu.org/licenses/>.                            *
 *****************************************************************************/


/**
 * @file    pump_msg_defines.c
 * @author  BNDTEK
 * @date    12 April 2022
 * @brief   Define the Pumping App. data structrure
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>


typedef enum _m_pumperror {
    RET_INV_PARAM = -100,
    RET_INV_OBJ,
    RET_INV_AID,            /**< AID can not identify */
    RET_UNKNOWN_TYPE,
    RET_FAIL    = -1, 
    RET_TRUE    = 0,
}pumperror_m;

/**
 * @brief Message it-self DIAGNOSIS type define 
 */
typedef enum _m_DIAGNOSIS_define {
    DIAGNOSIS_NONE     = 0,     /**<  Message not verify */
    DIAGNOSIS_HASH256,          /**<  HASH-SHA-256 */
    DIAGNOSIS_HMAC256,          /**<  HMAC-SHA-256 */
    DIAGNOSIS_UNKNONW
}DIAGNOSIS_DEFINE_m;

/**
 * @brief Message DIAGNOSIS interface structure 
 */
typedef struct _t_DIAGNOSIS_if {
    uint16_t algo; /**< Message DIAGNOSIS algorithm  */
    uint8_t *msg;  /**< DIAGNOSIS Message data */
}DIAGNOSIS_if_t;

typedef struct _t_pump_msg{
    int type;
    int len;
    uint8_t diagnosis;
    void *msg;
}pump_msg_t;


