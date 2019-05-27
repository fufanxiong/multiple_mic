//
// Created by fu on 3/2/18.
//
#include <stdlib.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "wifi_conf.h"
#include "flash_api.h"
#include "device_lock.h"

#include <FreeRTOS.h>
#include <task.h>
#include <platform_stdlib.h>
#include <httpc/httpc.h>


//#include <curl/curl.h>
//#include <memory.h>
//#include "common.h"
#include "ttsmain.h"
//#include "token.h"
//#include "ttscurl.h"

#include "asrmain.h"

#include "queue.h"


const char TTS_SCOPE[] = "audio_tts_post";
const char API_TTS_URL[] = "http://tsn.baidu.com/text2audio"; // 可改为https
const char API_TTS_PATH[] = "/text2audio";
const char API_TTS_SERVER[] = "tsn.baidu.com";

char text[512];

extern char token[MAX_TOKEN_SIZE];

QueueHandle_t	recv_tts_queue;

extern int parse_mp3_frame(char *aux_buf,int aux_len);

RETURN_CODE fill_config_tts(struct tts_config *config) {
    // 填写网页上申请的appkey 如 g_api_key="g8eBUMSokVB1BHGmgxxxxxx"
    //char api_key[] = "4E1BG9lTnlSeIf1NQFlrSq6h";
    char api_key[] = "AIf8O70PGaQeoDAo1MQPbch3";
    // 填写网页上申请的APP SECRET 如 $secretKey="94dc99566550d87f8fa8ece112xxxxx"
    //char secret_key[] = "544ca4657ba8002e3dea3ac2f5fdd241";
    char secret_key[] = "TNqGwiaxkc1tmlwmt2MBxQqH71zahm2w";

    // text 的内容为"欢迎使用百度语音合成"的urlencode,utf-8 编码
    // 可以百度搜索"urlencode"
    //char text[] = "欢迎使用百度语音";

    // 发音人选择, 0为普通女声，1为普通男生，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
    int per = 0;
    // 语速，取值0-9，默认为5中语速
    int spd = 5;
    // #音调，取值0-9，默认为5中语调
    int pit = 5;
    // #音量，取值0-9，默认为5中音量
    int vol = 5;
    // 下载的文件格式, 3：mp3(default) 4： pcm-16k 5： pcm-8k 6. wav
	int aue = 3;
	
    // 将上述参数填入config中
    snprintf(config->api_key, sizeof(config->api_key), "%s", api_key);
    snprintf(config->secret_key, sizeof(config->secret_key), "%s", secret_key);
    snprintf(config->text, sizeof(text), "%s", text);
    config->text_len = sizeof(text) - 1;
    snprintf(config->cuid, sizeof(config->cuid), "1234567C");
    config->per = per;
    config->spd = spd;
    config->pit = pit;
    config->vol = vol;
	config->aue = aue;
	
	// aue对应的格式，format
	const char formats[4][4] = {"mp3", "pcm", "pcm", "wav"};
	snprintf(config->format, sizeof(config->format), formats[aue - 3]);
	
    return RETURN_OK;
}

#if 0
int main() {
    curl_global_init(CURL_GLOBAL_ALL);
    RETURN_CODE rescode = run();
    curl_global_cleanup();
    if (rescode != RETURN_OK) {
        fprintf(stderr, "ERROR: %s, %d", g_demo_error_msg, rescode);
    }
    return rescode;
}
#endif

RETURN_CODE run_start_tts() {
    struct tts_config config;
    //char token[MAX_TOKEN_SIZE];

    RETURN_CODE res = fill_config_tts(&config);
    if (res == RETURN_OK) {
        // 获取token
        //res = speech_get_token(config.api_key, config.secret_key, TTS_SCOPE, token);
       // if (res == RETURN_OK) {
            // 调用识别接口
            run_tts(&config, token);
      //  }
    }

    return RETURN_OK;
}

