
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

#include "pump_msg_macro.h"
#include "list_manager.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "safe_mem_lib.h"


/**< Recorded Stream Path */
#define RECORD_STREAM_DIR       ("/var/www/html/test")

/**< Length of the Record Name  */
#define RECORD_NAME_LEN         (0xFF)


typedef struct _t_file_io {
    char f_name[RECORD_NAME_LEN];
    size_t fsize;
    int nw_mode;                /**< Network Mode ( 0 : Server , 1 : Client ) */
    int f_idfy;
    int f_mode;
    bool igonre;
    struct stat statbuf;

    FUNC(int, open, (void*, const char*));
    FUNC(int, read, (int, char*, size_t));
    FUNC(int, write, (int, const char*, size_t));
    FUNC(void, close, ());
    FUNC(int, seek, (int,off_t,int));
}file_io_t;

typedef struct _t_record_file_h {
    char record_name[RECORD_NAME_LEN];
    char f_name[RECORD_NAME_LEN];
    bool is_first; 
    list_priv_t *list;
    list_element_t *pos;

    file_io_t *fio;
}record_file_t;
