
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


/**< File I/O handling structure */
typedef struct _t_file_io {
    char f_name[RECORD_NAME_LEN];
    size_t fsize;
    int nw_mode;                /**< Network Mode ( 0 : Server , 1 : Client ) */
    int f_idfy;
    int f_mode;
    bool igonre;
    struct stat statbuf;

    /**
     * open - opens the file specified by pathname 
     *
     * @param[in]       void* - Record managing handle 
     * @param[in]       const char* - File path with name 
     *
     * @return  Return the new file descriptor, or -1 if an error occurred ( in which case, errno is set appropriately )
     */
    FUNC(int, open, (void*, const char*));

    /**
     * read - read from a file descriptor
     *
     * @param[in]       int - file discriptor 
     * @param[out]      char * - read buffer
     * @param[in]       size_t - read count
     *
     * @return  On success, the number of bytes read is returned. \n
     *          On Error, -1 is returned 
     */
    FUNC(int, read, (int, char*, size_t));

    /**
     * read - write to a file descriptor
     *
     * @param[in]       int - file discriptor 
     * @param[in]       char * - write buffer
     * @param[in]       size_t - write count
     *
     * @return  On success, the number of bytes read is returned. \n
     *          On Error, -1 is returned 
     */
    FUNC(int, write, (int, const char*, size_t));


    /**
     * close - close a file descriptor
     *
     */
    FUNC(void, close, ());

    /**
     * seek - reposition read/write file offset
     *
     * @param[in]       int - file descriptor
     * @param[in]       off_t - reposition offset
     * @param[in]       int - directive whence (SEEK_SET, SEEK_CUR, SEEK_END) 
     *
     * @return  Upon succeful completion, returns the resulting offset location. \n
     *          On error, the value(off_t) -1 is returned. 
     */
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


int file_mgm_init(record_file_t *p);
void file_mgm_deinit(record_file_t *p);