RETURN_CODE run_tts(struct tts_config *config, const char *token)
{
	
		char url[512];
		//char tmp_data[256];
		struct httpc_conn *conn = NULL;
		//uint8_t buf[1024];
		unsigned char *buf = NULL;
		unsigned char result[MAX_RESULT_SIZE]={0};
		int size;
		
		RETURN_CODE res = RETURN_OK;

		buf = (char *)malloc(1024);
		if(buf == NULL)
		{
			printf("can not malloc buf\n");
			res = RETURN_ERROR;
			return res;
		}
		//snprintf(url, sizeof(url), "/server_api?cuid=%s&token=%s&dev_pid=%d",
		//		 config->cuid, token, config->dev_pid);
		//memset(tmp_data,0,256);
		char params_pattern[] = "ctp=1&lan=zh&cuid=%s&tok=%s&tex=%s&per=%d&spd=%d&pit=%d&vol=%d&aue=%d";
		snprintf(url,sizeof(url),params_pattern,config->cuid,token,config->text,
					config->per,config->spd,config->pit,config->vol,config->aue);

		//snprintf(url,sizeof(url),"%s?%s",API_TTS_URL,tmp_data);
		printf("request url :%s\n", url);
	
	#if 0//USE_HTTPS
		conn = httpc_conn_new(HTTPC_SECURE_TLS, NULL, NULL, NULL);
    #else
		conn = httpc_conn_new(HTTPC_SECURE_NONE, NULL, NULL, NULL);
    #endif
	
		if(conn) {
	
		#if 0//USE_HTTPS
			if(httpc_conn_connect(conn, API_ASR_SERVER, 443, 0) == 0) {
		#else
			 
			if(httpc_conn_connect(conn, API_TTS_SERVER, 80, 0) == 0) {
		#endif
	
			  // start a header and add Host (added automatically), Content-Type and Content-Length (added by input param)
				httpc_request_write_header_start(conn, "POST", API_TTS_PATH, NULL, strlen(url));	//CONTENT_LEN IS ? 
			 
				httpc_request_write_header(conn, "Connection", "keep-alive");
				
				 char header[50];
				 snprintf(header, sizeof(header), "audio/%s", config->format); 
				 printf("header :%s\n",header);
				 httpc_request_write_header(conn, "Content-Type", header);
			   
	
				// finish and send header
				
				httpc_request_write_header_finish(conn);
				// send http body
				//Upload_file(conn);
				httpc_request_write_data(conn, url, strlen(url));
				// receive response header
				if(httpc_response_read_header(conn) == 0) {
					httpc_conn_dump_header(conn);
	
					// receive response body
					if(httpc_response_is_status(conn, "200 OK")) {
					   
							
							int read_size = 0, total_size = 0;
	
							//todo 
							//body a queue to receive
							//judge server is get right info
							//get content_type whether we set
							const char formats[4][4] = {"mp3", "pcm", "pcm", "wav"};
							char *audio_format = NULL;
							int i;
							for(i = 0; i < 4; i++)
							{
								if(NULL != (audio_format = strstr(conn->response.content_type,formats[i])))
								{
									printf("get format\n");
									break;
								}
							}

							size = conn->response.content_len;
							
							if(audio_format != NULL)
							{
								//we need save which format for decode
								HTTP_RECV_INFO  tmp_entity;
								LPHTTP_RECV_INFO tmp_info = &tmp_entity;
								printf("start read audio data\n");
								memset(buf, 0, sizeof(buf));
								do
								{
									//memset(buf, 0, sizeof(buf));
									read_size = httpc_response_read_data(conn, buf, 1024);
									//printf("read data size=%d\n",read_size);
									if(read_size <= 0)
										break;
									size -= read_size;
									tmp_info->recv_addr = buf;
									tmp_info->len = read_size;
									printf("read data size=%d,data addr=%p\n",tmp_info->len,tmp_info->recv_addr);
									xQueueSend(recv_tts_queue,(void *)&tmp_info,1000/portTICK_RATE_MS);
								}while(size > 0);
							}
#if 0							
							if(read_size > 0) {
								//total_size += read_size;
								
								if(buf>0) dbg_printf(":%s", buf);
								//	====  获取 result字段 =========
							   
							  get_json_result(buf, "result",result, &size);
							   
								printf("\n");
								for(int i=0;i<size;i++)
								printf("%02x ",result[i]);
								 
								printf("/%s/",result);
							}
#endif
					}
					else
						res = ERROR_ASR_CURL;
	
				}
			}
		 httpc_conn_close(conn);
		 httpc_conn_free(conn);
		 printf("exit\n");
		}

	
	if(buf != NULL)
	{
		free(buf);
		buf = NULL;
	}
	return res;

}


#if 0
// 调用识别接口
RETURN_CODE run_tts(struct tts_config *config, const char *token) {
    char params[200 + config->text_len * 9];
    CURL *curl = curl_easy_init(); // 需要释放
    char *cuid = curl_easy_escape(curl, config->cuid, strlen(config->cuid)); // 需要释放
    char *textemp = curl_easy_escape(curl, config->text, config->text_len); // 需要释放
	char *tex = curl_easy_escape(curl, textemp, strlen(textemp)); // 需要释放
	curl_free(textemp);
	char params_pattern[] = "ctp=1&lan=zh&cuid=%s&tok=%s&tex=%s&per=%d&spd=%d&pit=%d&vol=%d&aue=%d";
    snprintf(params, sizeof(params), params_pattern , cuid, token, tex,
             config->per, config->spd, config->pit, config->vol, config->aue);
			 
	char url[sizeof(params) + 200];
	snprintf(url, sizeof(url), "%s?%s", API_TTS_URL, params);
    printf("test in browser: %s\n", url);
    curl_free(cuid);
  	curl_free(tex);
	
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
    curl_easy_setopt(curl, CURLOPT_URL, API_TTS_URL);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5); // 连接5s超时
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60); // 整体请求60s超时
    struct http_result result = {1, config->format ,NULL};
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback); // 检查头部
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &result);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);  // 需要释放
    curl_easy_setopt(curl, CURLOPT_VERBOSE, ENABLE_CURL_VERBOSE);
    CURLcode res_curl = curl_easy_perform(curl);

    RETURN_CODE res = RETURN_OK;
    if (res_curl != CURLE_OK) {
        // curl 失败
        snprintf(g_demo_error_msg, BUFFER_ERROR_SIZE, "perform curl error:%d, %s.\n", res,
                 curl_easy_strerror(res_curl));
        res = ERROR_TTS_CURL;
    }
	if (result.fp != NULL) {
		fclose(result.fp);
	}
    curl_easy_cleanup(curl);
    return res;
}
#endif

