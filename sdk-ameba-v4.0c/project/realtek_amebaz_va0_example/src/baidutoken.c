
#include <stdio.h>
#include <stdlib.h>
//#include <memory.h>
//#include <curl/curl.h> 
#include "baidutoken.h"
#include "wifi_conf.h"
#include <FreeRTOS.h>
#include <task.h>
#include <platform_stdlib.h>
#include <httpc/httpc.h>
#include "Dbmd_init.h"

const char API_TOKEN_URL[] = "http://openapi.baidu.com/oauth/2.0/token";

//const int MAX_TOKEN_SIZE = 100;

#define SERVER_HOST_API  "openapi.baidu.com"


RETURN_CODE
speech_get_token(const char *api_key, const char *secret_key, const char *scope, char *token) {
    //char url_pattern[] = "%s?grant_type=client_credentials&client_id=%s&client_secret=%s";
    char url[200];
	#if 0
    char *response = NULL;
   
    snprintf(url, 200, url_pattern, API_TOKEN_URL, api_key, secret_key);
    printf("url is: %s\n", url);
	
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url); // 注意返回值判读
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60); // 60s超时
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res_curl = curl_easy_perform(curl);
    RETURN_CODE res = RETURN_OK;
    if (res_curl != CURLE_OK) {
        snprintf(g_demo_error_msg, BUFFER_ERROR_SIZE, "perform curl error:%d, %s.\n", res,
                 curl_easy_strerror(res_curl));
        res = ERROR_TOKEN_CURL;
    } else {
        res = parse_token(response, scope, token); // 解析token，结果保存在token里
        if (res == RETURN_OK) {
            printf("token: %s of %s\n", token, response);
        }
    }
    free(response);
    curl_easy_cleanup(curl);
	#else

    RETURN_CODE res = RETURN_ERROR;
	struct httpc_conn *conn = NULL;
	uint8_t buf[1024];
    printf("\r\n\r\n\r\n>>>>>>>>>>>>>>>get baidu token<<<<<<<<<<<<<<<<<\r\n\r\n\r\n");
	
	#if 0// USE_HTTPS
	conn = httpc_conn_new(HTTPC_SECURE_TLS, NULL, NULL, NULL);
	#else
	conn = httpc_conn_new(HTTPC_SECURE_NONE, NULL, NULL, NULL);
	#endif
	if(conn) {
#if 0//USE_HTTPS
		if(httpc_conn_connect(conn, SERVER_HOST_API, 443, 0) == 0) {
#else
         
		if(httpc_conn_connect(conn, SERVER_HOST_API, 80, 0) == 0) {
#endif
			/* HTTP POST request */
		    
            sprintf(url,"/oauth/2.0/token?grant_type=client_credentials&client_id=%s&client_secret=%s", api_key, secret_key);
			// start a header and add Host (added automatically), Content-Type and Content-Length (added by input param)
			httpc_request_write_header_start(conn, "POST", url, NULL, strlen(url));
			// add other header fields if necessary

			
			//httpc_request_write_header(conn, "Connection", "close");
			httpc_request_write_header(conn, "Connection", "keep-alive");
			
			// finish and send header
			
			httpc_request_write_header_finish(conn);
			// send http body
			
			httpc_request_write_data(conn, url, strlen(url));
         
			// receive response header
			if(httpc_response_read_header(conn) == 0) {
				httpc_conn_dump_header(conn);

				// receive response body
				if(httpc_response_is_status(conn, "200 OK")) {
					
					int read_size = 0, total_size = 0;

					while(1) {
						memset(buf, 0, sizeof(buf));
						read_size = httpc_response_read_data(conn, buf, sizeof(buf) - 1);
                      
						if(read_size > 0) {
							total_size += read_size;
							if(buf>0) {
							  res = parse_token(buf, scope, token);
						        if (res == RETURN_OK) {
						            printf("token: %s of %s\n", token, buf);
									break;
						        }
							}	
						}
						else {
							break;
						}

						if(conn->response.content_len && (total_size >= conn->response.content_len))
							break;
					}
				}
				
			}
			
		}
		else {
			printf("\nERROR: httpc_conn_connect\n");
		}

		httpc_conn_close(conn);
		httpc_conn_free(conn);
		
		printf("\nexit speech_get_token\n");
	}
		
	#endif
    return res;
}


//notice : expires_in	token valid data
RETURN_CODE parse_token(const char *response, const char *scope, char *token) {
    
    RETURN_CODE res = obtain_json_str(response, "access_token", token, MAX_TOKEN_SIZE);
    if (res != RETURN_OK) {
       // snprintf(g_demo_error_msg, BUFFER_ERROR_SIZE, "parse token error: %s\n", response);
        return ERROR_TOKEN_PARSE_ACCESS_TOKEN;
    }

    // ==== 检查scope =========
    char scopes[300];
    res = obtain_json_str(response, "scope", scopes, 300);
    if (res != RETURN_OK) {
       // snprintf(g_demo_error_msg, BUFFER_ERROR_SIZE, "parse scope error: %s\n", response);
        return ERROR_TOKEN_PARSE_ACCESS_TOKEN;
    }
    char *scope_pos = strstr(scopes, scope);
    if (scope_pos == NULL) {
      //  snprintf(g_demo_error_msg, BUFFER_ERROR_SIZE, "scope： %s not exist in:%s \n", scope,
       //          response);
        return ERROR_TOKEN_PARSE_SCOPE;
    }
    return RETURN_OK;
}

RETURN_CODE obtain_json_str(const char *json, const char *key, char *value, int value_size) {
  //  int len = 4 + strlen(key) + 1;
   // char search_key[len];
   char search_key[50];
   int len=50;
    snprintf(search_key, len, "\"%s\":\"", key);
    char *start = strstr(json, search_key);
    if (start == NULL) {
        printf( "%s key not exist\n", key);
        return RETURN_ERROR;
    }

    start += strlen(search_key);
    char *end = strstr(start, "\"");
    
    int copy_size = (value_size < end - start) ? value_size : end - start;
    snprintf(value, copy_size + 1, "%s", start);
    return RETURN_OK;
}

RETURN_CODE get_json_result(const char *json, const char *key, unsigned char *value,int *copy_size) {
  //  int len = 4 + strlen(key) + 1;
   // char search_key[len];
   char search_key[50];
   int len=50;
   char *start=NULL,*end=NULL,*beijing=NULL;
 
   snprintf(search_key, len, "\"%s\":[\"", key);
   	
   start = strstr(json, search_key);
    if (start == NULL) { 
        printf( "%s key not exist\n", key);
       return RETURN_ERROR;
    }
	
	start += strlen(search_key);
	printf("start %s",start);

	beijing=strstr(start, "����");
	//if(beijing)
	//	 config_operate_led_status(1);
    end = strstr(start, "\"");
    printf("end %s\n",end);
    
  
    *copy_size = (MAX_RESULT_SIZE < end - start) ? MAX_RESULT_SIZE : end - start;
	printf("copysize %d\n",*copy_size);
    
    
     memcpy(value,start,*copy_size );

	 for(int i=0;i<end-start;i++)
	 	printf("%02x ",value[i]);
	 
	
    return RETURN_OK;
}
