#include "curl_usage_api.h"
#include "pump_msg_macro.h"

#include <unistd.h>

#define HTTP_CON_NOTFOUND       404
#define HTTP_CON_OK             200 

int curl_getinfo(const char *url)
{
    int ret = -1;
    CURL *curl;
    CURLcode res;
    long respcode = 0;

    curl = curl_easy_init();
    if(curl == NULL) {
        goto done;
    }

    curl_easy_setopt(curl,CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        goto done;
    }

    // Ask for the content-type
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&respcode);
    if(respcode != HTTP_CON_OK) {
        goto done;
    }

    ret = 0L;

done:
    return ret;

}


static size_t curl_url2file_write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

int curl_url2file(const char *url, const char *path)
{
    int ret = -1;
    CURL *curl_handle = NULL;
    FILE *pagefile = NULL;
    //CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    // Init the curl session
    curl_handle = curl_easy_init();
    if(curl_handle == NULL) {
        err_printf("Failed init curl session ( %s )", strerror(errno));
        goto done;
    }

    // set URL to get here
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    // switch on full protocol/debug output while testing
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    // disable progress meter, set to 0L ot enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    // send all data to this function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_url2file_write_data);

    
    pagefile = fopen(path, "a+");
    if(pagefile == NULL) {
        err_printf("Failed the fopen() - %s", strerror(errno));
        goto done;
    }

    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
    if(curl_easy_perform(curl_handle) != CURLE_OK) {
        err_printf("Failed the curl perform - %s", strerror(errno));
        goto done;
    }


    ret = 0L;


done:

    if(pagefile) {
        fclose(pagefile);
    }

    if(curl_handle) {
        curl_easy_cleanup(curl_handle);
    }


    curl_global_cleanup();
    return ret;
    

}