//i assume this code_transfer data is hex 
//only use in this item
int one_urlencode_utf_8(unsigned char *code_transfer,unsigned char *des_transfer,int len)
{
	int i = 0,pos_len=0;
	//printf("len is %d\n",len);
	//for(i=0;i<len;i++)
	//{
	//	printf("value=%02x\n",code_transfer[i]);
	//}
	for(i=0;i<len;i++)
	{
		//des_transfer[j++] =
		sprintf(des_transfer+pos_len,"%%%2x",code_transfer[i]);
		pos_len = strlen(des_transfer);
	}
	return 0;
}

int two_urlencode_uft_8(unsigned char *code_transfer,unsigned char *des_transfer,int len)
{
	int i = 0, pos_len = 0;
	for(i = 0; i< len;i++)
	{
		sprintf(des_transfer+pos_len,"%%25%2x",code_transfer[i]);
		pos_len = strlen(des_transfer);
	}
	return 0;
}

void tts_recv_task(void *param)
{
	HTTP_RECV_INFO  point_entity;
	LPHTTP_RECV_INFO tmp_recv = &point_entity;
	unsigned char pbuf[1024];
	while(1)
	{
		if(xQueueReceive(recv_tts_queue,&tmp_recv,5000/portTICK_RATE_MS))
		{
			printf("queue has data push data len=%d,recv addr=%p\n",tmp_recv->len,tmp_recv->recv_addr);
			memcpy(pbuf,tmp_recv->recv_addr,tmp_recv->len);
			printf("pbuf recv0=%d,recv1=%d,recv2=%d,recv3=%d\n",pbuf[0],pbuf[1],pbuf[2],pbuf[3]);
			parse_mp3_frame(pbuf,tmp_recv->len);
		}
		else
		{
			printf("queue last 5 s time dont have data\n");
		}
	}

}

void init_queue_to_recv_tts(void)
{
	int SetQueueLength = 1;
	int SetItemSize;
	SetItemSize = sizeof(LPHTTP_RECV_INFO);
	printf("set item size=%d\n",SetItemSize);
	recv_tts_queue = xQueueCreate(SetQueueLength,SetItemSize);

	if(recv_tts_queue != NULL)
	{
		if(xTaskCreate(tts_recv_task,((const char*)"recv tts"),1024,NULL,tskIDLE_PRIORITY+4,NULL) != pdTRUE)
		{
			printf("create tts recv task failed\n");
		}
	}
	else
	{
		printf("recv queue create faild");
	}

}
