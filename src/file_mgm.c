

#include "file_mgm.h"


static const char const *prefix_table[] = {
    "jpeg","png","jpg", NULL
};

static int fio_open(void *priv, const char *path) 
{
    int ret = EFAIL;
    int is_exist = 0;
    size_t flen = 0;
    file_io_t *p = NULL;


    p = (file_io_t *)ptr_member_of_container(priv, record_file_t,  fio);
    debug_printf("checking : %p , %p", p, ((record_file_t *)priv)->fio);

    if( p == NULL || path == NULL ) { 
        ret = ESNULLP;
        goto err_done;
    }


    flen = strnlen_s((const char *)path, RECORD_NAME_LEN);

    strcpy_s(p->f_name, flen+1, path);

    is_exist = access(p->f_name, F_OK);
    if(is_exist != 0) {
        info_printf("%s file is not exist on your system (%s)", p->f_name, SYS_ERROR_MSG());
    }

    // App running type check
#if defined(_APP_SERVER_)

    p->f_mode = O_CREAT | O_RDWR | O_DSYNC;
    p->f_idfy = open((const char *)p->f_name , p->f_mode, S_IRWXU);
#elif defined(_APP_CLIENT_)
    if(stat((const char *)p->f_name, &p->statbuf) < 0) {
        err_printf("Failed the file stat check, (%s)",SYS_ERROR_MSG());
        ret = errno;
        goto err_done; 
    }

    p->f_mode = O_RDONLY;
    p->f_idfy = open((const char *)p->f_name, p->f_mode);
    p->fsize = p->statbuf.st_size;
    
#else
#error "Please, MUST select regards to Application running mode"
#endif

    if( p->f_idfy < 0 ) {
        ret = errno;
        err_printf("%s open fail (%s)", p->f_name, SYS_ERROR_MSG());
        goto err_done;
    }


    return EOK;
err_done:

    return ret;
}

static int fio_write(int fd, const char *buf, size_t wrsz)
{
    int ret = EOK; 
    

    if( buf == NULL) {
        err_printf("Invalird parameter ");
        ret = EFAIL;
        errno = ESNULLP;
        goto err_done;
    }

    ret =  write(fd, buf,  wrsz);
    if(ret < 0) {
        err_printf("Wrong write");
        ret = EFAIL;
        goto err_done;
    }

err_done:
    RETURN_VAL(ret);
}

static int fio_read(int fd, char *buf, size_t rdsz)
{
    int ret = EOK; 
    

    if( buf == NULL) {
        err_printf("Invalird parameter ");
        ret = EFAIL;
        errno = ESNULLP;
        goto err_done;
    }

    ret =  read(fd, buf,  rdsz);
    if(ret < 0) {
        err_printf("Wrong read (%s)", SYS_ERROR_MSG());
    }

err_done:
    RETURN_VAL(ret);

}

static void fio_close(int fd)
{
    close(fd);
}

static int fio_seek(int fd, off_t offset, int whence) 
{
    int ret = EOK;

    if((ret = lseek(fd,offset,whence)) < 0) {
        ret = EFAIL;
        err_printf("Failed file seeking (%s)", SYS_ERROR_MSG());
        goto err_done;
    }
   
err_done:
    return ret; 

}

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

        p->fio = calloc(1,sizeof p->fio);
        if(p->fio == NULL) {
            err_printf("Not enough space (%s)",SYS_ERROR_MSG());
            return EFAIL;
        }

        p->fio->open = fio_open;
        p->fio->close = fio_close;
        p->fio->write = fio_write;
        p->fio->read = fio_read;
        p->fio->seek = fio_seek;

    }
    return EOK;

}

void file_mgm_deinit(record_file_t *p) {
    if(p->list) {
        free(p->list);
        p->list=NULL;
    }

    if(p->fio) {
        free(p->fio);
        p->fio = NULL;
    }
}
