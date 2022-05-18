
/****************************************************************************
 * Copyright (C) 2022 by BNDTek                                             *
 *                                                                          *
 *                                                                          *
 * All right reserved.                                                      *
 * This software contains confidential information of BNDTEK.               *
 * And Unauthorized distribution of this software, or any portion of it are *
 * prohibited                                                               *
 *****************************************************************************/

/**
 * @file    file_mgm.h
 * @author  BNDTEK
 * @date    28 April 2022
 * @brief   Implement the debugging message
 *
 */

#pragma once

#include "list_manager.h"
#include <stdbool.h>


/**< Recorded Stream Path */
#define RECORD_STREAM_DIR       ("/var/www/html/test")

/**< Length of the Record Name  */
#define RECORD_NAME_LEN         (0xFF)

typedef struct _t_record_file_h {
    char record_name[RECORD_NAME_LEN];
    bool is_first; 
    list_priv_t *list;
    list_element_t *pos;
}record_file_t;
