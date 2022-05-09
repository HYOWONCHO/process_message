#pragma once

#include <curl/curl.h>

 /**
 * curl_getinfo - Use getinfo to get content-type after completed transfer
 *
 * @parm [in] url       Speicifed the Remote address
 * @return On success
 */
int curl_getinfo(const char *url);


/**
 * curl_url2file - Download a given URL into a local 
 * 
 * @param [in] url      Specified the remote address
 * @param [in] path     Download file name with path
 *
 * @return 
 */
int curl_url2file(const char *url, const char *path);

