#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

#include "file_mgm.h"
#include "pump_debug.h"
#include "safe_mem_lib.h"


static char *prefix_table[] = {
    "jpeg","png","jpg", NULL
};

static int _is_image_file_format(char *path)
{
    int ret = -1;
    char *p2tok;
    char *p2str;
    char delimiter = '.';
    rsize_t len;

    char check_name[255] = {0, };

    if( path == NULL )  {
        errno = EFAULT;
        err_printf("Input PARAM Invalid ( %s )", strerror(errno));
        goto done;
    }

    len = strlen(path);
    if(strcpy_s(check_name, sizeof check_name, path) != EOK) {
        err_printf("check file path copy fail");
    }

    check_name[len] = '\0';
    p2tok = strtok_s(check_name, &len, &delimiter, &p2str);
    if(p2tok == NULL) {
        err_printf(" Delimiter \'%c\' can not found in %s", delimiter, check_name);
        goto done;
    }

    //info_printf("Token -%s- Str -%s- len = %ld", p2tok, p2str, len);

    int sz_table = sizeof prefix_table / sizeof *prefix_table;
    int x = 0L;
#if 0
    if(len < 3) {
        //err_printf("%s not image file !!!", path); 
        goto done;
    }
#endif
    while(prefix_table[x] != NULL) {
        if(strncmp((const char *)p2str, (const char *)prefix_table[x], len) == 0) {
            break;
        }
        x++;
    }

    if(x == sz_table - 1) {
        //err_printf("%s not image file !!!", path); 
        goto done;
    }

    //debug_printf("last path: %s", path);
    ret = 0L;
done:

    return ret;
}


/**
 * record_steam_list: Searching the file name
 *
 * @param p     output buffer for file list
 * @param size  size for one element for output buffer 
 *
 * @return      On success, return number of file in directory, \n
 *              otherwise, return the -1
 */

int record_stream_list(record_file_t *p, int size)
{
    int ret = 0L;
    DIR *dirp;
    struct dirent *rd_dir;
    struct stat statbuf;

    char check_file[512] = {0, };

    if(p == NULL) {
        err_printf("memory allocation %s", strerror(errno));
        return -1;
    }

    if((dirp = opendir(RECORD_STREAM_DIR)) == NULL) {
        err_printf("opendir fail (%s)", strerror(errno));
        return -1;
    }

    // directory read
    while((rd_dir = readdir(dirp)) != NULL) {
        //info_printf("Read Dir: %s", rd_dir->d_name);

        sprintf(check_file, "%s/%s", RECORD_STREAM_DIR, rd_dir->d_name);


        if(stat((const char *)&check_file[0], &statbuf) < 0) {
            info_printf("File information check fail (%s)", strerror(errno));
            memset_s((void *)&check_file[0], sizeof check_file, 0x00);
            continue;
        }

        switch(statbuf.st_mode & S_IFMT) {
            case S_IFDIR:
                //info_printf("%s is a directory", check_file);
                break;
            case S_IFREG:
                //info_printf("%s is a regular file", check_file);
                //    info_printf("stored path: %s", rd_dir->d_name);
                if(!_is_image_file_format(rd_dir->d_name)) {
                    //TODO: Assign the file name on "p" buffer
                    ret++;
                    //strcpy_s();
//                    info_printf("stored path: %s", rd_dir->d_name);
                    list_insert_next(p->list, NULL, rd_dir->d_name);
                }
                break;
            default:
                info_printf("%s is Unknown", check_file);
                break;
        }

        memset_s((void *)&check_file[0], sizeof check_file, 0x00);
    }

    return ret;

}

void file_mgm_list_destory(void *priv)
{
    //TODO : Add the operation when Linked list destroy is called.

    info_printf("Called the Destroy callback regards to Linked Lit");

}


int file_mgm_init(record_file_t *p)
{

    if(p->is_first == false) {
        memzero_s((void *)p, sizeof *p);
        p->list = (list_priv_t *)calloc(1, sizeof(record_file_t));
        if(p->list == NULL) {
            err_printf("failed memory allocation (%s)", strerror(errno));
            return -1;
        }

        list_initialize(p->list, file_mgm_list_destory);
        p->is_first = true;
    }
    return 0;

}
