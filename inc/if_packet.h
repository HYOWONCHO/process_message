/****************************************************************************
 * Copyright (C) 2022 by BNDTek                                             *
 *                                                                          *
 * This file is packet structure  and packet processing                     *
 *                                                                          *
 * All right reserved.                                                      *
 * This software contains confidential information of BNDTEK.               *
 * And Unauthorized distribution of this software, or any portion of it are *
 * prohibited                                                               *
 *****************************************************************************/

/**
 * @file    if_packet.h
 * @author  BNDTEK
 * @date    12 April 2022
 * @brief   Implement the debugging message
 *
 */

#pragma once


#include <stdio.h>
#include <stdint.h>


#define PACKET_AID          0x3399

/** 
 * \defgroup Interface  Packet Processing 
 * @{
 */





/**
 * @brief Packet Main Body 
 */
typedef struct _t_if_pkt_main {
    uint16_t aid;       /**< Application Program Identifier */
    uint16_t len;       /**< Length of BODY packet */
    uint8_t body[1];    /**< Packet Body */
}if_pkt_main_t;


/**
 * @brief Union type to send the Response 
 */
typedef union _t_if_resp {
    struct {
        uint8_t sw1;            /**< Command Processing Status */
        uint8_t sw2;            /**< Command Processing Qualifier */
        uint16_t len;           /**< Length */
    }resp;
    uint32_t w;
}if_resp_t;

#define NP_RESP_NORMAL_prefix       (0x50 << 4)
#define NP_RESP_STATUS2_prefix      (0x60 << 4)
#define WP_RESP_prefix              (0x62 << 4)
#define ER_RESP_prefix              (0x63 << 4)
#define SE_RESP_prefix              (0x64 << 4)

#define NP_RESP_PC                  (NP_RESP_NORMAL_prefix | 0x00) /**< Process complete without Error or Warning */


/**
 * @brief Header of the Message Body packet
 */
typedef union _t_if_msghd {
    struct {
        uint16_t cla;       /**< Instruction Identifier */
        uint16_t type;      /**< Instruction Type */
        uint16_t param;     /**< Parameter for Instruction proceesing */
        uint16_t len;       /**< Incoming packet length */
    }msghd;

    uint16_t body[4];
}if_msghd_t;

/**
 * @brief Message body of Main Body
 */
typedef struct _t_if_msgbody {
    if_msghd_t header;
    uint8_t in[1];
}if_msgbody_t;



typedef struct _t_tlv {
    uint16_t tag;
    uint16_t len;
    void *value;
}tlv_t;



#define REQ_H2T_PREFIX                  (0xEB << 8)                 /**< REQUST FROM HOST TO TARGET */
#define REQ_H2T_SYSTEM_STATUS           (REQ_H2T_PREFIX | 0x01)     /**< Request the System Status */
#define REQ_H2T_CAM_SENSOR_INFO         (REQ_H2T_PREFIX | 0x02)     /**< Request the CAM Sensor information */
#define REQ_H2T_APP_UPDATE              (REQ_H2T_PREFIX | 0x03)     /**< Request the Application Update  */
#define REQ_H2T_OP_POLICY               (REQ_H2T_PREFIX | 0x04)     /**< Request the Operation Policy */
#define REQ_H2T_LATEST_DETECT           (REQ_H2T_PREFIX | 0x05)     /**< Request the Latest Decection Information */


#define REQ_T2H_PREFIX                  (0xEA << 8)                 /**< Request FROM TARGET TO HOST */
#define REQ_T2H_DETECTION               (REQ_T2H_PREFIX | 0x01)     /**< Notify the Person detection signal */
#define REQ_T2H_DWNINFO                 (REQ_T2H_PREFIX | 0x02)     /**< Notify the Download information


/** @}*/



