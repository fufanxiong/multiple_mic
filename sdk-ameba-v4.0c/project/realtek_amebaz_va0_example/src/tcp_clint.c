
#include <stdint.h>
#include <stdlib.h>
#include <wifi/wifi_conf.h>
//#include  "tcp_client.h"
#include "FreeRTOS.h"
#include "task.h"
#include <platform/platform_stdlib.h>
#include <lwip/sockets.h>
#include <wlan_fast_connect/example_wlan_fast_connect.h>
//#include "tcp_client.h"
//#include "HAPLightbulb.h"
#include "lwip_netconf.h"
//#include "WACServer/WAC.h"
//#include "i2c_api.h"
//#include "ota_8195a.h"
//#include "Timer_LED_onoff.h"
//#include "wdt_api.h"


#define CONNECT_REMOTE  1


//#define REMOTE_HOST      "us.jingxuncloud.com"  //"54.223.249.109"
#define REMOTE_HOST      "query.jingxuncloud.com"
//#define REMOTE_HOST        "cn.jingxuncloud.com"

//#define REMOTE_HOST		"39.108.208.204"
#define REMOTE_PORT    6000
//#define REMOTE_PORT    9192

#define PROBE_PORT       15000
#define GROUP_PORT     	 16000
//#define set_group_udp
  
#define MAX_SOCKETS     15
#define SELECT_TIMEOUT  10
#define SERVER_PORT     6002
#define LISTEN_QLEN     2
#define SELECT_TIMEOUT_SOCKET  1
#define SOCKET_CLOSE_TIMEOUT 120

#define UUID  "JX2-12903-f74e"

//#define MAX_CW_PWM 100
#define MAX_CW_PWM 255		//beacuse cw and ww has 0~255

#define BT2CW_C(b,r)  (b*r*MAX_CW_PWM/10000)
#define BT2CW_W(b,r)  (b*(100-r)*MAX_CW_PWM/10000)		//reference haplightbulb.c

#define   DNS_Interval_Time     150				//dns interval timer to query dns server

#if 0
S_TCP_CLIENT_PARA remote_service_par;
char Phone_ID_buf[24];
int Phone_ID_length=0;
int connect_remote_process=0;
unsigned char Device_IMEI[16];
unsigned char recvBuff[1024]; 
int scan_ap_end=0;
int wac_configure_sucess=0;
rtw_scan_result_t  ssid_info[MAX_SSID_NUM];
int system_reboot=0;
int socket_used[MAX_SOCKETS];
int socket_timeout[MAX_SOCKETS];


unsigned char ota_port[6];
unsigned char ota_user_ip[24];
unsigned char user_home[60];

extern  HAPLight_HSB hsb_status;
unsigned char RGBW_value[4];
#endif
/*
#define   USER_RTC_TIMER_ID     TIMER4
#define   SECOND     1000000

static gtimer_t user_rtc;
*/

int local_server_fd;

int write_to_info_fd = -1;

int socket_used[MAX_SOCKETS];


#if 0

int sleep_delaytime=0;

unsigned char  sleepmode_lighton_flag=0;
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))
unsigned char  RGB_changemode_flag=0;
MODE_COLOR	special_control;
#endif
static unsigned char  valuebufftemp[3];

unsigned char  echo_cmd_tempbuff[20];
unsigned char  send_echo_data_buff[20];
unsigned char  send_echo_data_len=0;
unsigned char  echo_cmd_flag=0;

int ota_socket=-1;
int ota_started=0;

int ota_result=0;

 extern HAPLight_status  Light_status;
extern struct netif xnetif[NET_IF_NUM];
extern int error_flag;
 extern HAPLight_RGB rgb_status;
 extern HAPLight_CT color_temp_status;
 extern u8	special_light;
extern RGB_set RGBset;
extern HSV_set HSVset;

extern Schdule_task TmrLed_period[USER_SET_TIMER_TASK_MAX];
struct rtw_wifi_setting read_setting ={0};
static unsigned char connect_flag = 0;

extern rtw_mode_t wifi_mode;
//extern void start_hap_server(void);
//extern int update_ota_local_jingxun(uint32_t file_size,uint32_t check_sum,int32_t socket_fd);
extern unsigned char SET_TIME_FLAG;
extern bool SntpGetTimeSuccessFlag;

//extern int yield_time;		//updata data speed
//extern unsigned char yield_time_flag;
//extern MODE_COLOR mode_set;
extern u8 rgb_map[13][3];
MUSIC_PARA music_rgb_cw;
unsigned char music_flag = 1;
//HAPLight_CT music_cw;
unsigned char phone_heart_term_flag=0;
unsigned char phone_heart_term_count = 0;
unsigned char dns_flag = 0;
unsigned short dns_count = 0;
ip_addr_t get_remote_server_address;


unsigned short socket_conut=0;		//when sys socket create err,restart this thread
extern struct tm sys_tm;
extern tmrled_user_set_t TmrLed_onoff_current_task;

//static unsigned int test_count=0;
//static unsigned char test_flag=0;
//unsigned char continue_data_flag;
//extern unsigned char status_change_flag;

extern void m_SetCurrentTime(char *time);
extern void ota_platform_reset(void);
extern void pwm_rgb_wwcw(u8 *buf, u8 len);

#endif

#if 0
/***********************************************/
/* 转换*/
/***********************************************/
 unsigned int  _inet_addr1(char *str)
{
    unsigned int ipaddr;
    unsigned int data[4];
    unsigned int ret;

    ret = sscanf(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);
    if (ret < 0) {
        return 0;
    } else {
        ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }
    return ipaddr;
}
#if 1
static char str[20];

char * _inet_ntoa1(unsigned int ip)
{
    memset(str, 0, sizeof (str));
    sprintf(str, "%u.%u.%u.%u",
            (unsigned) (ip >> 24),
            (unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
    return str;
}
#endif


/********************************************** ***/
/*字符串转换为十六进制，每两个字符转换为一个字节*/
/*eg:"0530"===>0x05 0x30*/
/*************************************************/
int Str2Hex(unsigned char *dst_str,unsigned char *src_str,int length)
{
	int i;

	for(i=0;i<(length/2);i++){
		if((src_str[2*i] <='9') && (src_str[2*i]>='0')){
			dst_str[i]=(src_str[2*i]-'0')*16;
		}
		else if((src_str[2*i] <='F') && (src_str[2*i] >='A')){
			dst_str[i]=(src_str[2*i]-'A'+0x0a)*16;
		}
		else if((src_str[2*i] <='f') && (src_str[2*i] >='a')){
			dst_str[i]=(src_str[2*i]-'a'+0x0a)*16;
		}
		else {
			printf("Unsupport char 0x%x\n",src_str[2*i]);
			return -1;
		}

		if((src_str[2*i+1] <='9') && (src_str[2*i+1]>='0')){
			dst_str[i] += (src_str[i*2+1]-'0');
		} 		
		else if((src_str[2*i+1] <='F')&&(src_str[2*i+1] >='A')) dst_str[i] += (src_str[i*2+1]-'A'+0xA); 
		else if((src_str[2*i+1] <='f')&&(src_str[2*i+1] >='a')) dst_str[i] += (src_str[i*2+1]-'a'+0xA); 
		else{
			printf("Unsupport char 0x%x\n",src_str[2*i+1]);
			return -1;
		}
	}
	return i;
}

/********************************************** ***/
/*十六进制转化为字符串*/
/*eg:0x05 0x30===>"0530"*/
/*************************************************/
int Hex2Str(unsigned char *dst_str,unsigned char *src_str,int length)
{
	int i;
	unsigned char byte_low,byte_high;
	for(i=0;i<length;i++){
		byte_low = src_str[i]&0x0f;
		byte_high = (src_str[i]>>4) & 0x0f;
		if(byte_high<0x0a) dst_str[i*2]=byte_high+0x30;
		else dst_str[i*2]=byte_high-0x0a+'A';
		if(byte_low<0x0a) dst_str[i*2+1]=byte_low+0x30;
		else dst_str[i*2+1]=byte_low-0x0a+'A';
	}
	//A_PRINTF("Hex2Str=%s\n",dst_str);
	return i;
}

/*****************************************************************

设备升级
******************************************************************/

#if REMOTE_SERVER==JINGXUN_CLOUND
void user_http_update_ota_task(void *param){
	
	vTaskDelay(3*1000);
	int ret = -1;
    int updata_port=6001;
	//ret = http_update_ota(ota_user_ip, updata_port, &user_home[1]);

exit:	
	printf("\n\r[%s] Update task exit", __FUNCTION__);
	if(!ret){
		printf("\n\r[%s] Ready to reboot", __FUNCTION__);	
		ota_platform_reset();
	}
	vTaskDelete(NULL);	
}


void user_ota_http(void){
	if(xTaskCreate(user_http_update_ota_task, (char const *)"http_update_ota_task", 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS){
		printf("\n\r[%s] Create update task failed", __FUNCTION__);
	}
}




int
qca_ota_upgrade(char *ip_addr, char *file_path,unsigned int protocol, int partition_index)
{
    int ret;
    unsigned short i=0;

    
    /* input parameter sanity check */
    if (!file_path) {
        return 1005;
    }
    if (protocol > 1) {
        return 1006;
    }
		
    /* do OTA upgrade according with different protocol */
    switch (protocol) {
	    case 1: 
		    user_ota_http();
	        break;
	    default:
	        ret = 1006;
	        break;
    }
    return ret;
}




/******************WIFI SOFT_UPDATA_ACK******************************/


//软件升级网址分析得到用于升级ip和升级文件
//参数1：升级网址
//参数2：参数1长度
void Soft_updata_url_anlysis(char *data,unsigned short len)
{
	unsigned short i,j=0,flag=0;
	//unsigned char Updata_IP[24];
	//unsigned char home[60];

	//http://219.133.59.110:6001/importFile/woan_1_V1.8.0.bin
	memset(ota_user_ip,0,sizeof(ota_user_ip));
	memset(user_home,0,sizeof(user_home));
	memset(ota_port,0,sizeof(ota_port));
	for(i=5;i<len;i++)
	{
		if(data[i]=='/')                                    //http:/     /  219.133.59.110:6001  /  importFile   /  woan_1_V1.8.0.bin          
		{                                                   //     flag          updata_ip          flag               flag        home
			flag++;                                         //     1      2                             3                   4
			if((flag==2)||(flag==4))
			{j=0;}
		}
		if(data[i]==':')
		{
			flag++;
			if(flag==3)
			{
               j=0;
			}
		}		
		if(flag==2)
		{
			if(data[i]!='/')
				{
				ota_user_ip[j++] = data[i];
				}
		}
		if(flag==3)
		{
          if(data[i]!=':')
          	{
				ota_port[j++] = data[i];  
          	}
		}
		else if(flag>=4)
			{			
			  user_home[j++] = data[i];				
			}
	}
	printf("Updata_IP:%s  home:%s  port:%s \n",ota_user_ip,user_home,ota_port);
	//Dev_basic_para.Soft_updata_flag = 1;
	//ota 软件升级
	qca_ota_upgrade((char*)ota_user_ip, (char*)user_home, 1,-1);      
	//Dev_basic_para.Soft_updata_flag = 0;
}

void WIFI_SOFT_UPDATA(char *recv)
{
	unsigned short recv_tag;
	unsigned short tlv_lenth;
	unsigned char mURL[100];

	memset(mURL,0,sizeof(mURL));
	recv_tag =(unsigned short)(recv[TCP_Head_Len]<<8)+(unsigned short)(recv[TCP_Head_Len+1]);
	tlv_lenth = (unsigned short)(recv[TCP_Head_Len+2]<<8)+(unsigned short)(recv[TCP_Head_Len+3]);
	printf("recv_tag == 0x%x   tlv_lenth==0x%x\n",recv_tag,tlv_lenth);
	memcpy(mURL,recv+TCP_Head_Len+4,tlv_lenth);
	printf("WIFI_SOFT_UPDATA  URL:%s len:%d \n",mURL,tlv_lenth);
	Soft_updata_url_anlysis((char*)mURL,tlv_lenth);                             //网址解析
	//m_Send_Message_Body(SOFT_UPDATA_ACK,0);
}
#endif


int generate_imei_from_mac(unsigned char *buff_ptr)
{
	int i;
	unsigned char mac_buff[20];
	i=wifi_get_mac_address(mac_buff);
	if(i<0){
		printf("tian debug get mac error\n");
		return i;
	}		
	sprintf(buff_ptr,"%s-",DEV_PREFIX);
	//memcpy(buff_ptr+4,mac_buff+3,5);
	i=4;
	buff_ptr[i++]=mac_buff[4];
	buff_ptr[i++]=mac_buff[6];
	buff_ptr[i++]=mac_buff[7];
	buff_ptr[i++]=mac_buff[9];
	buff_ptr[i++]=mac_buff[10];
	buff_ptr[i++]='-';
	//memcpy(buff_ptr+10,mac_buff+8,4);
	buff_ptr[i++]=mac_buff[12];
	buff_ptr[i++]=mac_buff[13];
	buff_ptr[i++]=mac_buff[15];
	buff_ptr[i++]=mac_buff[16];
	buff_ptr[i]=0;
	printf("tian debug1,mac:%s, IMEI:%s\n",mac_buff,buff_ptr);
	return 0;
}
#if REMOTE_SERVER==JINGXUN_CLOUND
void  remote_dns_found(const char* hostname, ip_addr_t *ipaddr, void *arg)
{
  LWIP_UNUSED_ARG(hostname);
  LWIP_UNUSED_ARG(arg);
  //ip_addr_t remote_server_address;
  //err_t err;
  if (ipaddr != NULL) {
    /* Address resolved, send request */
	//dns_flag = 1;
	remote_service_par.remote_status=REMOTE_DNS;
	memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
	strcpy(remote_service_par.serverip,ipaddr_ntoa(ipaddr));
	 printf("dns sucess 11,%s-->IP:%s\n",hostname,remote_service_par.serverip);
  } else {
    /* DNS resolving failed -> try another server */
	printf("~~~dns failed~~~,try again!");
#if 0
	err=dns_gethostbyname(REMOTE_HOST, &remote_server_address, remote_dns_found, NULL);
	//sntp_try_next_server(NULL);
	if(err==ERR_INPROGRESS){
		remote_service_par.remote_status=REMOTE_DNS;
		printf("dns in progeress 11\n");
	}else if (err == ERR_OK) {
		dns_flag = 1;
		memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
		strcpy(remote_service_par.serverip,ipaddr_ntoa(&remote_server_address));
		printf("dns ok 11,ip=%s\n",remote_service_par.serverip);
		//remote_socket_create();    //try connect remote server
		//if(remote_service_par.remote_socket_fd >0){
		//	remote_service_par.remote_status=REMOTE_SERVER_QUERY;
		//}

	}else{
		printf("something wrong when dns 11,code=%d\n",err);
	}
#endif

  }
}

int GetHostByName(void)
{
	err_t err;
	dns_count = DNS_Interval_Time;
	err=dns_gethostbyname(REMOTE_HOST, &get_remote_server_address, remote_dns_found, NULL);
			//remote_service_par.remote_status=REMOTE_DNS;		//fu add because test server needn,t dns
				if(err==ERR_INPROGRESS){
					//remote_service_par.remote_status=REMOTE_DNS;
					printf("dns in progeress %s\n",__FUNCTION__);
				}else if (err == ERR_OK) {
					remote_service_par.remote_status=REMOTE_DNS;
					memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
					strcpy(remote_service_par.serverip,ipaddr_ntoa(&get_remote_server_address));
					printf("dns ok,ip=%s\n",remote_service_par.serverip);
			//remote_socket_create();    //try connect remote server
			//if(remote_service_par.remote_socket_fd >0){
			//	remote_service_par.remote_status=REMOTE_SERVER_QUERY;
			//}
				}else{
					printf("something wrong when dns,code=%d\n",err);
				}		
	return 0;
}


void remote_socket_create_task(void *param)
{
	int remote_fd=-1;
	struct sockaddr_in remote_addr;
	connect_remote_process=1;
	if((remote_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
			remote_addr.sin_family = AF_INET;
			remote_addr.sin_addr.s_addr = inet_addr(remote_service_par.serverip);
			remote_addr.sin_port = htons(REMOTE_PORT);
			if(connect(remote_fd, (struct sockaddr *) &remote_addr, sizeof(remote_addr)) == 0) {
				printf("remote socket(%d) connect sucess\n", remote_fd);
				remote_service_par.remote_socket_fd=remote_fd;
				if(remote_service_par.remote_status==REMOTE_DNS)
						remote_service_par.remote_status=REMOTE_SERVER_QUERY;
				//return 0;
			}
			else {
				printf("tian connect remote error\n");
				close(remote_fd);
				//return -1;
			}
	}
	else {
		printf("remote socket create error\n");
		//return -1;
	}
	connect_remote_process=0;
	vTaskDelete(NULL);
}
int remote_socket_create(void)
{
	//static int task_in_process=0;
	if(remote_service_par.remote_socket_fd >=0) return 0;
	if(connect_remote_process==0){
		if(xTaskCreate(remote_socket_create_task, ((const char*)"remote_socket_create_task"), 1024, NULL, tskIDLE_PRIORITY + 3, NULL) != pdPASS)
			printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);
	}
	return -1;
}
int m_Query_server_address(void)
{
//	unsigned short i;
	unsigned short msg_lenth=0;
	char buf[64];
	if(remote_service_par.remote_socket_fd <0) {
		printf("wait remote socket\n");
		return -1;
	}	
	memset(buf, 0x0, sizeof(buf));
       sprintf(buf,"{query_server_id#%s}",Device_IMEI);
	msg_lenth = strlen(buf);
	
	printf("%s,Send data:%s\n",__func__,buf);
	write(remote_service_par.remote_socket_fd, buf,strlen(buf));
	return 0;
}
void decode_query_response(char *httpmsg)   
{
	//{"server_id":"192.168.17.1","port":"5000"}
	int i=0,j=0,flag=0;

	printf("decode_query_response data == %s\n",httpmsg);
	memset(remote_service_par.query_serverip,0x0,sizeof(remote_service_par.query_serverip));
	memset(remote_service_par.query_serverport,0x0,sizeof(remote_service_par.query_serverport));
	for(i=0;i<strlen(httpmsg);i++)  //httpmsg[0]='s'
	{
		if(httpmsg[i]=='"')
		{
			if(flag%2==0)j=0;
				flag++;
			i++;
		}
		if(flag==3)
			remote_service_par.query_serverip[j++] = httpmsg[i];     //存放ip  192.168.17.1
		else if(flag==7)
			remote_service_par.query_serverport[j++] = httpmsg[i];      //存放端口值5000
	}
	//A_PRINTF("decode  query_serverip == %s\n",client_para.query_serverip);
	if(strlen(remote_service_par.query_serverip) != 0)
	{
		printf("serverip == %s\n",remote_service_par.serverip);
		if(strcmp(remote_service_par.query_serverip,remote_service_par.serverip) != 0)
		{
			if(remote_service_par.remote_socket_fd>=0) 
			{
				//TCP_Client_Close(&client_para.tcp_client_fd);
				//qcom_thread_msleep(100);
				printf("tian change serverip\n");
			}
			memset(remote_service_par.serverip,0x0,sizeof(remote_service_par.serverip));
			memcpy(remote_service_par.serverip,remote_service_par.query_serverip,strlen(remote_service_par.query_serverip));
		}
		//nvram_bufset("Server_Ip", client_para.serverip);
		//nvram_commit();
		//qcom_thread_msleep(100);
		remote_service_par.remote_status= REMOTE_SERVER_REGISTER;
		//client_para.count = 0;
	}
}
/***********************************************************************
*Description:与云服务器通讯消息体的构建及发送
*          
*Author:zq   Date: 2014-10-15
*
************************************************************************/
//添加报文头
int m_ADD_Message_Head(char *msg_head,unsigned short cmd)
{
	S_TCP_HEAD head;
	char smac_id[15];                                   
	int i;                                                                                    //head.len = htons(TCP_Head_Len);
	                                                                                           //head.cmd =  htons(cmd);
	memset(smac_id,0,sizeof(smac_id));                                      //device_id=Dev_basic_para.UID
	strcpy(smac_id,(char*)Device_IMEI);                          //head.protocol_number=htons(Protocol_Number);

	head.len = htons(TCP_Head_Len);                                         //head.aes=0x00;
	head.cmd =  htons(cmd);
	for(i=0;i<14;i++) {                                                              //head.reseve=0x00;
		head.device_id[i]=smac_id[i];     //设备ID即为uid
	}
	head.protocol_number=htons(Protocol_Number);
	head.aes=0x00;
	
	for(i=0;i<4;i++) {
		head.reseve[i]=0x00;
	}
	memcpy(msg_head,(char *)&head,TCP_Head_Len);
	return 0;	
} 

/******************WIFI REGISTER******************************/
int WIFI_REGISTER(char *senddata)
{
	unsigned short i;
	unsigned short msg_lenth;

	m_ADD_Message_Head(senddata,T_REGISTER);
	i=TCP_Head_Len;
	*(unsigned short *)(senddata+i)=htons(soft_version);//tag
	i+=2;
	*(unsigned short *)(senddata+i)=htons(strlen(SoftVersion));//lenth
	i+=2;
	memcpy(senddata+i,SoftVersion,strlen(SoftVersion));
	i+=strlen(SoftVersion);
	*(unsigned short *)(senddata+i)=htons(dev_password);
	i+=2;
	*(unsigned short *)(senddata+i)=htons(strlen(Deault_password));
	i+=2;
	memcpy(senddata+i,Deault_password,strlen(Deault_password));
	i+=strlen((char*)Deault_password);//39
	*(unsigned short *)(senddata)=htons(i);
      
	msg_lenth = i;
	//mretry = 0;//判断是否为重发
	return msg_lenth;	
}
void WIFI_REGISTER_ACK(char *recv)
{
	unsigned short recv_tag;
	//unsigned short tlv_lenth;
	unsigned char  response_result;
	
	response_result = 	recv[TCP_Head_Len];
	printf("T_REGISTER_ACK_result == 0x%x \n",response_result);


    //当结果为01时服务器平台下发给wifi终端4004 的信息
    
    //0003	2	0003 0002 001E 终端心跳间隔30秒
	if(response_result == 0x01) 
	{//regsiter succese
		recv_tag =(unsigned short)(recv[TCP_Head_Len+1]<<8)+(unsigned short)(recv[TCP_Head_Len+2]);
		//if(recv_tag == 0x0003)
			remote_service_par.remote_status=REMOTE_OK;
		//else 
			printf("T_REGISTER_ACK tag OK!,heartbeat=%d\n",recv_tag);
	}
	else if(response_result == 0x00)
		printf("REGISTER FAIL! \n");
	else if(response_result == 0x02)
	{
		printf("REGISTERED! \n");
		remote_service_par.remote_status=REMOTE_OK;
	}
	else
		printf("REGISTERED OTHER ERROR!\n");	
}
/******************WIFI HEART_BEAT******************************/
int WIFI_HEART_BEAT(char *senddata)
{
	unsigned short i;
	unsigned short msg_lenth;
	
	m_ADD_Message_Head(senddata,HEART_BEAT);
	i=TCP_Head_Len;
	*(unsigned short *)(senddata)=htons(i);
	msg_lenth = i;
	return msg_lenth;
}
//添加报文体内容tag+len+data
void  m_ADD_Message_SmaterID(char *message)
{
	S_TCP_MasterID body;
	int i;

	body.cmd=htons(subdev_id);
	body.len = htons(2);
	for(i=0;i<2;i++) {
		body.data[i]=0x30;
	}
	memcpy(message,(char *)&body,6);   
}
#endif
/***************************************
清除模式设置

*****************************************/
void clear_setmode_handler()
{
	Light_status.mode=0;

#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))	
	if(RGB_changemode_flag==1)
	{
		RGB_changemode_flag=0;
	}
#endif
	if(sleepmode_lighton_flag==1)
    {		
		sleep_delaytime=0;
		sleepmode_lighton_flag=0;		
	}
}

/***************************************
RGB转RGBW
*****************************************/
int  get_RGBW_value(void)
{
#if 0
	 //'Ri', 'Gi', and 'Bi' correspond to the Red, Green, and Blue inputs.
        int M = get_max(rgb_status.r,rgb_status.g,rgb_status.b);; //The maximum value between R,G, and B.
        int Wo =0; //White output
        int Ro=0; //Red output
        int Go=0; //Green output
        int Bo=0; //Blue output

        int av = 0; //Average between the two minimum values
        int hR = 0; //Red with 100% hue
        int hG = 0; //Green with 100% hue
        int hB = 0; //Blue with 100% hue

        //These 4 lines serve to figure out what the input color is with 100% hue.
        float multiplier = 255.0f / M;
        hR = (int)(rgb_status.r * multiplier);
        hG = (int)(rgb_status.g* multiplier);
        hB = (int)(rgb_status.b * multiplier);

        //Depending on the maximum value, get an average of the least used colors, weighted for their importance in the overall hue.
        //This is the problematic part
        if (M == rgb_status.r)
           av = (rgb_status.b*hB + rgb_status.g*hG) / (hB+hG);
        else if (M == rgb_status.g)
            av = (rgb_status.r*hR + rgb_status.b*hB) / (hR+hB);
        else if (M == rgb_status.b)
            av = (rgb_status.g*hG + rgb_status.r*hR) / (hG+hR);

        //Set the rgbw colors
        Wo = av;
        Bo = rgb_status.b - av;
        Ro = rgb_status.r - av;
        Go = rgb_status.g - av;
        if (Wo < 1) Wo = 0;
        if (Bo < 1) Bo = 0;
        if (Ro < 1) Ro = 0;
        if (Go < 1) Go = 0;
        if (Wo > 255) Wo = 255;
        if (Bo > 255) Bo = 255;
        if (Ro > 255) Ro = 255;
        if (Go > 255) Go = 255;
	RGBW_value[0]=Ro;
	RGBW_value[1]=Go;
	RGBW_value[2]=Bo;
	RGBW_value[3]=Wo;
#else
	u8 min_led_val=get_min(rgb_status.r,rgb_status.g,rgb_status.b);
	u8 max_led_val=get_max(rgb_status.r,rgb_status.g,rgb_status.b);

	//if(max_led_val>0)
	{
		RGBW_value[3]=min_led_val;
								
		RGBW_value[0]=((rgb_status.r*(min_led_val+max_led_val))/max_led_val)-min_led_val;
		RGBW_value[1]=((rgb_status.g*(min_led_val+max_led_val))/max_led_val)-min_led_val;
		RGBW_value[2]=((rgb_status.b*(min_led_val+max_led_val))/max_led_val)-min_led_val;	
		//return 0;
	}
#endif	
	printf("rgb-rgbw:%d-%d-%d-->%d-%d-%d-%d\n",rgb_status.r,rgb_status.g,rgb_status.b,RGBW_value[0],RGBW_value[1],RGBW_value[2],RGBW_value[3]);
	return 0;
}


void local_report_status(int fd,unsigned char flag)			//flag = 0,repeat send local ,flag = 1,repeat send remote
{
	unsigned int  send_len=0;
	char temp[60]={0};
	char report_buf[200]={0};
	//char local_buf[100]={0};
	char cmd_buf[5]={0};
	int ret;
	unsigned short i,j,k;
	//int data_cmd = 1;

	//sprintf(&temp[4],"820000000000%02x",data_cmd);
	sprintf(&temp[4],"8200%04x0000dc00%02x00",MODEL,MODEL);

	if(Light_status.onoff == 0)
		sprintf(temp+4+strlen(temp+4), "%02x%02x%02x%02x",Light_status.Cw.cool,Light_status.Cw.warm,0,Light_status.onoff);
	else
		sprintf(temp+4+strlen(temp+4), "%02x%02x%02x%02x",Light_status.Cw.cool,Light_status.Cw.warm,Light_status.brightness,Light_status.onoff);
	sprintf(temp+4+strlen(temp+4), "%02x%02x%02x",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
	send_len = strlen(temp+4);
	memset(cmd_buf,0,sizeof(cmd_buf));
	sprintf(cmd_buf,"%04x",send_len/2);
	memcpy(temp,cmd_buf,4);
	
	if(flag == 0)
	{	
		sprintf(&report_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"SubDev\":\"\",\"data3\":\"",Device_IMEI);
		sprintf(report_buf+6+strlen(report_buf+6),"%s",temp);
		sprintf(report_buf+6+strlen(report_buf+6),"\",\"suc\":\"%d%d\"}",0,0);
			
		strcpy(report_buf,"##");                               //## 包识别头
		send_len = strlen(&report_buf[6]);                       //回复信息长度
		memset(cmd_buf,0,sizeof(cmd_buf));
		sprintf(cmd_buf,"%04x",send_len);
		memcpy(&report_buf[2],cmd_buf,4);      //将信息长度存入buf[2] 
		strcpy(&report_buf[send_len+6],"&&");
		i = strlen(report_buf);

		for(k=0;k<MAX_SOCKETS;k++){
			//printf("k=%d\n",k);
			if(socket_used[k]==1){
				//printf("into >>>fd=%d\n",k);
				if((k==remote_service_par.local_server_fd) || (k==remote_service_par.probe_fd) || k==remote_service_par.remote_socket_fd)
				{
					continue;
				}		
				else {
					printf("fd=%d\n",k);
					ret = write(k,report_buf,i);
					vTaskDelay(10);
				}				
			}	
		}
		
		
	}
	else if(flag == 1) {
		//if(phone_heart_term_flag == 1)
		//{
			m_ADD_Message_Head(report_buf,remote_reported_phone);
			i = TCP_Head_Len;
			m_ADD_Message_SmaterID(&report_buf[i]);
			i+=6;
			*(unsigned short *)(report_buf+i)=htons(heater_transfer_ack);
			i+=2;
			*(unsigned short *)(report_buf+i)=htons(send_len+4);
			i+=2;
			memcpy(report_buf+i,temp,send_len+4);
			//sprintf(report_buf+i,"%s",temp);
			i+=(send_len+4);
			*(unsigned short*)(report_buf)=htons(i);		
		//}
		ret = write(fd,report_buf,i);
#if 1
		printf("len=%d\n",i);
		for(j=0;j<i;j++)
			printf("%02x",report_buf[j]);
#endif
	}
#if 0
	for(k=0;k<MAX_SOCKETS;k++){
			if(socket_used[k]==1){
				printf("into >>>fd=%d\n",i);
				if((k==remote_service_par.local_server_fd) || (k==remote_service_par.probe_fd))
				{
					continue;
				}
				
				if(k==remote_service_par.remote_socket_fd){
					if(phone_heart_term_flag == 1)		//remote phone pass through heart
						ret = write(k,report_buf,i);	
				}else {
					printf("fd=%d\n",i);
					ret = write(k,local_buf,j);
				}				
			}	
		}
#endif
	//ret = write(fd,report_buf,i);
	if(ret < 0)
		printf("report send data err\n");
	else 
		printf("report data :%s\n",report_buf);			//remote is hex so printf one byte is 0,so look like null
}


void initiative_report_status()
{
	int j;
#if 1
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))
				if(RGB_changemode_flag == 0)
#endif
				{
#if 1
					for(j=0;j<MAX_SOCKETS;j++){
						if(socket_used[j]==1){
							if((j==remote_service_par.local_server_fd) || (j==remote_service_par.probe_fd))
							{
								continue;
							}
							
							if(j==remote_service_par.remote_socket_fd){
								if(phone_heart_term_flag == 1)		//remote phone pass through heart
									local_report_status(j,1);	
							}else {
								printf("fd=%d\n",j);
								local_report_status(j,0);
								break;
							}				
						}	
					}
#endif
				}
#endif

}


/*********************************

开关灯处理
**********************************/
void  light_turn_onoff_handler(unsigned char onoff)
{
	unsigned char i=0;
	u8 buf_rgbcw[15]={0};
	u8 length=0;
	int	ret=0;	
//	HAPLight_RGB temp_rgb;

   if(onoff==1)
   {

#if 0
		buf_rgbcw[0]= 0x04;
		buf_rgbcw[1]= 0x08;
#if(DEVICE_TYPE == RGB_LIGHT)

		if(Light_status.RGBmode==1)
		{
			temp_rgb.r = rgb_status.r;
			temp_rgb.g = rgb_status.g;
			temp_rgb.b = rgb_status.b;
			buf_rgbcw[5] = 0;
		}
		else
		{
			temp_rgb.r = color_temp_status.cool;
			temp_rgb.g = color_temp_status.cool;
			temp_rgb.b = color_temp_status.cool;
			buf_rgbcw[5] = color_temp_status.warm;
		}
		buf_rgbcw[2] = temp_rgb.r;
		buf_rgbcw[3] = temp_rgb.g;
		buf_rgbcw[4] = temp_rgb.b;
		buf_rgbcw[6] = 0;
		for(i=0;i<6;i++)
		{
			buf_rgbcw[6]+=buf_rgbcw[i];
		}
		length = 7;
		pwm_rgb_wwcw(buf_rgbcw,length);
		printf("fu debug set rgbw : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);
			
#endif

#endif	//if 0

		if(Light_status.onoff == 0)
			Light_status.onoff = 1;

#if 0						//no matter single /double / rgb ,set cmd  rgbcw-----0x0a

#if(DEVICE_TYPE == RGB_LIGHT)
		
		if(Light_status.RGBmode==1)
		{
			memset(&Light_status.Cw,0,sizeof(HAPLight_CT));			//adjust rgb ,so send app cw para so set zero
#if 0
			//ret= get_RGBW_value();
			if(ret==1)
			{
			  return;
			}

			buf_rgbcw[0]= 0x04;
			buf_rgbcw[1]= 0x09;
			buf_rgbcw[2]= RGBW_value[0];
			buf_rgbcw[3]= RGBW_value[1];
			buf_rgbcw[4]= RGBW_value[2];
			buf_rgbcw[5]= RGBW_value[3];
			buf_rgbcw[6]= 0;
#else
			buf_rgbcw[0]= 0xAA;
			buf_rgbcw[1]= 0x08;
			buf_rgbcw[2]= 0x0a;
			buf_rgbcw[3]= Light_status.Rgb.r;
			buf_rgbcw[4]= Light_status.Rgb.g;
			buf_rgbcw[5]= Light_status.Rgb.b;
			buf_rgbcw[6]= 0;
			buf_rgbcw[7]= 0;
			buf_rgbcw[8]= Light_status.brightness;
			buf_rgbcw[9]= 0;
#endif
			for(i=0;i<9;i++)
			{
				buf_rgbcw[9]+=buf_rgbcw[i];
			}
			length = 10;
			printf("into para\n");
			pwm_rgb_wwcw(buf_rgbcw,length);
			printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);
			
			
			#if 0
			i2csenddata[0]=0x04;
			i2csenddata[1]=0x09;
			/*
			i2csenddata[2]=(rgb_status.r*Light_status.brightness)/100;
			i2csenddata[3]=(rgb_status.g*Light_status.brightness)/100;
			i2csenddata[4]=(rgb_status.b*Light_status.brightness)/100;
			*/
			
            		RGBW_value[3]=(RGBW_value[3]*100)/255;
			
			i2csenddata[2]= RGBW_value[0];
			i2csenddata[3]= RGBW_value[1];
			i2csenddata[4]= RGBW_value[2];
			i2csenddata[5]= RGBW_value[3];
			i2csenddata[6]=0;
			for(i=0;i<6;i++)
			{
				i2csenddata[6]+=i2csenddata[i];
			}
		    i2c_write(&i2c_master, USER_I2C_SLAVE_ADDR0, &i2csenddata[0], 7, 1);
			#endif
			//return;
		}else			
#endif
		{

		memset(&Light_status.Rgb,0,sizeof(HAPLight_RGB));		//when adjust cw ,should set rgb para to zero
	
		buf_rgbcw[0]= 0xAA;
		buf_rgbcw[1]= 0x08;
		buf_rgbcw[2]= 0x0a;
		buf_rgbcw[3]= Light_status.Rgb.r;
		buf_rgbcw[4]= Light_status.Rgb.g;
		buf_rgbcw[5]= Light_status.Rgb.b;
		buf_rgbcw[6]= 0;
		buf_rgbcw[7]= 0;
		buf_rgbcw[8]= Light_status.brightness;
		buf_rgbcw[9]= 0;
		for(i=0;i<9;i++)
		{
			buf_rgbcw[9]+=buf_rgbcw[i];
		}
		length = 10;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
		printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);

		}

#else
		if(Light_status.RGBmode==1)
			memset(&Light_status.Cw,0,sizeof(HAPLight_CT));			//adjust rgb ,so send app cw para so set zero
		else
			memset(&Light_status.Rgb,0,sizeof(HAPLight_RGB));		//when adjust cw ,should set rgb para to zero
		buf_rgbcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[1]= 0x09;
#else
		buf_rgbcw[1]= 0x08;
#endif
		buf_rgbcw[2]= 0x0a;
		buf_rgbcw[3]= Light_status.Rgb.r;
		buf_rgbcw[4]= Light_status.Rgb.g;
		buf_rgbcw[5]= Light_status.Rgb.b;
		buf_rgbcw[6]= Light_status.Cw.warm;
		buf_rgbcw[7]= Light_status.Cw.cool;
		buf_rgbcw[8]= Light_status.brightness;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[9]= 0;
		buf_rgbcw[10]= 0;
		for(i=0;i<10;i++)
		{
			buf_rgbcw[10]+=buf_rgbcw[i];
		}
		length = 11;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
#else
		buf_rgbcw[9]= 0;
		for(i=0;i<9;i++)
		{
			buf_rgbcw[9]+=buf_rgbcw[i];
		}
		length = 10;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
#endif

		printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);

#endif
	
   	}
    else if(onoff==0)
    {
    	#if 0
		i2csenddata[0]=0;
		i2csenddata[1]=0x0C;
		i2csenddata[2]=0;
		for(i=0;i<2;i++)
		{
			i2csenddata[2]+=i2csenddata[i];
		}
		i2c_write(&i2c_master, USER_I2C_SLAVE_ADDR0, &i2csenddata[0], 3, 1);
		#endif
#if 1
		if(Light_status.onoff==1)
			Light_status.onoff= 0;
		buf_rgbcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[1]= 0x03;
#else
		buf_rgbcw[1]= 0x02;
#endif
		buf_rgbcw[2]= 0x0C;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[3]= 0;
		buf_rgbcw[4]= 0;
		for(i=0;i<4;i++)
		{
			buf_rgbcw[4] += buf_rgbcw[i];
		}
		length = 5;
		pwm_rgb_wwcw(buf_rgbcw,length);
#else
		buf_rgbcw[3]= 0;
		for(i=0;i<3;i++)
		{
			buf_rgbcw[3] += buf_rgbcw[i];
		}
		length = 4;
		pwm_rgb_wwcw(buf_rgbcw,length);
#endif
		printf("fu debug close\n");
#else
		if(Light_status.onoff==1)
			Light_status.onoff= 0;
		buf_rgbcw[0]= 0xAA;
		buf_rgbcw[1]= 0x08;
		buf_rgbcw[2]= 0x0a;
		buf_rgbcw[3]= Light_status.Rgb.r;
		buf_rgbcw[4]= Light_status.Rgb.g;
		buf_rgbcw[5]= Light_status.Rgb.b;
		buf_rgbcw[6]= Light_status.Cw.warm;
		buf_rgbcw[7]= Light_status.Cw.cool;
		buf_rgbcw[8]= 0;		//close    light = 0
		buf_rgbcw[9]= 0;
		for(i=0;i<9;i++)
		{
			buf_rgbcw[9]+=buf_rgbcw[i];
		}
		length = 10;
		printf("fu debug close\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
		printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);
#endif
	}
	
}


void light_turn_music_handler()
{
	unsigned char i=0;
	u8 buf_rgbcw[15]={0};
	u8 length=0;
	int	ret=0;

		buf_rgbcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[1]= 0x09;
#else
		buf_rgbcw[1]= 0x08;
#endif
		buf_rgbcw[2]= 0x0a;
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))
		buf_rgbcw[3]= music_rgb_cw.music_rgb.r;
		buf_rgbcw[4]= music_rgb_cw.music_rgb.g;
		buf_rgbcw[5]= music_rgb_cw.music_rgb.b;
		buf_rgbcw[6]= 0;
		buf_rgbcw[7]= 0;
#else
		buf_rgbcw[3]= 0;
		buf_rgbcw[4]= 0;
		buf_rgbcw[5]= 0;
		buf_rgbcw[6]= Light_status.Cw.warm;
		buf_rgbcw[7]= Light_status.Cw.cool;
#endif
		buf_rgbcw[8]= music_rgb_cw.music_brightness;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[9]= music_flag;
		buf_rgbcw[10]= 0;
		for(i=0;i<10;i++)
		{
			buf_rgbcw[10]+=buf_rgbcw[i];
		}
		length = 11;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
#else
		buf_rgbcw[9]= 0;
		for(i=0;i<9;i++)
		{
			buf_rgbcw[9]+=buf_rgbcw[i];
		}
		length = 10;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
		printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);
#endif


#if 0
#if(DEVICE_TYPE == RGB_LIGHT)
			buf_rgbcw[0]= 0x04;
			buf_rgbcw[1]= 0x09;
			buf_rgbcw[2]= music_rgb.music_rgb.r;
			buf_rgbcw[3]= music_rgb.music_rgb.g;
			buf_rgbcw[4]= music_rgb.music_rgb.b;
			buf_rgbcw[5]= 0;
			buf_rgbcw[6]= 0;

			for(i=0;i<6;i++)
			{
				buf_rgbcw[6]+=buf_rgbcw[i];
			}
			length = 7;
			printf("into para\n");
			pwm_rgb_wwcw(buf_rgbcw,length);
			printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);
			return;
#endif
					
			buf_rgbcw[0]= 0x02;
			buf_rgbcw[1]= 0x07;
			buf_rgbcw[2]= music_cw.warm;
			buf_rgbcw[3]= music_cw.cool;
			buf_rgbcw[4]= 0;
			for(i=0;i<4;i++)
			{
				buf_rgbcw[4]+=buf_rgbcw[i];
			}
			length = 5;
			printf("into cw\n");
			pwm_rgb_wwcw(buf_rgbcw,length);
			printf("fu debug set wwcw : %02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3]);
#endif

}

void  light_flash_handler(unsigned char onoff)
{
	unsigned char i=0;
	u8 flash_wwcw[15]={0};
	u8 length = 0;
#if 0
   if(onoff==1)
   {	
   	#if 0
		i2csenddata[0]=0x02;
		i2csenddata[1]=0x07;						
		i2csenddata[2]=0;
		i2csenddata[3]=70;						
		i2csenddata[4]=0;
		for(i=0;i<4;i++)
		{
		   i2csenddata[4]+=i2csenddata[i];
		}
		i2c_write(&i2c_master, USER_I2C_SLAVE_ADDR0, &i2csenddata[0], 5, 1);					
	#endif	
   	} 
    else if(onoff==2)
    {	
    #if 0
		i2csenddata[0]=0x02;
		i2csenddata[1]=0x07;						
		i2csenddata[2]=0;
		i2csenddata[3]=10;						
		i2csenddata[4]=0;
		for(i=0;i<4;i++)
		{
		   i2csenddata[4]+=i2csenddata[i];
		}
		i2c_write(&i2c_master, USER_I2C_SLAVE_ADDR0, &i2csenddata[0], 5, 1);					
	#endif	
   	} 
    else if(onoff==0)
    {
    	#if 0
		i2csenddata[0]=0;
		i2csenddata[1]=0x0C;
		i2csenddata[2]=0;
		for(i=0;i<2;i++)
		{
		i2csenddata[2]+=i2csenddata[i];
		}
		i2c_write(&i2c_master, USER_I2C_SLAVE_ADDR0, &i2csenddata[0], 3, 1);
	#endif
	}

#else
	if(onoff==1)
	{
		flash_wwcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
		flash_wwcw[1]= 0x09;
#else
		flash_wwcw[1]= 0x08;
#endif

		flash_wwcw[2]= 0x0a;
		flash_wwcw[3]= 0;
		flash_wwcw[4]= 0;
		flash_wwcw[5]= 0;
		flash_wwcw[6]= 0;
		flash_wwcw[7]= 70;			//cw
		flash_wwcw[8]= 100;			//light
#ifdef 	NOT_GRADUAL_MUSIC
		flash_wwcw[9]= 0;
		flash_wwcw[10]= 0;
		for(i=0;i<10;i++)
		{
			flash_wwcw[10]+=flash_wwcw[i];
		}
		length = 11;
		printf("into para\n");
		pwm_rgb_wwcw(flash_wwcw,length);
#else
		flash_wwcw[9]= 0;
		for(i=0;i<9;i++)
		{
			flash_wwcw[9]+=flash_wwcw[i];
		}
		length = 10;
		printf("into para\n");
		pwm_rgb_wwcw(flash_wwcw,length);
#endif
		printf("fu debug set init rgb : %02x,%02x,%02x,%02x\n",flash_wwcw[2],flash_wwcw[3],flash_wwcw[4],flash_wwcw[5]);
#if 0
		flash_wwcw[0]=0x02;
		flash_wwcw[1]=0x07;
		flash_wwcw[2]=0;
		flash_wwcw[3]=70;
		flash_wwcw[4]=0;
		for(i=0;i<4;i++)
		{
			flash_wwcw[4]+=flash_wwcw[i];
		}
		length = 5;
		printf("fu debug flash wwcw : %02x,%02x\n",flash_wwcw[2],flash_wwcw[3]);
		pwm_rgb_wwcw(flash_wwcw,length);
#endif
	}
	else if(onoff==2)
	{
		flash_wwcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
				flash_wwcw[1]= 0x09;
#else
				flash_wwcw[1]= 0x08;
#endif

		flash_wwcw[2]= 0x0a;
		flash_wwcw[3]= 0;
		flash_wwcw[4]= 0;
		flash_wwcw[5]= 0;
		flash_wwcw[6]= 0;
		flash_wwcw[7]= 10;
		flash_wwcw[8]= 100;
#ifdef 	NOT_GRADUAL_MUSIC
		flash_wwcw[9]= 0;
		flash_wwcw[10]= 0;
		for(i=0;i<10;i++)
		{
			flash_wwcw[10]+=flash_wwcw[i];
		}
		length = 11;
		printf("into para\n");
		pwm_rgb_wwcw(flash_wwcw,length);
#else
		flash_wwcw[9]= 0;
		for(i=0;i<9;i++)
		{
			flash_wwcw[9]+=flash_wwcw[i];
		}
		length = 10;
		printf("into para\n");
		pwm_rgb_wwcw(flash_wwcw,length);
#endif
		printf("fu debug set init rgb : %02x,%02x,%02x,%02x\n",flash_wwcw[2],flash_wwcw[3],flash_wwcw[4],flash_wwcw[5]);
#if 0
		flash_wwcw[0]=0x02;
		flash_wwcw[1]=0x07;
		flash_wwcw[2]=0;
		flash_wwcw[3]=10;
		flash_wwcw[4]=0;
		for(i=0;i<4;i++)
		{
			flash_wwcw[4]+=flash_wwcw[i];
		}
		length = 5;
		printf("fu debug flash wwcw : %02x,%02x\n",flash_wwcw[2],flash_wwcw[3]);
		pwm_rgb_wwcw(flash_wwcw,length);
#endif
	}
	else if(onoff==0)
	{
		flash_wwcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
		flash_wwcw[1]= 0x03;
#else
		flash_wwcw[1]= 0x02;
#endif
		flash_wwcw[2]= 0x0C;
#ifdef	NOT_GRADUAL_MUSIC
		flash_wwcw[3]= 0;
		flash_wwcw[4]= 0;
		for(i=0;i<4;i++)
		{
			flash_wwcw[4] += flash_wwcw[i];
		}
		length = 5;
		pwm_rgb_wwcw(flash_wwcw,length);
#else
		flash_wwcw[3]= 0;
		for(i=0;i<3;i++)
		{
			flash_wwcw[3] += flash_wwcw[i];
		}
		length = 4;
		pwm_rgb_wwcw(flash_wwcw,length);
#endif
		printf("fu debug flash close\n");
#if 0
		flash_wwcw[0]=0x00;
		flash_wwcw[1]=0x0C;		
		length = 2;
		printf("du debug flash close\n");
		pwm_rgb_wwcw(flash_wwcw,length);
#endif
	}
#endif

}

/******************************
特殊控制是动态的变化的参数，这里只会
****************************/

void  special_light_handle()
{
	unsigned char i=0;
	u8 buf_rgbcw[15]={0};
	u8 length=0;
	int	ret=0;

		buf_rgbcw[0]= 0xAA;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[1]= 0x09;
#else
		buf_rgbcw[1]= 0x08;
#endif

		buf_rgbcw[2]= 0x0a;
		buf_rgbcw[3]= rgb_status.r;
		buf_rgbcw[4]= rgb_status.g;
		buf_rgbcw[5]= rgb_status.b;
		//buf_rgbcw[6]= color_temp_status.warm;		//if add speical cw can use it
		//buf_rgbcw[7]= color_temp_status.cool;
		buf_rgbcw[6]= 0;
		buf_rgbcw[7]= 0;
		//buf_rgbcw[8]= special_light;			//if add special light  can use it
		buf_rgbcw[8]= Light_status.brightness;
#ifdef	NOT_GRADUAL_MUSIC
		buf_rgbcw[9]= 0;
		buf_rgbcw[10]= 0;
		for(i=0;i<10;i++)
		{
			buf_rgbcw[10]+=buf_rgbcw[i];
		}
		length = 11;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
#else
		buf_rgbcw[9]= 0;
		for(i=0;i<9;i++)
		{
			buf_rgbcw[9]+=buf_rgbcw[i];
		}
		length = 10;
		printf("into para\n");
		pwm_rgb_wwcw(buf_rgbcw,length);
#endif
		printf("fu debug set rgb : %02x,%02x,%02x,%02x\n",buf_rgbcw[2],buf_rgbcw[3],buf_rgbcw[4],buf_rgbcw[5]);



}

/******************************
模式设置处理
****************************/

void app_setmode_handler()
{
	   unsigned char i=0;  
	
	   if(Light_status.mode==1)
	   {   
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))	   
		   Light_status.RGBmode=0;
#endif
		   //color_temp_status.cool=100;
		   //color_temp_status.warm=0;
		   Light_status.Cw.cool = 255;
		   Light_status.Cw.warm = 0;
		   Light_status.brightness=100;
		   //Light_status.ratio=100;
		   light_turn_onoff_handler(1);
	   }
		else if(Light_status.mode==2)
		{	
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))
		    Light_status.RGBmode=0;
#endif
			Light_status.Cw.cool = 64;
		    Light_status.Cw.warm = 191;
			Light_status.brightness=20;
			//Light_status.ratio=25;
			light_turn_onoff_handler(1);
		}
		else if(Light_status.mode==3)
		{
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))
		Light_status.RGBmode=0;
#endif
			Light_status.Cw.cool = 170;
		    Light_status.Cw.warm = 85;  
			Light_status.brightness=60;
			//Light_status.ratio=67;	
			light_turn_onoff_handler(1);
			
		}
		else if(Light_status.mode==4)
		{ 
		    sleepmode_lighton_flag=1;
            sleep_delaytime=valuebufftemp[0]*60; 			
		}	
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))	
        else if(Light_status.mode==11||Light_status.mode==12||Light_status.mode==13||Light_status.mode==14)
		{ 
			Light_status.RGBmode=1;
			//rgb_status.r=valuebufftemp[0];
			//rgb_status.g=valuebufftemp[1];
			//rgb_status.b=valuebufftemp[2];
			Light_status.Rgb.r = valuebufftemp[0];
			Light_status.Rgb.g = valuebufftemp[1];
			Light_status.Rgb.b = valuebufftemp[2];
			//light_set_rgb2hsb(valuebufftemp[0],valuebufftemp[1],valuebufftemp[2]);
			// light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status); 
			light_turn_onoff_handler(1);
			printf("set mode to %d  r=%d g=%d  b=%d \n",Light_status.mode,Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
		}
		else if(Light_status.mode==15)
		{ 
		    Light_status.RGBmode=1;
		    RGB_changemode_flag=1;
		}	
#endif
        if(Light_status.mode!=4)
		{
			if(sleepmode_lighton_flag==1)
			{
				sleep_delaytime=0;
				sleepmode_lighton_flag=0;
			}
		}
#if(DEVICE_TYPE == RGB_LIGHT)
		if(Light_status.mode!=15)
		{
			if(RGB_changemode_flag==1)
			{
				RGB_changemode_flag=0;
			}
		}
#endif
 
}


/**************************************
ECHO调节色温处理
*************************************/
void echo_adjust_temp_handler(int data)
{

    unsigned char flag=0;  
	if(data==2200)
	{    
	    	flag=1;
		//color_temp_status.cool=10;
		//color_temp_status.warm=25;
		Light_status.Cw.cool=0;
		Light_status.Cw.warm=255;
		//color_temp_status.cool=0;		//cw、ww range 0~255
		//color_temp_status.warm=63;
		//Light_status.brightness=35;
		//Light_status.ratio=0;		
	}
	else if(data==2700)
	{
	    	flag=1;
		//color_temp_status.cool=20;
		//color_temp_status.warm=35;
		Light_status.Cw.cool=91;
		Light_status.Cw.warm=164;
		//color_temp_status.cool=40;
		//color_temp_status.warm=89;
		//Light_status.brightness=55;
		//Light_status.ratio=36;			
	}
	else if(data==4000)
	{
	    	flag=1;
		//color_temp_status.cool=50;
		//color_temp_status.warm=25;
		Light_status.Cw.cool=170;
		Light_status.Cw.warm=85;
		//color_temp_status.cool=128;
		//color_temp_status.warm=63;
		//Light_status.brightness=75;
		//Light_status.ratio=67;		
	}
	else if(data==5500)
	{
	    flag=1;
		//color_temp_status.cool=70;
		//color_temp_status.warm=30;
		Light_status.Cw.cool=204;
		Light_status.Cw.warm=51;
		//color_temp_status.cool=178;
		//color_temp_status.warm=76;
		//Light_status.brightness=100;
		//Light_status.ratio=80;	
	}
	else if(data==7000)
	{
	    flag=1;
		//color_temp_status.cool=100;
		Light_status.Cw.cool=255;
		Light_status.Cw.warm=0;
		//color_temp_status.cool=255;
		//color_temp_status.warm=0;
		//Light_status.brightness=100;
		//Light_status.ratio=100;		
	}

	if(flag==1)
    {
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))    
		Light_status.RGBmode=0;
#endif

	//color_temp_status.cool=BT2CW_C(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*Light_status.ratio)/100;
	//color_temp_status.warm=BT2CW_W(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*(100-Light_status.ratio))/100;
	printf("tian alex debug cw-ww:%d-%d\n",Light_status.Cw.cool,Light_status.Cw.warm);
		light_turn_onoff_handler(1);
	}


} 


int do_timer_task(int timer_id)
{
	int op_brightness;
	if(music_rgb_cw.music_flag == 0) {
		if(timer_id<USER_SET_TIMER_TASK_MAX){
			//operation=TmrLed_period[timer_id].oper&(~TIMER_ENABLE);
			op_brightness = TmrLed_period[timer_id].parm[5];
			if(op_brightness==LED_TURN_OFF){
				light_turn_onoff_handler(0);
			}
			else{
#if 0
			if(operation==LED_TURN_ON || operation==LED_TURN_SINGLE_ON){
				Light_status.brightness=TmrLed_period[timer_id].parm[3];
				HAP_LED_Brightness(Light_status.brightness);
			}
			else if(operation==LED_TURN_DOUBLE_ON){
				color_temp_status.cool=TmrLed_period[timer_id].parm[0];
				color_temp_status.warm=TmrLed_period[timer_id].parm[1];
				clear_setmode_handler();
#if(DEVICE_TYPE == RGB_LIGHT)			
				Light_status.RGBmode=0;
#endif
				Light_status.brightness=color_temp_status.cool+color_temp_status.warm;
	            		Light_status.ratio=color_temp_status.cool*100/Light_status.brightness;
				light_turn_onoff_handler(1);
			}
			else if(operation==LED_TURN_RGB_ON){
#if(DEVICE_TYPE == RGB_LIGHT)				
	            		clear_setmode_handler();			
				Light_status.RGBmode=1;	
				rgb_status.r=TmrLed_period[timer_id].parm[0]; 
				rgb_status.g=TmrLed_period[timer_id].parm[1];
				rgb_status.b=TmrLed_period[timer_id].parm[2];
				light_set_rgb2hsb(rgb_status.r,rgb_status.g,rgb_status.b);
				//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status); 
				light_turn_onoff_handler(1);
#endif	
			}
#endif

			if(op_brightness==LED_TURN_ON)
				HAP_LED_Brightness(Light_status.brightness);
			else{
				if((TmrLed_period[timer_id].parm[3]!=0)||(TmrLed_period[timer_id].parm[4]!=0))
				{
					clear_setmode_handler();
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))			
					Light_status.RGBmode=0;
#endif
					//Light_status.brightness=echo_cmd.cmd_par[2];			//have change because format is diffrent
		            //		Light_status.ratio=echo_cmd.cmd_par[3];
					//color_temp_status.cool=(echo_cmd.cmd_par[0]*echo_cmd.cmd_par[1])/100;
					//color_temp_status.warm=(echo_cmd.cmd_par[0]*(100-echo_cmd.cmd_par[1]))/100;
					//update_lightbulb_service(LIGHT_ONOFF);
					//update_lightbulb_service(LIGHT_BRIGHTNESS);
					//Light_status.brightness=((echo_cmd.cmd_par[2]+echo_cmd.cmd_par[3])*100/255);
					//Light_status.ratio=(TmrLed_period[timer_id].parm[3]*100/255);
					Light_status.brightness = TmrLed_period[timer_id].parm[5];
					Light_status.Cw.warm=TmrLed_period[timer_id].parm[4];
					Light_status.Cw.cool=TmrLed_period[timer_id].parm[3];
					//color_temp_status.cool=BT2CW_C(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*Light_status.ratio)/100;
					//color_temp_status.warm=BT2CW_W(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*(100-Light_status.ratio))/100;
					printf("fu debug timer cw-ww:%d-%d\n",Light_status.Cw.cool,Light_status.Cw.warm);
					light_turn_onoff_handler(1);
				}else if((TmrLed_period[timer_id].parm[0]!=0)||(TmrLed_period[timer_id].parm[1]!=0)||(TmrLed_period[timer_id].parm[2]!=0)){
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))				
			            clear_setmode_handler();			
						Light_status.RGBmode=1;	
						//rgb_status.r=echo_cmd.cmd_par[2]; 
						//rgb_status.g=echo_cmd.cmd_par[3];
						//rgb_status.b=echo_cmd.cmd_par[4];
						Light_status.Rgb.r=TmrLed_period[timer_id].parm[0];
						Light_status.Rgb.g=TmrLed_period[timer_id].parm[1];
						Light_status.Rgb.b=TmrLed_period[timer_id].parm[2];
						printf("timer set app rgb to %d-%d-%d\n",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
						//light_set_rgb2hsb(TmrLed_period[timer_id].parm[0],TmrLed_period[timer_id].parm[1],TmrLed_period[timer_id].parm[2]);
						Light_status.brightness = TmrLed_period[timer_id].parm[5];
						//hsb_status.b = Light_status.brightness;
						//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status); 			//convet has a litter errors with count from app
						light_turn_onoff_handler(1);
#endif
						}
						
				}
					
			}
		}
		else{
			printf("timer id too big %d\n",timer_id);
			return -1;
		}
	}
	return 0;
}


/************************************
创建ECHO回复消息
************************************/

void creat_echo_resp_data(uint8_t data)
{

	memset(send_echo_data_buff,0,sizeof(send_echo_data_buff));   
	memcpy(&send_echo_data_buff[2],&echo_cmd_tempbuff[2],6);

	if(data==1)    //onoff
	{
		send_echo_data_buff[8]=1;
		send_echo_data_buff[9]=Light_status.onoff;
		send_echo_data_len=10;			
	}
	else if(data==2)   //调光
	{
		send_echo_data_buff[8]=1;
		send_echo_data_buff[9]=Light_status.brightness;
		send_echo_data_len=10;				
	}
	else if(data==3)   //调色温
	{
		send_echo_data_buff[8]=1;
		memcpy(&send_echo_data_buff[9],&echo_cmd_tempbuff[8],2);
		send_echo_data_len=11;			
	}
	else if(data==4)   //RGB
	{	   
		send_echo_data_buff[8]=1;
		memcpy(&send_echo_data_buff[9],&echo_cmd_tempbuff[8],6);
		send_echo_data_len=15;		
	}
	else
	{
		send_echo_data_buff[8]=0;  
		send_echo_data_len=9;
	}
	send_echo_data_buff[0]=0;
	send_echo_data_buff[1]=send_echo_data_len-2;

}

 

/********************************************** ***/
/*APP 透传接口*/
/*************************************************/
unsigned char APP_DATA_PASS_THROUGH(char * cmd_str,unsigned short cmd_length,char *data_out,int data_length)
{
	unsigned char  cmd_temp=0;
	int i,ret=0;
	//int par_count=0;
	//A_INT8  control_result=0;
	unsigned char data_buff[64]={0};
	unsigned char uart_buff[64]={0};
	S_ECHO_PARA echo_cmd;
	uint32_t file_size=0;
	printf("cmd_str =%s,str_len =%d\n",cmd_str,cmd_length);
	ret = Str2Hex(data_buff, (unsigned char*)cmd_str, cmd_length);
	if(ret < 0){
		printf("Str2Hex err!\n",cmd_length);
		return 1;
	}
	uart_buff[0]=0xFA;
	if(ret>8 && (data_buff[2]&0x80)) //amazon echo cmd,length must bigger than 8 bytes
	{

        memset(echo_cmd_tempbuff,0,sizeof(echo_cmd_tempbuff));
		memcpy(echo_cmd_tempbuff,data_buff,ret);
	
		memcpy((char *)&echo_cmd,data_buff,ret);
		//echo_cmd.cmd_type=*(unsigned short *)&data_buff[2];
		//printf("\ntian debug echo cmd_type=%02x%02x\n",echo_cmd.cmd_type[0],echo_cmd.cmd_type[1]);
		//printf("tian debug echo device_type=%02x%02x\n",echo_cmd.device_type[0],echo_cmd.device_type[1]);
	  	if(music_rgb_cw.music_flag == 0) {
			printf("into not music control :%d\n",music_rgb_cw.music_flag);
			if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x01){  //on off cmd
		#if 0
			if(echo_cmd.device_type[0]==0x00 && echo_cmd.device_type[1]==0x01){//device socket
				uart_buff[1]=0x00;
				uart_buff[2]=0x0b;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x06;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x04;// cmd key,on off
				uart_buff[9]=echo_cmd.cmd_par[0];//on off
				uart_buff[10]=0x01;//单控
				uart_buff[11]=0x00;
				for(i=1;i<11;i++){
					uart_buff[11]+=uart_buff[i];
				}
				ret=11;
		       }
		       else if(echo_cmd.device_type[0]==0x00 && echo_cmd.device_type[1]==0x02){//device lamp 
				uart_buff[1]=0x00;
				uart_buff[2]=0x0b;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x06;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x04;// cmd key,on off
				uart_buff[9]=echo_cmd.cmd_par[0];//on off
				uart_buff[10]=0x01;//单控
				uart_buff[11]=0x00;
				for(i=1;i<11;i++){
					uart_buff[11]+=uart_buff[i];
				}
				ret=11;
		       }
			else if(echo_cmd.device_type[0]==0x00&& echo_cmd.device_type[1]==0x03)	{ //switch device
				uart_buff[1]=0x00;
				uart_buff[2]=0x0b;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x06;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x04;// cmd key,on off
				uart_buff[9]=0x01;//单路
				uart_buff[10]=echo_cmd.cmd_par[0];
				uart_buff[11]=0x00;
				for(i=1;i<11;i++){
					uart_buff[11]+=uart_buff[i];
				}
				ret=11;
			}
			#else								//dont done turn off light, Light_status.onoff should set 0,or 61 will open

			 if(echo_cmd.device_addr[0]==0&&echo_cmd.device_addr[1]==0)
			 {
				HAP_LED_ONOFF(echo_cmd.cmd_par[0]);							
				//update_lightbulb_service(LIGHT_ONOFF);
				if(echo_cmd.device_type[1] != 0x05)
				{
				echo_cmd_flag=1;
				creat_echo_resp_data(1);
				}
				else
					printf("app turn on/off\n");
			 }
#if 0
			 else
			 {
				uart_buff[1]=0x00;
				uart_buff[2]=0x0c;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x07;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x04;// cmd key,on off
				uart_buff[9]=echo_cmd.cmd_par[0];//on off
				uart_buff[10]=0x01;
				uart_buff[11]=0xFF;
				uart_buff[12]=0x00;
				for(i=1;i<12;i++){
				uart_buff[12]+=uart_buff[i];
				}
				ret=12;
			 }	
#endif
			#endif
		}
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x02){  //brightness
		       if(echo_cmd.device_type[0]==0x00/* && echo_cmd.device_type[1]==0x02*/){//device lamp 				
			    if((echo_cmd.device_addr[0]==0)&&(echo_cmd.device_addr[1]==0))
			   	{	               
					HAP_LED_Brightness(echo_cmd.cmd_par[0]);		//echo can entry but app rgb also can entry
					//update_lightbulb_service(LIGHT_ONOFF);
					//update_lightbulb_service(LIGHT_BRIGHTNESS);
					if(echo_cmd.device_type[1] != 0x05)
					{
					echo_cmd_flag=1;
					creat_echo_resp_data(2);
					}
					else
						printf("app command\n");
					
                 }
#if 0
				 else 
				 {

					uart_buff[1]=0x00;
					uart_buff[2]=0x0c;   //length
					uart_buff[3]=0x02;//cmd type
					uart_buff[4]=0x07;//data length
					uart_buff[5]=echo_cmd.device_addr[0];   //address high
					uart_buff[6]=echo_cmd.device_addr[1]; //address how
					uart_buff[7]=0x03;//control cmd
					uart_buff[8]=0x08;// cmd key,brightness
					uart_buff[9]=0x00;//色温值，0
					uart_buff[10]=echo_cmd.cmd_par[0];//色光值
					uart_buff[11]=0x01;//单控
					uart_buff[12]=0x00;
					for(i=1;i<12;i++){
						uart_buff[12]+=uart_buff[i];
					}
					ret=12;
				 	}
#endif
		       	}
			//   if(echo_cmd.device_type[0]==0x00 && echo_cmd.device_type[1]==0x05)	//set rgb light brightness
			//		HAP_LED_Brightness(echo_cmd.cmd_par[0]);	//just test brigtness to rgb or cw/ww
			  
		}
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x03){  //brightness increase
		      // if(echo_cmd.device_type[0]==0x00 && echo_cmd.device_type[1]==0x02){//device lamp 
				Light_status.brightness +=echo_cmd.cmd_par[0];
			  	if(Light_status.brightness > 100)
					Light_status.brightness = 100;
				HAP_LED_Brightness(Light_status.brightness);
				//light_turn_onoff_handler(1);

				//update_lightbulb_service(LIGHT_ONOFF);
				//update_lightbulb_service(LIGHT_BRIGHTNESS);
				echo_cmd_flag=1;
				creat_echo_resp_data(2);
		}
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x04){  //brightness decrease
			   if(Light_status.brightness < echo_cmd.cmd_par[0])
			   		Light_status.brightness = echo_cmd.cmd_par[0];
			   Light_status.brightness -=echo_cmd.cmd_par[0];
		
			   HAP_LED_Brightness(Light_status.brightness);
			   //light_turn_onoff_handler(1);

			   //update_lightbulb_service(LIGHT_ONOFF);
			   //update_lightbulb_service(LIGHT_BRIGHTNESS);
			   echo_cmd_flag=1;
			   creat_echo_resp_data(2);
		}
#if 0
		else if(echo_cmd.cmd_type[0]==0x81&& echo_cmd.cmd_type[1]==0x00){  //color temperature cmd from jingxun app
			printf("set color temperature to %d-%d\n",echo_cmd.cmd_par[0],echo_cmd.cmd_par[1]);

			clear_setmode_handler();
#if(DEVICE_TYPE == RGB_LIGHT)			
			Light_status.RGBmode=0;
#endif
			Light_status.brightness=echo_cmd.cmd_par[0];			//406-01 old version
            		Light_status.ratio=echo_cmd.cmd_par[1];
			color_temp_status.cool=(echo_cmd.cmd_par[0]*echo_cmd.cmd_par[1])/100;
			color_temp_status.warm=(echo_cmd.cmd_par[0]*(100-echo_cmd.cmd_par[1]))/100;
			//update_lightbulb_service(LIGHT_ONOFF);
			//update_lightbulb_service(LIGHT_BRIGHTNESS);

			light_turn_onoff_handler(1);
            			
		}
		else if(echo_cmd.cmd_type[0]==0x81&& echo_cmd.cmd_type[1]==0x01){  //rgb cmd from jingxun app
			printf("set rgb to %d-%d-%d\n",echo_cmd.cmd_par[0],echo_cmd.cmd_par[1],echo_cmd.cmd_par[2]);
#if(DEVICE_TYPE == RGB_LIGHT)				
            		clear_setmode_handler();			
			Light_status.RGBmode=1;	
			rgb_status.r=echo_cmd.cmd_par[0]; 
			rgb_status.g=echo_cmd.cmd_par[1];
			rgb_status.b=echo_cmd.cmd_par[2];
			//Light_status.Rgb.r=echo_cmd.cmd_par[0];
			//Light_status.Rgb.g=echo_cmd.cmd_par[1];
			//Light_status.Rgb.b=echo_cmd.cmd_par[2];
			light_set_rgb2hsb(echo_cmd.cmd_par[0],echo_cmd.cmd_par[1],echo_cmd.cmd_par[2]);		//fu change keep brightness the same
			//hsb_status.b = Light_status.brightness;		//fu change for stay brightness the same
			//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status); 		//fu change it
			light_turn_onoff_handler(1);
#endif				
			
		}
		else if(echo_cmd.cmd_type[0]==0x81&& echo_cmd.cmd_type[1]==0x02){  //mode cmd from jingxun app
			Light_status.mode=echo_cmd.cmd_par[0];
			memset(valuebufftemp,0,3);
			memcpy(valuebufftemp,&(echo_cmd.cmd_par[1]),3);
			printf("set mode to %d\n",echo_cmd.cmd_par[0]);
	        	app_setmode_handler();
			//update_lightbulb_service(LIGHT_ONOFF);
			//update_lightbulb_service(LIGHT_BRIGHTNESS);
 
		}
#endif
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x05){  ////color temperature cmd from alexa
		     if(echo_cmd.device_type[0]==0x00/* && echo_cmd.device_type[1]==0x02*/){//device lamp 

			if((echo_cmd.device_addr[0]==0)&&(echo_cmd.device_addr[1]==0))
			{
				int tempdata=echo_cmd.cmd_par[0]*256+echo_cmd.cmd_par[1];
				printf("tian debug temp=%d\n",tempdata);
				clear_setmode_handler();
				echo_adjust_temp_handler(tempdata);
				echo_cmd_flag=1;
				creat_echo_resp_data(3);
				//update_lightbulb_service(LIGHT_ONOFF);
				//update_lightbulb_service(LIGHT_BRIGHTNESS);
			}
#if 0
			else
			{
				  uart_buff[1]=0x00;
				  uart_buff[2]=0x0c;   //length
				  uart_buff[3]=0x02;//cmd type
				  uart_buff[4]=0x07;//data length
				  uart_buff[5]=echo_cmd.device_addr[0];   //address high
				  uart_buff[6]=echo_cmd.device_addr[1]; //address how
				  uart_buff[7]=0x03;//control cmd
				  uart_buff[8]=0x07;// cmd key,brightness
				  uart_buff[9]=echo_cmd.cmd_par[0];//色温值，0
				  uart_buff[10]=echo_cmd.cmd_par[1];//色温值
				  uart_buff[11]=0x01;//单控
				  uart_buff[12]=0x00;
				  for(i=1;i<12;i++){
				  uart_buff[12]+=uart_buff[i];
			  }
			  ret=12;							  
			 }
#endif
			
		       }
		}	
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x08){  //alexa colour,hsb mode
			if(echo_cmd.device_type[0]==0x00 /*&& echo_cmd.device_type[1]==0x05*/){//device lamp 
				HSVset.h = echo_cmd.cmd_par[0]*256+echo_cmd.cmd_par[1];
				HSVset.s =(unsigned char)(((echo_cmd.cmd_par[2]*256+echo_cmd.cmd_par[3])*255)/1000);
				HSVset.v =(unsigned char)(((echo_cmd.cmd_par[4]*256+echo_cmd.cmd_par[5])*255)/1000);
				printf("tian alexa hsb:%d-%d-%d\n",HSVset.h, HSVset.s,HSVset.v);
				//HsvToRgb(HSVset, &RGBset);
				//printf("fu alexa hsb->rgb:%d-%d-%d\n",RGBset.r,RGBset.g,RGBset.b);
				if(echo_cmd.device_addr[0]==0&&echo_cmd.device_addr[1]==0)
				{
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))				
				clear_setmode_handler();
				// change_rgb_value_to_max(RGBset.r,RGBset.g,RGBset.b); 

				/*
				rgb_status.r=RGBset.r; 
				rgb_status.g=RGBset.g;
				rgb_status.b=RGBset.b;
				*/
				//Light_status.Rgb.r = RGBset.r;
				//Light_status.Rgb.g = RGBset.g;
				//Light_status.Rgb.b = RGBset.b;
				
				Light_status.RGBmode=1;
				hsb_status.h = HSVset.h;
				hsb_status.s = HSVset.s*100/255;
				hsb_status.b = HSVset.v*100/255;
				//Light_status.hue = hsb_status.h;
				//Light_status.saturation = hsb_status.s;			
				//light_set_rgb2hsb(RGBset.r,RGBset.g,RGBset.b);
				light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&Light_status.Rgb);
				printf("fu alexa hsb->rgb:%d-%d-%d\n",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
				
				//hsb_status.b = Light_status.brightness;
				//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status);
				light_turn_onoff_handler(1);
				echo_cmd_flag=1;
				creat_echo_resp_data(4);	
				//update_lightbulb_service(LIGHT_ONOFF);
				//update_lightbulb_service(LIGHT_BRIGHTNESS);
#endif					
				}
#if 0
				else
				{
				uart_buff[1]=0x00;
				uart_buff[2]=0x0E;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x09;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x08;// cmd key,brightness
				uart_buff[9]=0x01;//
				uart_buff[10]=RGBset.r;//H
				uart_buff[11]=RGBset.g;//H
				uart_buff[12]=RGBset.b;//S

				uart_buff[13]=0x01;
				for(i=1;i<14;i++){
				uart_buff[14]+=uart_buff[i];
				}
				ret=14;
				}
#endif

			}
		}
		else if(echo_cmd.cmd_type[0]==0x81&& echo_cmd.cmd_type[1]==0x0a){
			memcpy(uart_buff,echo_cmd.cmd_par,4);//copy file length
			file_size=echo_cmd.cmd_par[0]*0x1000000+echo_cmd.cmd_par[1]*0x10000+echo_cmd.cmd_par[2]*0x100+echo_cmd.cmd_par[3];
			ota_started=1;
			//update_ota_local_jingxun(file_size,0,ota_socket);
			update_ota_local_jingxun(file_size,0,ota_socket);
			sprintf(data_out,"%d%d",0,0);
		}
#if 0
		else if(echo_cmd.cmd_type[0]==0x81&& echo_cmd.cmd_type[1]==0x03){
			ret=TmrLed_recvdata(echo_cmd.cmd_par,data_out,data_length);			//因为定时任务要返回数据，所以调用的需要改动
			return ret;
		}
#endif
		else if(echo_cmd.cmd_type[0]==0x82&& echo_cmd.cmd_type[1]==0x00){		//local app send data accord mesh format
			if(echo_cmd.cmd_par[0]==0xd0)							//what op
				HAP_LED_ONOFF(echo_cmd.cmd_par[1]);
			else if(echo_cmd.cmd_par[0]==0xe2){

				if((echo_cmd.cmd_par[1]==0x00)||(echo_cmd.cmd_par[1]==0x09)){
					if((echo_cmd.cmd_par[5]==0)&&(echo_cmd.cmd_par[6]==0))
					{
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))				
							clear_setmode_handler();			
							Light_status.RGBmode=1; 
							Light_status.Rgb.r=echo_cmd.cmd_par[2];
							Light_status.Rgb.g=echo_cmd.cmd_par[3];
							Light_status.Rgb.b=echo_cmd.cmd_par[4];
							if(echo_cmd.cmd_par[1]==0x00)
							{
								Light_status.brightness=echo_cmd.cmd_par[7];
							}
							printf("get app rgb to %d-%d-%d\n",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
							//light_set_rgb2hsb(echo_cmd.cmd_par[2],echo_cmd.cmd_par[3],echo_cmd.cmd_par[4]);
							//hsb_status.b = Light_status.brightness;
							//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status);	//convet has a litter errors with count from app
							//rgb_status.r = Light_status.Rgb.r*Light_status.brightness/100;					//when low light hsb2rgb has a little deviation than divide light
							//rgb_status.g = Light_status.Rgb.g*Light_status.brightness/100;
							//rgb_status.b = Light_status.Rgb.b*Light_status.brightness/100;
							light_turn_onoff_handler(1);
#endif	

					}
					else
					{
						clear_setmode_handler();
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))			
						Light_status.RGBmode=0;
#endif
						//Light_status.ratio=(echo_cmd.cmd_par[5]*100/255);
						Light_status.Cw.warm=echo_cmd.cmd_par[6];
						Light_status.Cw.cool=echo_cmd.cmd_par[5];
						if(echo_cmd.cmd_par[1]==0x00)
						{
							Light_status.brightness=echo_cmd.cmd_par[7];
						}
						//color_temp_status.cool=BT2CW_C(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*Light_status.ratio)/100;
						//color_temp_status.warm=BT2CW_W(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*(100-Light_status.ratio))/100;
						printf("fu debug cw-ww:%d-%d\n",Light_status.Cw.cool,Light_status.Cw.warm);
						light_turn_onoff_handler(1);
					}
						
				}
				else if((echo_cmd.cmd_par[1]==0x01)||(echo_cmd.cmd_par[1]==0x02)||(echo_cmd.cmd_par[1]==0x03)){
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))				
							clear_setmode_handler();			
							Light_status.RGBmode=1;
							if(echo_cmd.cmd_par[1]==0x01)
							{
								Light_status.Rgb.r=echo_cmd.cmd_par[2];
								Light_status.Rgb.g=0;
								Light_status.Rgb.b=0;
							}
							else if(echo_cmd.cmd_par[1]==0x02){
								Light_status.Rgb.r=0;
								Light_status.Rgb.g=echo_cmd.cmd_par[2];
								Light_status.Rgb.b=0;
							}
							else if(echo_cmd.cmd_par[1]==0x03){
								Light_status.Rgb.r=0;
								Light_status.Rgb.g=0;
								Light_status.Rgb.b=echo_cmd.cmd_par[2];
							}
							printf("get app rgb to %d-%d-%d\n",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
							//light_set_rgb2hsb(Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
							//hsb_status.b = Light_status.brightness;
							//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status);			//convet has a litter errors with count from app
							//rgb_status.r = Light_status.Rgb.r*Light_status.brightness/100;					//when low light hsb2rgb has a little deviation than divide light
							//rgb_status.g = Light_status.Rgb.g*Light_status.brightness/100;
							//rgb_status.b = Light_status.Rgb.b*Light_status.brightness/100;
							light_turn_onoff_handler(1);
#endif							
				}
				else if((echo_cmd.cmd_par[1]==0x04)||(echo_cmd.cmd_par[1]==0x08)){
					printf("set rgb to %d-%d-%d\n",echo_cmd.cmd_par[2],echo_cmd.cmd_par[3],echo_cmd.cmd_par[4]);
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))				
			            clear_setmode_handler();			
						Light_status.RGBmode=1;	
						//rgb_status.r=echo_cmd.cmd_par[2]; 
						//rgb_status.g=echo_cmd.cmd_par[3];
						//rgb_status.b=echo_cmd.cmd_par[4];
						Light_status.Rgb.r=echo_cmd.cmd_par[2];
						Light_status.Rgb.g=echo_cmd.cmd_par[3];
						Light_status.Rgb.b=echo_cmd.cmd_par[4];
						if(echo_cmd.cmd_par[1]==0x08)
						{
							Light_status.brightness=echo_cmd.cmd_par[5];
						}
						printf("get app rgb to %d-%d-%d\n",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
						//light_set_rgb2hsb(echo_cmd.cmd_par[2],echo_cmd.cmd_par[3],echo_cmd.cmd_par[4]);
						//hsb_status.b = Light_status.brightness;
						//light_set_hsb2rgb(hsb_status.h,hsb_status.s,hsb_status.b,&rgb_status); 			//convet has a litter errors with count from app
						//rgb_status.r = Light_status.Rgb.r*Light_status.brightness/100;					//when low light hsb2rgb has a little deviation than divide light
						//rgb_status.g = Light_status.Rgb.g*Light_status.brightness/100;
						//rgb_status.b = Light_status.Rgb.b*Light_status.brightness/100;
						light_turn_onoff_handler(1);
#endif	
				}
				else if(echo_cmd.cmd_par[1]==0x05){
					HAP_LED_Brightness(echo_cmd.cmd_par[2]);
				}
				else if((echo_cmd.cmd_par[1]==0x06)||(echo_cmd.cmd_par[1]==0x07)){
					printf("set color temperature to cw-ww = %d-%d\n",echo_cmd.cmd_par[2],echo_cmd.cmd_par[3]);

					clear_setmode_handler();
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))			
					Light_status.RGBmode=0;
#endif
					//Light_status.brightness=echo_cmd.cmd_par[2];			//have change because format is diffrent
		            //		Light_status.ratio=echo_cmd.cmd_par[3];
					//color_temp_status.cool=(echo_cmd.cmd_par[0]*echo_cmd.cmd_par[1])/100;
					//color_temp_status.warm=(echo_cmd.cmd_par[0]*(100-echo_cmd.cmd_par[1]))/100;
					//update_lightbulb_service(LIGHT_ONOFF);
					//update_lightbulb_service(LIGHT_BRIGHTNESS);
					//Light_status.brightness=((echo_cmd.cmd_par[2]+echo_cmd.cmd_par[3])*100/255);
					//Light_status.ratio=(echo_cmd.cmd_par[2]*100/255);
					Light_status.Cw.warm=echo_cmd.cmd_par[3];
					Light_status.Cw.cool=echo_cmd.cmd_par[2];
					if(echo_cmd.cmd_par[1]==0x07)
					{
						Light_status.brightness=echo_cmd.cmd_par[5];
					}
					//color_temp_status.cool=BT2CW_C(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*Light_status.ratio)/100;
					//color_temp_status.warm=BT2CW_W(Light_status.brightness,Light_status.ratio);//(Light_status.brightness*(100-Light_status.ratio))/100;
					printf("fu debug cw-ww:%d-%d\n",Light_status.Cw.cool,Light_status.Cw.warm);
					light_turn_onoff_handler(1);
				}
			}
			else if((echo_cmd.cmd_par[0]==0xe5)||(echo_cmd.cmd_par[0]==0xe6)){
						ret=TmrLed_recvdata(echo_cmd.cmd_par,data_out,data_length);
						return ret;
			}
			else if(echo_cmd.cmd_par[0]==0xf8){
					printf("\r\nset mode\n");
					
#if(DEVICE_TYPE == RGB_LIGHT)
					memset(&special_control,0,sizeof(MODE_COLOR));
					if(echo_cmd.cmd_par[1]==0x0b)
					{
						special_control.mode_flag = 1;
						special_control.mode_count = 3;
						special_control.mode_index[0] = echo_cmd.cmd_par[3];
						special_control.mode_index[1] = echo_cmd.cmd_par[5];
						special_control.mode_index[2] = echo_cmd.cmd_par[4];
						//Light_status.Rgb.r = rgb_map[echo_cmd.cmd_par[3]][0];
						//Light_status.Rgb.g = rgb_map[echo_cmd.cmd_par[3]][1];
						//Light_status.Rgb.b = rgb_map[echo_cmd.cmd_par[3]][2];
						//light_set_rgb2hsb(Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
					}
					else if(echo_cmd.cmd_par[1]==0x0c){
						special_control.mode_flag = 1;
						special_control.mode_count = 2;
						special_control.mode_index[0] = echo_cmd.cmd_par[3];
						special_control.mode_index[1] = echo_cmd.cmd_par[4];
						//Light_status.Rgb.r = rgb_map[echo_cmd.cmd_par[3]][0];
						//Light_status.Rgb.g = rgb_map[echo_cmd.cmd_par[3]][1];
						//Light_status.Rgb.b = rgb_map[echo_cmd.cmd_par[3]][2];
						//light_set_rgb2hsb(Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
					}
					//Light_status.mode = 1;		//mode regard as special rgb control 	,this is different under mode				
					//Light_status.RGBmode=1;
		    		RGB_changemode_flag=1;
#endif

			}
			else if(echo_cmd.cmd_par[0]==0xa1) {
					printf("\r\nupdata data\n");
					//yield_time = 3000;
					//yield_time_flag = 1;
			}
			
		  }
	  	}

		if(echo_cmd.cmd_type[0]==0x82&& echo_cmd.cmd_type[1]==0x00){
			if(echo_cmd.cmd_par[0]==0xd2) {
						if(echo_cmd.cmd_par[1]==0xfe) {
							printf("start music rgb\n");
						memset(&music_rgb_cw,0,sizeof(MUSIC_PARA));
						music_rgb_cw.music_flag = 1;
					}else if(echo_cmd.cmd_par[1]==0xff){
						printf("end music rgb\n");
						memset(&music_rgb_cw,0,sizeof(MUSIC_PARA));
						if(Light_status.onoff == 0)
							light_turn_onoff_handler(0);
						else
							light_turn_onoff_handler(1);						
					}else {
						//if(music_rgb_cw.music_flag == 1)
						//{
							if(music_rgb_cw.music_flag == 0)
								music_rgb_cw.music_flag = 1;
							music_rgb_cw.music_timeout = 2;
							if(echo_cmd.cmd_par[1] <0 || echo_cmd.cmd_par[1] >100){
								printf("brightness value error,must 1-100,but %d\n",echo_cmd.cmd_par[1]);
								return 1;
							}
							music_rgb_cw.music_brightness = echo_cmd.cmd_par[1];
#if((DEVICE_TYPE == RGB_LIGHT) || (DEVICE_TYPE == RGBW_LIGHT))
							
							music_rgb_cw.music_rgb.r = echo_cmd.cmd_par[2];
							music_rgb_cw.music_rgb.g = echo_cmd.cmd_par[3];
							music_rgb_cw.music_rgb.b = echo_cmd.cmd_par[4];
#else
							//music_cw.cool=BT2CW_C(echo_cmd.cmd_par[1],Light_status.ratio);//(Light_status.brightness*Light_status.ratio)/100;
							//music_cw.warm=BT2CW_W(echo_cmd.cmd_par[1],Light_status.ratio);//(Light_status.brightness*(100-Light_status.ratio))/100;
							//music_rgb_cw.music_cw.cool = Light_status.Cw.cool;
							//printf("tian debug cw-ww:%d-%d\n",music_cw.cool,music_cw.warm);
#endif
							light_turn_music_handler();
						//}					
					}
			}
		}
		/*
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x03){  //brightness increase
		       if(echo_cmd.device_type[0]==0x00 && echo_cmd.device_type[1]==0x02){//device lamp 
				uart_buff[1]=0x00;
				uart_buff[2]=0x0b;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x06;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x04;// cmd key,on off
				uart_buff[9]=echo_cmd.cmd_par[0];//on off
				uart_buff[10]=0x01;//单控
				uart_buff[11]=0x00;
				for(i=1;i<11;i++){
					uart_buff[11]+=uart_buff[i];
				}
				ret=11;
		       }
		}
		else if(echo_cmd.cmd_type[0]==0x80&& echo_cmd.cmd_type[1]==0x02){  //brightness decrease
		       if(echo_cmd.device_type[0]==0x00 && echo_cmd.device_type[1]==0x02){//device lamp 
				uart_buff[1]=0x00;
				uart_buff[2]=0x0b;   //length
				uart_buff[3]=0x02;//cmd type
				uart_buff[4]=0x06;//data length
				uart_buff[5]=echo_cmd.device_addr[0];   //address high
				uart_buff[6]=echo_cmd.device_addr[1]; //address how
				uart_buff[7]=0x03;//control cmd
				uart_buff[8]=0x04;// cmd key,on off
				uart_buff[9]=echo_cmd.cmd_par[0];//on off
				uart_buff[10]=0x01;//单控
				uart_buff[11]=0x00;
				for(i=1;i<11;i++){
					uart_buff[11]+=uart_buff[i];
				}
				ret=11;
		       }
		}
		*/
	}
	else{
		for(i=0;i<(ret-1);i++){
			//A_PRINTF("%02x  ",data_buff[i]);
			cmd_temp+=data_buff[i];
			uart_buff[i+1]=data_buff[i];
		}
		//A_PRINTF("%02x  char_len =%d  ......\n",data_buff[i],ret);
		if(cmd_temp != data_buff[ret-1])
		{
			printf("check sum err! should =%02x, but =%02x\n",cmd_temp,data_buff[ret-1]);
			return 1;
		}
		uart_buff[ret]=data_buff[ret-1];
	}
	#if 0
	//uart_cmd_create();
	i=Write_Cmd2Buf((char*)uart_buff, ret+1);
	if(i<0){
		A_PRINTF("Write_Cmd2Buf error ret=%d\n",i);
		return 1;
	}
	Send_Cmd_To_UART();
	#endif
	return 0;

}
#if REMOTE_SERVER==JINGXUN_CLOUND
/*************************************************/
/*远程透传*/
/*************************************************/
int WIFI_LIAN_TRANSFER(char *recv,int recv_length)
{
	unsigned short recv_tag;
	unsigned short tlv_lenth;

	unsigned char data2[256];
	//char   *heaterIndex = NULL;
	int data_length_temp=0,i=0;
	memset(data2,0,sizeof(data2));
	data_length_temp=recv_length-TCP_Head_Len;
	while(data_length_temp>=4){
	
		recv_tag =(unsigned short)(recv[TCP_Head_Len+i]<<8)+(unsigned short)(recv[TCP_Head_Len+i+1]);
		i=i+2;
		tlv_lenth = (unsigned short)(recv[TCP_Head_Len+i]<<8)+(unsigned short)(recv[TCP_Head_Len+i+1]);
		i=i+2;
		printf("WIFI_HEATER_TRANSFER recv_tag=0x%04x  tlv_lenth=0x%04x\n",recv_tag,tlv_lenth);
		
		if((PhoneID==recv_tag) && (tlv_lenth<=sizeof(Phone_ID_buf))){
				Phone_ID_length=tlv_lenth;
				memset(Phone_ID_buf,0x0,sizeof(Phone_ID_buf));
				memcpy(Phone_ID_buf,recv+TCP_Head_Len+i,tlv_lenth);
		}
		else if((recv_tag == heater_transfer))//&&(tlv_lenth<20))
		{
				//Remote_ACK_Wait=1;
				//UART_data_recived_remote=0;
				APP_DATA_PASS_THROUGH(recv+TCP_Head_Len+i,tlv_lenth,data2,sizeof(data2));
				#if 1
				m_Send_Message_Body(LIAN_TRANSFER_ACK,data2);
				#else
				while(UART_data_recived_remote==0){
					qcom_thread_msleep(100);
					i++;
					//if(i>31){
					if(i>22){
						A_PRINTF("\n~~~Remote wait UART ACK timeout~~~~\n");
						break;
					}
				}
				Remote_ACK_Wait=0;
				UART_data_recived=0;
				if(UART_data_recived_remote){
					client_para.Result = 0x00;
					m_Send_Message_Body(LIAN_TRANSFER_ACK,0);
					UART_data_recived_remote=0;
				}
				else{
					client_para.Result = 0x01;
					m_Send_Message_Body(LIAN_TRANSFER_ACK,0);
				}
				#endif
		}
		else{
			printf("unknown tag %04x\n",recv_tag);
		}
		i+=tlv_lenth;
		data_length_temp -=(4+tlv_lenth);
	}
	return i;
}
int WIFI_LIAN_TRANSFER_ACK(char *senddata,char * data_in)
{
	unsigned short i,j;
	unsigned short msg_lenth;
	//unsigned char data3[20]={0};
	char body[200];

	memset(body,0,sizeof(body));
	//A_PRINTF("hu debug client_para.Result = %d\n",client_para.Result);
	//if(client_para.Result == 0x00)
	//{
	if(echo_cmd_flag==1)
	{
	    echo_cmd_flag=0;
		Hex2Str((unsigned char*)body, (unsigned char*)send_echo_data_buff, send_echo_data_len);
	}
	else if((data_in!=NULL) && (strlen(data_in)>0))
	{
		strcpy(body,data_in);
	}
	else
    {
		//A_STRCPY(body,"timeout!");
		strcpy(body,"0000"); 
	}
	//A_PRINTF("tian debug data=%s\n",body);
	m_ADD_Message_Head(senddata,LIAN_TRANSFER_ACK);
	i=TCP_Head_Len;
	m_ADD_Message_SmaterID(&senddata[i]);
	i+=6;
	*(unsigned short *)(senddata+i)=htons(heater_transfer_ack);
	i+=2;
	*(unsigned short *)(senddata+i)=htons(strlen(body));
	i+=2;
	for(j=0;j<strlen(body);j++)
	{
		senddata[i++] = body[j];
	}
	//m_ADD_Message_IphoneID(&senddata[i]);
	//i=(i+4+Phone_ID_length);
	//Phone_ID_length =0;


#if  0
	//senddata[i++]=0x00;

	if(Phone_ID_length>0){
		*(unsigned short *)(senddata+i)=htons(PhoneID);
		i+=2;
		*(unsigned short *)(senddata+i)=htons(Phone_ID_length);
		i+=2;
		for(j=0;j<Phone_ID_length;j++)
		{
			senddata[i++] = Phone_ID_buf[j];
		}
		Phone_ID_length=0;
	}
#endif

	senddata[i++] = 0;//client_para.Result;
	
	*(unsigned short *)(senddata)=htons(i);
	msg_lenth = i;
	return msg_lenth;
}
int WIFI_GET_ONOFF_STATUS_ACK(char *senddata)
{
	unsigned short i,j;
	unsigned short msg_lenth;

	unsigned char data[40];

	//A_STRCPY(data1,"060085020081");
	memset(data,0x0,sizeof(data));
#if 0
		sprintf(data,"%d-%d-%d-%d",Light_status.mode,Light_status.onoff,Light_status.brightness,Light_status.ratio);
		sprintf(data+strlen(data),"-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
#else
		sprintf(data+strlen(data), "00-%02d-00",DEVICE_TYPE);//#Dev_basic_para.Dev_Version#data1#
		if(Light_status.onoff == 0)
			sprintf(data+strlen(data), "-%d-%d-%d-%d",Light_status.Cw.cool,Light_status.Cw.warm,0,Light_status.onoff);
		else
			sprintf(data+strlen(data), "-%d-%d-%d-%d",Light_status.Cw.cool,Light_status.Cw.warm,Light_status.brightness,Light_status.onoff);
		sprintf(data+strlen(data), "-%d-%d-%d",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
#if 0
		if(Light_status.RGBmode)
		{
			sprintf(data+strlen(data), "-%d-%d-%d-%d",0,0,Light_status.brightness,Light_status.onoff);
			sprintf(data+strlen(data), "-%d-%d-%d",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
		}
		else{
			sprintf(data+strlen(data), "-%d-%d-%d-%d",color_temp_status.cool,color_temp_status.warm,Light_status.brightness,Light_status.onoff);
			sprintf(data+strlen(data), "-%d-%d-%d",0,0,0);
		}
#endif
#endif

	m_ADD_Message_Head(senddata,HEATER_GET_ACK);
	senddata[TCP_Head_Len]=0x00;//client_para.Result;  //get parameters and status back result

	i=TCP_Head_Len+1;
	*(unsigned short *)(senddata+i)=htons(heater_get);
	i+=2;
	*(unsigned short *)(senddata+i)=htons(strlen(data));
	i+=2;
	for(j=0;j<strlen(data);j++)
	{
		senddata[i++] = data[j];
	}
	*(unsigned short *)(senddata)=htons(i);
	return i;
}
int m_Send_Message_Body(unsigned short cmd,char *data_ptr)
{
	unsigned short i,lenth=0;
	unsigned char   *TCP_Send_Msg_Body = NULL;
    	int ret=0;

	if(remote_service_par.remote_socket_fd<0){
		printf("remote_socket_fd=-1,send message fail\n");
		return -1;
	}
	//(uint8_t *) malloc(FLASH_DATA_LEN);
	TCP_Send_Msg_Body = (unsigned char *)malloc(Client_Max_Sendlen);
	if(TCP_Send_Msg_Body == NULL)
	{
		printf("%s: alloc buf failed. \n",__func__);
		return -1;
	}
	memset(TCP_Send_Msg_Body, 0x0, Client_Max_Sendlen);
	switch(cmd)
	{
		case T_REGISTER:
			lenth=WIFI_REGISTER(TCP_Send_Msg_Body);
			break; 
		case HEART_BEAT:
			lenth=WIFI_HEART_BEAT(TCP_Send_Msg_Body);
			//heartbeat_count=1;
			break;
		case LIAN_TRANSFER_ACK: //0x802f
			lenth=WIFI_LIAN_TRANSFER_ACK(TCP_Send_Msg_Body,data_ptr);
			break;
		case  HEATER_GET_ACK:
			lenth=WIFI_GET_ONOFF_STATUS_ACK(TCP_Send_Msg_Body);
		default:
			break;
	}
	ret = write(remote_service_par.remote_socket_fd,TCP_Send_Msg_Body,lenth);
	//lwip_write(int s, const void * data, size_t size)
	if(ret >= 0){
		printf("remote Send %d:",lenth);
       	 for(i=0;i<lenth;i++){
    			printf("%02x",TCP_Send_Msg_Body[i]);
    	 }
	printf("  ret=%d\n",ret);
	//A_PRINTF("command send=0x%04x,length=%d\n",cmd,lenth);
	}
	free(TCP_Send_Msg_Body);
	return 0;
} 
/************************************************************************
*Description:TCP客户端接收服务器数据
*          
*Author:zq   Date: 2014-10-14
*
************************************************************************/
//接收报文并解析
int m_RecvData_remote(void)
{
	S_TCP_HEAD recv_head;
	unsigned short recv_cmd=0;
	int ret,pos,command_length;
	unsigned short i;
	char   *QueryServerIPIndex  = NULL;
	unsigned char *process_ptr;
	char length_buf[4];
	
	memset(recvBuff,0,sizeof(recvBuff));

	ret=read(remote_service_par.remote_socket_fd, recvBuff, sizeof(recvBuff));
	if(ret<=0)
	{
		printf("WIFI_RX_Data remote  error,ret=%d,\n",ret);
		return -1;
	}
	else if(ret<sizeof(recvBuff))
	{
          	//Debug WIFI_RX_Data receive  49 bytes data
		//A_PRINTF("Debug WIFI_RX_Data receive  %d bytes data\n",ret);
		//{"server_id":"54.223.249.109","port":"6000"} 
		
		if(remote_service_par.remote_status== REMOTE_SERVER_QUERY)
		{
			if((recvBuff[0] == 0x7B)&&(strlen(recvBuff) >= 38))  //0X7B:字符 '{'
			{
				QueryServerIPIndex = strstr(recvBuff,"server_id");
				if(QueryServerIPIndex != NULL)
				{
					decode_query_response(recvBuff);
					return  1;
				}
			}
		}
		#if 1
		printf("remote recv : ----time %d:%d:%d\n",sys_tm.tm_hour,sys_tm.tm_min,sys_tm.tm_sec);		//test
		printf("\nremote Receive %d:",ret);
		for(i=0;i<ret;i++)
		{
			printf("%02x",recvBuff[i]);       
		}
		printf("\n");
		#endif
		pos=0;
		command_length = ntohs(*(unsigned short *)(recvBuff+pos));
		while((ret-pos) >= command_length)   //command length
		{
			if((ret-pos)<TCP_Head_Len)
			{
				printf("length too short,may be not a right packet,command_length=%d\n",command_length);
				return 1;
			}
			//A_PRINTF("pos=%d\n",pos);
			//A_PRINTF("command_length=%d\n",command_length);
			process_ptr=recvBuff+pos;
			memset(&recv_head,0,sizeof(recv_head));
			memcpy((unsigned char *)&recv_head,process_ptr,sizeof(recv_head));   //获取头
			recv_cmd=ntohs(recv_head.cmd);                                         //获取命令
			printf("command=0x%04x,length=%d\n",recv_cmd,command_length);			
			if(recv_cmd & 0x8000) 
			{    //server ACK
				switch(recv_cmd)
				{
					case T_REGISTER_ACK: //0x8001
						WIFI_REGISTER_ACK(process_ptr);
						break;
					case HEART_BEAT_ACK: //0x8002
						printf("HEART_BEAT_ACK !\n");	
						//heartbeat_count=0;
						break;	
				}
			}
			else 
			{ //server send cmd
				switch(recv_cmd)
				{
					case SOFT_UPDATA: //0x0008										  //软件升级
						WIFI_SOFT_UPDATA(process_ptr);
					    break;				 
					case LIAN_TRANSFER: //0x002f 远程透传接口 
						phone_heart_term_count = 0;
						phone_heart_term_flag = 1;
						WIFI_LIAN_TRANSFER(process_ptr,command_length);
						break;
					case HEATER_GET:	
						m_Send_Message_Body(HEATER_GET_ACK,0);
						break;
					case phone_through_heater:
						phone_heart_term_count = 0;
						phone_heart_term_flag = 1;		//phone online and through heart and into contrrol
						break;
					default:
						printf("unknown cmd! %x\n",recv_cmd);
						break;
				}
			}
			pos+=command_length;
			//A_PRINTF("process over,pos=%d\n",pos);
			if((ret-pos)<2) {
				break;
			}	
			memcpy(length_buf,recvBuff+pos,2);
			command_length = ntohs(*(unsigned short *)(length_buf));	
		}
	}
	else
		printf("receive data too much ret=%d\n",ret);		
	return 0;
}

int start_remote_service(void)   //called every 1000ms
{
	static int times_count=0; 
	//enum remote_service_status remote_status;

	times_count++;
	//remote_status=remote_service_par.remote_status;
	if(error_flag !=RTW_NO_ERROR){
		return-1;
	}
	if((remote_service_par.remote_socket_fd==-1) &&(remote_service_par.devide_mode==MODE_STA))
	{
		if(strlen(remote_service_par.serverip)>0){
			remote_socket_create();
		}else {
			if(dns_count == 0)
				GetHostByName();
		}
	}
	if(remote_service_par.remote_status==REMOTE_SERVER_QUERY){
		printf("tian send REMOTE_SERVER_QUERY\n");
		m_Query_server_address();
	}
	else if(remote_service_par.remote_status==REMOTE_SERVER_REGISTER){
		m_Send_Message_Body(T_REGISTER,0);
		printf("tianREMOTE_SERVER_REGISTER \n");
	}
	else if(remote_service_par.remote_status==REMOTE_OK){
		if(times_count > 30){
			printf("tian send  HEART_BEAT\n");
			m_Send_Message_Body(HEART_BEAT,0);
			times_count=0;
		}
	}
	return 0;
}
#endif
/********************************************** ***/
/******************APP 数据解析**********/
/*************************************************/
int  m_AnalysisData(char *recv,unsigned short len,S_RECV_APP_DATA *recvData)
{//{\"app_cmd\":\"1\",\"ir_data\":\"%s\"}\r\n
	int i=0,j=0,flag=0;
//	char *cmdIndex = NULL;
	int start_find=0,end_find=0;
	for(i=0;i<len;i++)
	{
		if(recv[i]=='{'){
			start_find=1;
			break;
		}	
	}
	if(start_find==0) {
		printf("format error:not find { as start flag,find len=%d\n",i);
		return -1;
	}
	for(;i<len;i++)
	{
		if(recv[i]=='"')
		{
			if(flag%2==0)j=0;
			flag++;
			i++;
			if(i==len) break;
			if(recv[i]=='"')
			{
				flag++;
			}
		}
		if(recv[i]=='}'){
			//printf("end find ,len=%d\n",i+1);
			end_find=1;
			return i+1;
		}
		if(recv[i]=='&')
		{
			if(((i+1)<len) && (recv[i+1]=='&'))
			{
				//printf("....recv once cmd over ! len:%d i:%d\n",len,i+2);
				#if 0
				if(len == i+2)
				{
					return;
				}
				else if(((len -i-2)>10)&&((len -i-2)<256))
				{
					cmdIndex = strstr(&recv[i+2], "app_cmd");
					if(cmdIndex != NULL)
					{
						printf("....left buf data:%s\n",&recv[i+2]);
						memset(server_rx_buf, 0x0, sizeof(server_rx_buf));
						server_rx_buf_len = len -i-2;
						A_STRCPY(server_rx_buf,&recv[i+2]);
					}
				}
				return;
				#else
				printf("one cmd over &&,len=%d\n\n",i+2);
				return i+2;
				#endif
			}		
		}
		if(flag==3)
			recvData->par1[j++] = recv[i];
		else if(flag==7)
			recvData->par2[j++] = recv[i]; 
		else if(flag==11)
			recvData->par3[j++] = recv[i];
		else if(flag==15){
			recvData->par4[j++] = recv[i]; 
		}
		else if(flag==19)
			recvData->par5[j++] = recv[i];
		//else if(flag==23)
		//	recvData->par5[j++] = recv[i]; 
	}
	printf("m_AnalysisData over ,len=%d\n",i);
	return i;
}
void system_soft_reset(void)
{
	//int socket_used[MAX_SOCKETS];
	int i;
	vTaskDelay(1000 / portTICK_RATE_MS);//wait socket send data over.
	for(i=0;i<MAX_SOCKETS;i++){
		if(socket_used[i]==1){
			close(i);
			printf("close socket %d\n",i);
		}
	}
	// disconnect all hap sessions
	//HAPServerForceCloseConnections();
	// Turn off Wi-Fi and reboot system
	wifi_off();
	printf("system will reboot......\n");
	sys_reset();
}

static rtw_result_t app_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
	static int ApNum = 0;

	if (malloced_scan_result->scan_complete != RTW_TRUE) {
		rtw_scan_result_t* record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */
		ApNum++;
		//print_scan_result(record);
		printf("SSID:%s,sec:%x,rssi:%d\n",record->SSID.val,record->security,record->signal_strength);
		if(malloced_scan_result->user_data)
			
			memcpy((void *)((char *)malloced_scan_result->user_data+(ApNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
			
		    
	} else{
		printf("scan over\n");
		ApNum = 0;
		scan_ap_end=1;
	}
	return RTW_SUCCESS;
}

int Scan_all_ssid(void)
{
       int  bssnum,ret;
	int  i;
	//scan_buf_arg scan_buf;
	//scan_buf.buf_len = 2048;
	//scan_buf.buf = malloc(scan_buf.buf_len);
	u8 *inic_scan_buf = NULL;
	//memset(scan_buf.buf, 0, scan_buf.buf_len);


	
	memset(ssid_info,0,sizeof(rtw_scan_result_t) * MAX_SSID_NUM);
	//inic_scan_buf = malloc(65*sizeof(rtw_scan_result_t));
	//if(inic_scan_buf == NULL){
		//ret = RTW_BUFFER_UNAVAILABLE_TEMPORARY;
		//goto exit;
	//	printf("tian debug alloc failed\n");
	//}
	scan_ap_end=0;
	//memset(inic_scan_buf, 0, 65*sizeof(rtw_scan_result_t));
	if((ret = wifi_scan_networks(app_scan_result_handler,ssid_info)) != RTW_SUCCESS){
		printf("[ATWS]ERROR: wifi scan failed\n\r");
		//goto exit;
	}
	
	// bssnum=wifi_scan(RTW_SCAN_TYPE_ACTIVE, RTW_BSS_TYPE_ANY, &scan_buf);
	//printf("tian debug wifi_scan over,return=%d\n",bssnum);
	/*
	if(bssnum==RTW_SUCCESS) 
	{
		bssnum=0;
		for(i=0;i<MAX_SSID_NUM;i++){
			if(ssid_info[i].SSID.len>0){
				bssnum++;
				printf("SSID:%s,sec:%d,rssi:%d\n",ssid_info[i].SSID.val,ssid_info[i].security,ssid_info[i].signal_strength);
			}
		}
	}
	else {
		bssnum=0;
		printf("scan failed\n");
	}
*/
	return bssnum;
}
int Addssid2Buffer(char * result_str,int result_length,int ssid_num)
{
	int i=0;
	char  info_temp[128];

	memset(info_temp,0,sizeof(info_temp));
	sprintf(info_temp,"%02x",ssid_info[ssid_num].SSID.len);
	//info_temp[i++]=ssid_info[ssid_num].ssid_len;//ssid length
	i+=2;
	//A_MEMCPY(&info_temp[i], ssid_info[ssid_num].ssid,ssid_info[ssid_num].ssid_len);//ssid
	//i +=ssid_info[ssid_num].ssid_len;
	Hex2Str(&info_temp[i],ssid_info[ssid_num].SSID.val,ssid_info[ssid_num].SSID.len);
	i+=strlen(&info_temp[i]);
	//info_temp[i++]=12; //essid length
	sprintf(&info_temp[i],"%02x%02x%02x",ssid_info[ssid_num].BSSID.octet[0],ssid_info[ssid_num].BSSID.octet[1],ssid_info[ssid_num].BSSID.octet[2]);
	i+=strlen(&info_temp[i]);
	sprintf(&info_temp[i],"%02x%02x%02x",ssid_info[ssid_num].BSSID.octet[3],ssid_info[ssid_num].BSSID.octet[4],ssid_info[ssid_num].BSSID.octet[5]);
	i+=strlen(&info_temp[i]);
	//A_MEMCPY(info_temp+i,ssid_info[ssid_num].bssid,6);//essid
	//i +=6;
	sprintf(info_temp+i,"%02x",ssid_info[ssid_num].signal_strength+100);
	i=i+2;
	//info_temp[i++]=ssid_info[ssid_num].rssi;
	if(ssid_info[ssid_num].security ==RTW_SECURITY_OPEN)
		sprintf(info_temp+i,"%02x",0x00);
	else 
		sprintf(info_temp+i,"%02x",0x01);
	i+=2;
	//info_temp[i++]=ssid_info[ssid_num].security_enabled; //encrypt method
	if(i<result_length){
		memcpy(result_str,info_temp,i);
		return i;		
	}	
	else{
		printf("tian debug buffer overrun! %d-%d\n",i,result_length);
		return 0;
	}
}
int m_APP_AP_CONFIG(unsigned char *data,int data_length,unsigned char * output_buffer){
	unsigned char command_num=0;
	int command_length=0,i=0,j,pos=0;
	unsigned char command_par_buf[64];
	unsigned char  bssid_buffer[6];
	unsigned char password_buf[64];
	int leng_temp=0,find_ok=0;
	//WACPersistentConfig_t  wac_config_par;
	memset(command_par_buf,0,sizeof(command_par_buf));
	printf("tian debug command data:%s,len=%d\n",data,data_length);
	command_length=Str2Hex(command_par_buf, data,data_length);;
	if(command_length>0){
		command_num=command_par_buf[0];
		//A_PRINTF("tian debug command num=%02x\n",command_num);
		sprintf(output_buffer,"%s","{\"wifi_cmd\":\"65\",\"data\":\"01");
		pos=strlen(output_buffer);	
		if(command_num==0x01){ //scan command
			Scan_all_ssid();
			while(scan_ap_end ==0){
				vTaskDelay(1000 / portTICK_RATE_MS);
				printf("tian debug wait scan\n");
			}
			if(ssid_info[0].SSID.len>0){
				printf("start to add buffer\n");
				for(i=0;i<MAX_SSID_NUM;i++){
					if(ssid_info[i].SSID.len>0){ 
						leng_temp=Addssid2Buffer((unsigned char *)(output_buffer+pos),TCP_SERVER_MAX_BUF-pos-1,i);
						//printf("pos=%d-%d,str=%s\n",leng_temp,pos,(unsigned char *)(output_buffer+pos));
						if(leng_temp==0){
							break;
						}
						pos+=leng_temp;
					}
					else{
						//break;
					}
				 }
				sprintf(output_buffer+pos,"%s","\",\"suc\":\"00\"}");
				//data_len_tmp +=1;
				//printf("debug Scan_ssid: %s bytes,i=%d\n",output_buffer,pos);
			}
			else
			{
				sprintf(output_buffer+pos,"\",\"suc\":\"01\"}");
			}
		}
		else if(command_num==0x02){ //select command
			sprintf(output_buffer,"%s","{\"wifi_cmd\":\"65\",\"data\":\"02");
			pos=strlen(output_buffer);	
			memcpy(bssid_buffer,command_par_buf+1,6);//bssid
			leng_temp =command_par_buf[7];               //passoword length
			memset(password_buf,0,sizeof(password_buf));
			memcpy(password_buf,command_par_buf+8,leng_temp);  //password
			printf("tian debug password= %s,len=%d,pos=%d\n",password_buf,leng_temp,pos);
			for(i=0;i<MAX_SSID_NUM;i++){  //find ssid throush bssid
				if(ssid_info[i].SSID.len>0){
					if(memcmp(bssid_buffer, ssid_info[i].BSSID.octet,6)==0){
						printf("ready to connect to %s\n",ssid_info[i].SSID.val);
			#if 0
						struct wlan_fast_reconnect read_data = {0};
						memcpy(read_data.psk_essid,ssid_info[i].SSID.val,ssid_info[i].SSID.len);
						memcpy(read_data.psk_passphrase,password_buf,leng_temp);
						wlan_wrtie_reconnect_data_to_flash();						//has problem
			#else
						
						memcpy(read_setting.ssid,ssid_info[i].SSID.val,ssid_info[i].SSID.len);
						memcpy(read_setting.password,password_buf,leng_temp);
						read_setting.mode = RTW_MODE_STA;
						read_setting.channel = ssid_info[i].channel;
						read_setting.security_type = ssid_info[i].security;
						read_setting.key_idx = 0;
						
						printf("ap config ssid=%s,password=%s,channel=%d,securty=%d\n",read_setting.ssid,read_setting.password,read_setting.channel,read_setting.security_type);
						//connect_get_ssid(&read_setting,1);
						//atcmd_wifi_write_info_to_flash(&read_setting, 1);		//record password no matter error or correct
						connect_flag = 1;										//dont need the flag
						
						
						
						
			#endif
						/*
						memset(&wac_config_par,0,sizeof(wac_config_par));
						wac_config_par.ssid_len=ssid_info[i].SSID.len;
						memcpy(wac_config_par.ssid,ssid_info[i].SSID.val,ssid_info[i].SSID.len);
						wac_config_par.password_len=strlen(password_buf);
						memcpy(wac_config_par.password,password_buf,strlen(password_buf));
						//wac_config_par.name_len=strlen(Device_IMEI);
						//strcpy(wac_config_par.name,Device_IMEI);
						unsigned char *mac = LwIP_GetMAC(&xnetif[0]);
						sprintf(wac_config_par.name, "%s%02x%02x%02x",NAME_PREFIX,mac[3], mac[4], mac[5]);
						wac_config_par.name_len=strlen(wac_config_par.name);
						WACPlatformSaveConfig(&wac_config_par);
						*/
						sprintf(output_buffer+pos,"%02x%s",strlen(Device_IMEI),Device_IMEI);
						pos+=strlen(output_buffer+pos);
						sprintf(output_buffer+pos,"%02x%s",strlen(Deault_password),Deault_password);
						pos+=strlen(output_buffer+pos);
						//qcom_sprintf(output_buffer+pos,"%02x%s",strlen(mMainPar.Serial_Number),mMainPar.Serial_Number);
						//pos+=strlen(output_buffer+pos);
						sprintf(output_buffer+pos,"03%d%d%d",(SoftVersion[0]-0x30),(SoftVersion[2]-0x30),(SoftVersion[4]-0x30));
						pos+=strlen(output_buffer+pos);
						sprintf(output_buffer+pos,"%02x%04x",4,MODEL);
						pos+=strlen(output_buffer+pos);
						sprintf(output_buffer+pos,"%s","\",\"suc\":\"00\"}");
						find_ok=1;
						//system_reboot=1;		//don,t reset------send message and reboot
						break;
					}
				}
			}
			if(find_ok==0){
				sprintf(output_buffer+pos,"\",\"suc\":\"01\"}");
			}	
		}
	}
	return 0;
}
#endif


int local_socket_create(void)
{
	struct sockaddr_in server_addr;
	int server_fd = -1;
	local_server_fd=-1;
	int on = 1;
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
		//add setsockopt
		int s = setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
		if(s<0){
			printf("setsockopt err\n");
			//return -1;
		}
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(SERVER_PORT);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
			printf("\nbind error\n");
			goto socket_err;
			//return -1;
		}

		if(listen(server_fd, LISTEN_QLEN) != 0) {
			printf("listen error\n");
			goto socket_err;
			//return -1;
		}
		printf("server socket =%d\n",server_fd);
		local_server_fd=server_fd;
		return server_fd;
	}
	else {
		printf("server socket create error\n");
		goto socket_err;
		//return -1;
	}
socket_err:
#if 0
		if(socket_conut++ >= 2){
			sys_reset();
		}
		else {
			wac_configure_sucess=1;
		}
#endif
	return -1;
}



int m_RecvData_local(int receive_fd)
{
#if 0
	int ret,pos=0,len=0,cmd,value=-1,k=0;
	char   *cmdIndex  = NULL;
	char   *heaterIndex = NULL;
	S_RECV_APP_DATA mRecvdata;
	unsigned int  send_len=0;
	char   cmd_buf[40];
	//unsigned char i = 0;
	char   *wifi_server_tx_buf = (char *)malloc(TCP_SERVER_MAX_BUF);
	if(wifi_server_tx_buf == NULL)
	{
		printf("%s: alloc buf failed. \n",__func__);
		return -1 ;
	}   
	//printf("\n\r%s(%d),  middle Available heap 0x%x\n", __FUNCTION__, __LINE__, xPortGetFreeHeapSize());
	memset(recvBuff,0,sizeof(recvBuff));
	ret=read(receive_fd, recvBuff, sizeof(recvBuff));
	if(ret<=0)
	{
		printf("m_RecvData_local  error,ret=%d,\n",ret);
		if (wifi_server_tx_buf != NULL)			//free malloc heap
		{
			free(wifi_server_tx_buf); 
			wifi_server_tx_buf=NULL;
		}
		return -1;
	}
	len=ret;
	//printf("local receive:%s\n",recvBuff);
	while(pos<len){
		cmdIndex = strstr(recvBuff+pos, "app_cmd"); 
		heaterIndex = strstr(recvBuff+pos, "KeepConnect"); 
		if((heaterIndex !=NULL)||(cmdIndex!=NULL))
		{
	      		//手机端发送:##000bKeepConnect&&   终端回复：##000bKeepConnect&&    lenth:19
			if(heaterIndex !=NULL)   
			{
				//Tcp_para.Fd_Para[number].Fd_time_count = TCP_SOCKET_KEEPTIME;
				//memset(send_heater_buf,0x0,sizeof(send_heater_buf));
				//A_STRCPY(send_heater_buf,"##000bKeepConnect&&");
				//TCP_Server_Send_API(number, send_heater_buf, 19,0) ;                    //回  KeepConnect
				pos += 19;
			}
			else if(cmdIndex!=NULL)                 //有命令
			{
				memset(&mRecvdata,0x0,sizeof(S_RECV_APP_DATA));
				//Tcp_para.Fd_Para[number].Fd_time_count = TCP_SOCKET_KEEPTIME;
				//anlysis app data
				ret = m_AnalysisData(recvBuff+pos,len-pos,&mRecvdata);  //解析命令和参数数据
				if(ret <0){
					printf("m_AnalysisData fail,ret=%d\n ",ret);
					pos = ret;
					break;
				}  
				cmd=atoi(mRecvdata.par1);
				//printf("get cmd:%d pos:%d,str=%s\n",cmd,pos,mRecvdata.par1);
				pos += ret;
				memset(wifi_server_tx_buf, 0x0, TCP_SERVER_MAX_BUF);
				switch(cmd)
				{
					case m_APP_STATUS:
						memset(cmd_buf,0,sizeof(cmd_buf));
#if 1
						//sprintf(cmd_buf+strlen(cmd_buf), "00-05-00-%d-%d-%d-%d",color_temp_status.warm,color_temp_status.cool,Light_status.brightness,Light_status.onoff);//#Dev_basic_para.Dev_Version#data1#
						//sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
						sprintf(cmd_buf+strlen(cmd_buf), "00-%02d-00-%d-%d-%d-%d",DEVICE_TYPE,Light_status.Cw.cool,Light_status.Cw.warm,Light_status.brightness,Light_status.onoff);//#Dev_basic_para.Dev_Version#data1#
						sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
#else
						sprintf(cmd_buf+strlen(cmd_buf), "%d-%d-%d-%d",Light_status.mode,Light_status.onoff,Light_status.brightness,Light_status.ratio);//#Dev_basic_para.Dev_Version#data1#
						sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
#endif
						sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"61\",\"imei\":\"%s\",\"data1\":\"%s\",\"suc\":\"%d\"}",Device_IMEI,cmd_buf,0);
						break;
					case APP_LIAN_Transfer:
						ota_socket=receive_fd;
						sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"data3\":\"",Device_IMEI);
						APP_DATA_PASS_THROUGH(mRecvdata.par4,strlen(mRecvdata.par4),wifi_server_tx_buf+6+strlen(wifi_server_tx_buf+6),TCP_SERVER_MAX_BUF-6-strlen(wifi_server_tx_buf+6));
						if(strlen(wifi_server_tx_buf+6)>0)
							sprintf(wifi_server_tx_buf+6+strlen(wifi_server_tx_buf+6),"\",\"suc\":\"%d%d\",\"seq\":\"%s\"}",0,0,mRecvdata.par5);						
						//sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"suc\":\"%d\",\"seq\":\"%02x\"}",Device_IMEI,0,223);
						break;
					case APP_SET_AP_CONFIG:
						//A_PRINTF("tian debug APP_SET_AP_CONFIG,%s\n",mRecvdata.par1);
						m_APP_AP_CONFIG(mRecvdata.par2,strlen(mRecvdata.par2),&wifi_server_tx_buf[6]);
						break;
					default:
						break;
				}
				strcpy(wifi_server_tx_buf,"##");                               //## 包识别头
				send_len = strlen(&wifi_server_tx_buf[6]);                       //回复信息长度
				memset(cmd_buf,0,sizeof(cmd_buf));
				sprintf(cmd_buf,"%04x",send_len);
				memcpy(&wifi_server_tx_buf[2],cmd_buf,4);      //将信息长度存入buf[2] 
				strcpy(&wifi_server_tx_buf[send_len+6],"&&");        //将数据包尾标识符&& 存入send_buf尾
				//A_PRINTF("tcp server send:%s \n",wifi_server_tx_buf);
				if(music_rgb_cw.music_flag == 0) {
					printf("when not in music send message\n");
					ret = write(receive_fd,wifi_server_tx_buf,strlen(wifi_server_tx_buf));  //将信息发送出去
				}
				if(ret>0){
					if(music_rgb_cw.music_flag == 0)
						printf("local send:%s\n",wifi_server_tx_buf);

					if(connect_flag == 1)
					{
						vTaskDelay(2000 / portTICK_RATE_MS);
						connect_flag = 0;
#if 0					
						while(value != 0)
							{
							 	value = connect_get_ssid(&read_setting,0);	//suggest delay
							 	if(value != 0)
									vTaskDelay(20);
							 	if(++k > 2)
							 	{	
							 		k =0;
									break;
							 	}
								printf("test times : %d\n",k);
							}

							//value = 0;
							printf("return value=%d\n",value);				//value can tell app which wrong if fail,  if success value = 0
							//format is fix,so can write simple
							memset(wifi_server_tx_buf, 0x0, TCP_SERVER_MAX_BUF);
							sprintf(wifi_server_tx_buf,"##002a{\"wifi_cmd\":\"65\",\"data\":\"03%02x\",\"suc\":\"00\"}&&",value);
							
							ret = write(receive_fd,wifi_server_tx_buf,strlen(wifi_server_tx_buf)); 
							printf("write len : %d,write buf : %s\n",ret,wifi_server_tx_buf);
							//vTaskDelay(10000 / portTICK_RATE_MS);		//change interval 3s
							if(ret>0)
								printf("send 03:%s\n",wifi_server_tx_buf);

#endif

							value = 0;
							if(value == 0)
							{
								//close ap,write to flash
								//Mode_enable(1,2);
								//vTaskDelay(7000 / portTICK_RATE_MS);
								do{
									value = connect_get_ssid(&read_setting,1);	//suggest delay
							 		if(value != 0)
							 		{
							 			if(++k > 2)
							 			{	
							 				k =0;
											break;
							 			}
										printf("test times : %d\n",k);
										vTaskDelay(100);
							 		}
								}while(value != 0);
								//connect_get_ssid(&read_setting,1);
								atcmd_wifi_write_info_to_flash(&read_setting, 1);	//only wifi_connect success ,also DHCP_start also need success

								//wac_configure_sucess=1;
//								#if REMOTE_SERVER==AWS_IOT_CLOUD
//								printf("into config AWS_IOT_CLOUD\n");
//								example_amazon_awsiot();
//								#endif
							}
							else		//else reboot
							{
								printf("config error ,reboot\n");
								
							}
							system_reboot=1;	//certify fail,suggest reboot,beacuse low-level autoreconnect,ap still on but can,t use
							
					}

				}else{
									
					printf("local sent failed,ret=%d\n",ret);
				}
				//TCP_Server_Send_MsgBody(cmd,receive_fd);
			}
			//return  0;
		}
		else
		{
			//A_PRINTF("TCP Server Recv Data Error\n");
			break;
		}
	}
	if (wifi_server_tx_buf != NULL)
	{
		free(wifi_server_tx_buf); 
		wifi_server_tx_buf=NULL;
	}
#endif
	return 0;
}

#if 0

uint32_t get_brocast_ip(void)
{
	uint32_t broadcast_ip;
	uint32_t ip,netmask;
	struct netif * pnetif;

	//ip = *(uint32_t *)LwIP_GetIP(&xnetif[0]);
	//netmask = *(uint32_t *)LwIP_GetMASK(&xnetif[0]);
#if CONFIG_LWIP_LAYER
		if(wifi_mode == RTW_MODE_STA_AP)
			pnetif = &xnetif[1];
		else
			pnetif = &xnetif[0];
#endif

	ip = *(uint32_t *)LwIP_GetIP(pnetif);
	netmask = *(uint32_t *)LwIP_GetMASK(pnetif);
	//printf("ip:%s,ipaddr : %08x,netmask : %08x\n",_inet_ntoa1(ip),ip,netmask);
	broadcast_ip=(ip & netmask)|(~netmask);
	//printf("ip:%04x,mask:%04x,brocast:%04x\n",ip,netmask,broadcast_ip);
	//printf("get ip : %s,ipaddr : %08x\n",_inet_ntoa1(broadcast_ip),broadcast_ip);
	return broadcast_ip;
}
//探测初始化函数
void Probe_init(void)
{
	int  so_bcast = 1;
	struct sockaddr_in recv_addr;
	remote_service_par.probe_fd=-1;
	//printf("probe thread. ......  \n");
	if((remote_service_par.probe_fd=(socket(AF_INET,SOCK_DGRAM,0)))==-1)
	{
		printf("%s: socket create fail.\n",__func__);
		if(socket_conut++ >= 2)
			sys_reset();
		else
			wac_configure_sucess=1;
		//qcom_sys_reset();
		//wac_configure_sucess=1;
		return;
	}
	printf("probe thread,socket=%d  \n",remote_service_par.probe_fd);
	//setsockopt(remote_service_par.probe_fd,  SOL_SOCKET,  SO_NBIO,  (void *)&so_bcast,   sizeof(so_bcast))  ;
       setsockopt(remote_service_par.probe_fd,  SOL_SOCKET,  SO_BROADCAST,  (void *)&so_bcast,   sizeof(so_bcast)) ;
       setsockopt(remote_service_par.probe_fd, SOL_SOCKET,SO_REUSEADDR, (void *)&so_bcast,  sizeof(so_bcast));

	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(PROBE_PORT);
	recv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
       if((bind(remote_service_par.probe_fd,(struct sockaddr *)&recv_addr, sizeof(struct sockaddr_in))) < 0) 
	{
		printf("%s:  bind error.\n", __func__);
		if(socket_conut++ >= 2)
			sys_reset();
		else
			wac_configure_sucess=1;
		//qcom_sys_reset();
	}
}

//探测处理函数
int Probe_proc(void)
{
#if 1
	int ret,pos=0,len=0,cmd,value=-1,k=0;
	char   *cmdIndex  = NULL;
	char   *heaterIndex = NULL;
	S_RECV_APP_DATA mRecvdata;
	unsigned int  send_len=0;
	char   cmd_buf[40];
#endif
	char   buf[512];
	char *getbuf;
	char timebuf[22];
	char  probe_send_len = 1;	
	char  bcast_addr[20];
	int nrecv = 0;
	//int send_recv_buf = 0;
	char *check_recv = NULL;
	char *cammand_recv=NULL;
	//char *get_time = NULL;
	socklen_t size;
	//S_RECV_APP_DATA mRecvdata;
	uint32_t broadcast_ip;
	//int ret;
	//unsigned char cmd;
	struct sockaddr_in recv_addr;
	
	memset(buf, 0x0, sizeof(buf));     
	memset(timebuf,0,sizeof(timebuf));
	size=sizeof(recv_addr);
	ret = recvfrom(remote_service_par.probe_fd,   buf, sizeof(buf),   0,   (struct sockaddr *)&recv_addr, &size) ;
	//Probe#2015-03-11-14-13-50-0
	if (ret > 0)
	{
		uint8_t *ip = (uint8_t *) &recv_addr.sin_addr.s_addr;
		uint16_t from_port = ntohs(recv_addr.sin_port);
		//printf("recvfrom - %d bytes from %d.%d.%d.%d:%d\n", nrecv, ip[0], ip[1], ip[2], ip[3], from_port);
		//printf("recv content : %s\n",buf);
	       check_recv = strstr(buf, "Probe");
		   cammand_recv = strstr(buf, "app_cmd");
		   
        	if(check_recv != NULL)    
		{
			//设置模块时钟时间
			//sntp可以更新手机现在设置的时间，但是sntp如果已经获取到时间那就不要手机这个探测时间
			//sntp获取到时间，但是在24:00的时候会更新，这个时候其实也没必要要手机的时间，sys_tm是有值的，还是就借用这个标志位进行判断
			//printf("SET_TIME_FLAG = %d,remote_service_par.devide_mode=%d\n",SET_TIME_FLAG,remote_service_par.devide_mode);
			printf("recv content : %s-----time %d:%d:%d\n",buf,sys_tm.tm_hour,sys_tm.tm_min,sys_tm.tm_sec);		//test
			
			if((SET_TIME_FLAG==0)&&(remote_service_par.devide_mode==MODE_STA)&&(SntpGetTimeSuccessFlag == FALSE))
			{
				//format 2017-10-20-14-50-22-6	
				memcpy(timebuf,&buf[6],21);
				m_SetCurrentTime(timebuf);
			}
			#if 1
			int sendLen;
			struct sockaddr to;
			struct sockaddr_in *to_sin = (struct sockaddr_in*) &to;
			to_sin->sin_family = AF_INET;
			to_sin->sin_port = htons(PROBE_PORT);
			to_sin->sin_addr.s_addr = get_brocast_ip();//htonl(get_brocast_ip());//INADDR_ANY;
			getbuf=buf;
			//printf("brocast send to %s\n",inet_ntoa(to_sin->sin_addr.s_addr));
			memset(getbuf, 0x0,  sizeof(buf));
			sprintf(getbuf, "%s#%s#%04x#", Device_IMEI,Deault_password,MODEL);//#Dev_basic_para.Serial_Number#
#if 1
			sprintf(getbuf+strlen(getbuf), "%s##00-%02d-00",SoftVersion,DEVICE_TYPE);//#Dev_basic_para.Dev_Version#data1#
			if(Light_status.onoff == 0)
				sprintf(getbuf+strlen(getbuf), "-%d-%d-%d-%d",Light_status.Cw.cool,Light_status.Cw.warm,0,Light_status.onoff);
			else
				sprintf(getbuf+strlen(getbuf), "-%d-%d-%d-%d",Light_status.Cw.cool,Light_status.Cw.warm,Light_status.brightness,Light_status.onoff);
			sprintf(getbuf+strlen(getbuf), "-%d-%d-%d",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
			//sprintf(getbuf+strlen(getbuf),"-%d-%d-%d-%d-%d",sys_tm.tm_hour,sys_tm.tm_min,sys_tm.tm_sec,(60*sys_tm.tm_hour+sys_tm.tm_min),TmrLed_onoff_current_task.time);					//fu test for set timer
			//sprintf(getbuf+strlen(getbuf), "-%d-%d-%d-%d",color_temp_status.warm,color_temp_status.cool,Light_status.brightness,Light_status.onoff);
			//sprintf(getbuf+strlen(getbuf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
#else
			sprintf(getbuf+strlen(getbuf), "%s##%d",SoftVersion,Light_status.mode);//#Dev_basic_para.Dev_Version#data1#
			sprintf(getbuf+strlen(getbuf), "-%d-%d-%d",Light_status.onoff,Light_status.brightness,Light_status.ratio);
			sprintf(getbuf+strlen(getbuf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
#endif
			if((sendLen = sendto(remote_service_par.probe_fd, getbuf,strlen(getbuf), 0, &to, sizeof(struct sockaddr))) < 0)
				printf("ERROR: sendto broadcast,ret==%d\n",sendLen);
			else
				printf("brocast:%s\n",getbuf);
			#else
			memset(bcast_addr,  0x0,  sizeof(bcast_addr));
			getDeviceIP(bcast_addr); 
			memset(&probe_para.send_addr, 0x0, sizeof(probe_para.send_addr));
			probe_para.send_addr.sin_family = AF_INET;
			probe_para.send_addr.sin_port = htons(PROBE_PORT);
			probe_para.send_addr.sin_addr.s_addr =  htonl(_inet_addr1(bcast_addr));     //inet_addr("255.255.255.255");
			getbuf=buf;
			memset(getbuf, 0x0,  sizeof(buf));
			sprintf(getbuf, "%s#%s##", Dev_basic_para.UID,Dev_basic_para.PassWord);//#Dev_basic_para.Serial_Number#
			sprintf(getbuf+strlen(getbuf), "%s##%d",SoftVersion,Dev_Main_para.ONOFF);//#Dev_basic_para.Dev_Version#data1#
			sprintf(getbuf+strlen(getbuf), "-%d-%d-%d",Dev_Main_para.Cool_White_value,Dev_Main_para.Warm_white_value,Dev_Main_para.DELAY_SOCKET.DelayTimeF);
			probe_send_len = qcom_sendto(probe_para.probe_fd,  getbuf, strlen(getbuf),   0,  (struct sockaddr *)&probe_para.send_addr,sizeof(probe_para.send_addr));
			A_PRINTF("%s  send:  %s  len: %d.\n", __func__, getbuf, probe_send_len);
			#endif
			return 0;
		}

			if(cammand_recv != NULL)
			{
				//printf("recv data\n");
				char   *wifi_server_tx_buf = (char *)malloc(UDP_ALLOC_MAX_BUF);
				if(wifi_server_tx_buf == NULL)
				{
					printf("%s: alloc buf failed. \n",__func__);
					return -1 ;
				}
				int sendLen;
#if 0
				int sendLen;
				struct sockaddr to;
				struct sockaddr_in *to_sin = (struct sockaddr_in*) &to;
				to_sin->sin_family = AF_INET;
				to_sin->sin_port = htons(PROBE_PORT);
				to_sin->sin_addr.s_addr = get_brocast_ip();//htonl(get_brocast_ip());//INADDR_ANY;
#endif
				len=ret;
				//printf("recv len:%d\n",len);
					while(pos<len){
						cmdIndex = strstr(buf+pos, "app_cmd"); 
						heaterIndex = strstr(buf+pos, "KeepConnect"); 
						if((heaterIndex !=NULL)||(cmdIndex!=NULL))
						{
					      		//手机端发送:##000bKeepConnect&&   终端回复：##000bKeepConnect&&    lenth:19
							if(heaterIndex !=NULL)   
							{
								//Tcp_para.Fd_Para[number].Fd_time_count = TCP_SOCKET_KEEPTIME;
								//memset(send_heater_buf,0x0,sizeof(send_heater_buf));
								//A_STRCPY(send_heater_buf,"##000bKeepConnect&&");
								//TCP_Server_Send_API(number, send_heater_buf, 19,0) ;                    //回  KeepConnect
								pos += 19;
							}
							else if(cmdIndex!=NULL)                 //有命令
							{
								//printf("recv cmd\n");
								memset(&mRecvdata,0x0,sizeof(S_RECV_APP_DATA));
								//Tcp_para.Fd_Para[number].Fd_time_count = TCP_SOCKET_KEEPTIME;
								//anlysis app data
								//printf("recv:%s\n",buf+pos);
								ret = m_AnalysisData(buf+pos,len-pos,&mRecvdata);  //解析命令和参数数据
								if(ret <0){
									printf("m_AnalysisData fail,ret=%d\n ",ret);
									pos = ret;
									break;
								}  
								cmd=atoi(mRecvdata.par1);

								printf("get cmd:%d pos:%d,str=%s\n",cmd,pos,mRecvdata.par1);
								pos += ret;
								memset(wifi_server_tx_buf, 0x0, UDP_ALLOC_MAX_BUF);
								switch(cmd)
								{
									case m_APP_STATUS:
										memset(cmd_buf,0,sizeof(cmd_buf));
#if 1
										//sprintf(cmd_buf+strlen(cmd_buf), "00-05-00-%d-%d-%d-%d",color_temp_status.warm,color_temp_status.cool,Light_status.brightness,Light_status.onoff);//#Dev_basic_para.Dev_Version#data1#
										//sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
										sprintf(cmd_buf+strlen(cmd_buf), "00-%02d-00-%d-%d-%d-%d",DEVICE_TYPE,Light_status.Cw.cool,Light_status.Cw.warm,Light_status.brightness,Light_status.onoff);//#Dev_basic_para.Dev_Version#data1#
										sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
#else
										sprintf(cmd_buf+strlen(cmd_buf), "%d-%d-%d-%d",Light_status.mode,Light_status.onoff,Light_status.brightness,Light_status.ratio);//#Dev_basic_para.Dev_Version#data1#
										sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
#endif
										sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"61\",\"imei\":\"%s\",\"data1\":\"%s\",\"suc\":\"%d\"}",Device_IMEI,cmd_buf,0);
										break;
									case APP_LIAN_Transfer:
										//ota_socket=receive_fd;
										sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"data3\":\"",Device_IMEI);
										APP_DATA_PASS_THROUGH(mRecvdata.par4,strlen(mRecvdata.par4),wifi_server_tx_buf+6+strlen(wifi_server_tx_buf+6),TCP_SERVER_MAX_BUF-6-strlen(wifi_server_tx_buf+6));
										if(strlen(wifi_server_tx_buf+6)>0)
											sprintf(wifi_server_tx_buf+6+strlen(wifi_server_tx_buf+6),"\",\"suc\":\"%d%d\",\"seq\":\"%s\"}",0,0,mRecvdata.par5);						
										//sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"suc\":\"%d\",\"seq\":\"%02x\"}",Device_IMEI,0,223);
										break;
									case APP_SET_AP_CONFIG:
										//A_PRINTF("tian debug APP_SET_AP_CONFIG,%s\n",mRecvdata.par1);
										m_APP_AP_CONFIG(mRecvdata.par2,strlen(mRecvdata.par2),&wifi_server_tx_buf[6]);
										break;
									default:
										break;
								}
								strcpy(wifi_server_tx_buf,"##");                               //## 包识别头
								send_len = strlen(&wifi_server_tx_buf[6]);                       //回复信息长度
								memset(cmd_buf,0,sizeof(cmd_buf));
								sprintf(cmd_buf,"%04x",send_len);
								memcpy(&wifi_server_tx_buf[2],cmd_buf,4);      //将信息长度存入buf[2] 
								strcpy(&wifi_server_tx_buf[send_len+6],"&&");        //将数据包尾标识符&& 存入send_buf尾
								//A_PRINTF("tcp server send:%s \n",wifi_server_tx_buf);
								if(music_rgb_cw.music_flag == 0) {
									printf("not music control\n");
									if((sendLen = sendto(remote_service_par.probe_fd, wifi_server_tx_buf,strlen(wifi_server_tx_buf), 0, (struct sockaddr *)&recv_addr, sizeof(struct sockaddr))) < 0)
										printf("ERROR: sendto 15000 port broadcast,ret==%d\n",sendLen);
									else
										printf("15000 port brocast:%s\n",wifi_server_tx_buf);
								}
							
							}
			
						}
						else
						{
							//A_PRINTF("TCP Server Recv Data Error\n");
							break;
						}
			}
			if (wifi_server_tx_buf != NULL)
			{
				free(wifi_server_tx_buf); 
				wifi_server_tx_buf=NULL;
			}
			return 0;
			}	else {
					//printf("get no data\n");
				}
		
      }
	return 0;
}

#ifdef set_group_udp

void Group_probe_init(void)
{
	int  so_bcast = 1;
	struct sockaddr_in recv_addr;
	remote_service_par.group_udp_fd=-1;
	//printf("probe thread. ......  \n");
	if((remote_service_par.group_udp_fd=(socket(AF_INET,SOCK_DGRAM,0)))==-1)
	{
		printf("%s: socket create fail.\n",__func__);
		//qcom_sys_reset();
		return;
	}
	printf("16000 port probe thread,socket=%d  \n",remote_service_par.group_udp_fd);
	//setsockopt(remote_service_par.probe_fd,  SOL_SOCKET,  SO_NBIO,  (void *)&so_bcast,   sizeof(so_bcast))  ;
       setsockopt(remote_service_par.group_udp_fd,  SOL_SOCKET,  SO_BROADCAST,  (void *)&so_bcast,   sizeof(so_bcast)) ;
       setsockopt(remote_service_par.group_udp_fd, SOL_SOCKET,SO_REUSEADDR, (void *)&so_bcast,  sizeof(so_bcast));

	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(GROUP_PORT);
	recv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
       if((bind(remote_service_par.group_udp_fd,(struct sockaddr *)&recv_addr, sizeof(struct sockaddr_in))) < 0) 
	{
		printf("%s: 16000 port bind error.\n", __func__);
		//qcom_sys_reset();
	}
}


int Group_probe_proc(void)
{
	int ret,pos=0,len=0,cmd,value=-1,k=0;
	char   *cmdIndex  = NULL;
	char   *heaterIndex = NULL;
	S_RECV_APP_DATA mRecvdata;
	unsigned int  send_len=0;
	char   cmd_buf[40];
	//unsigned char i = 0;
	socklen_t size;
	struct sockaddr_in recv_addr;
	size=sizeof(recv_addr);
	char   *wifi_server_tx_buf = (char *)malloc(UDP_MAX_BUF);
	if(wifi_server_tx_buf == NULL)
	{
		printf("%s: alloc buf failed. \n",__func__);
		return -1 ;
	}   
	//printf("\n\r%s(%d),  middle Available heap 0x%x\n", __FUNCTION__, __LINE__, xPortGetFreeHeapSize());
	memset(recvBuff,0,sizeof(recvBuff));
	ret = recvfrom(remote_service_par.group_udp_fd,   recvBuff, sizeof(recvBuff),   0,   (struct sockaddr *)&recv_addr, &size);
	if(ret<=0)
	{
		printf("m_RecvData_local  error,ret=%d,\n",ret);
		if (wifi_server_tx_buf != NULL)			//free malloc heap
		{
			free(wifi_server_tx_buf); 
			wifi_server_tx_buf=NULL;
		}
		return -1;
	}
	len=ret;
	printf("local receive:%s\n",recvBuff);
	int sendLen;
	struct sockaddr to;
	struct sockaddr_in *to_sin = (struct sockaddr_in*) &to;
	to_sin->sin_family = AF_INET;
	to_sin->sin_port = htons(GROUP_PORT);
	to_sin->sin_addr.s_addr = get_brocast_ip();//htonl(get_brocast_ip());//INADDR_ANY;
	
	while(pos<len){
		cmdIndex = strstr(recvBuff+pos, "app_cmd"); 
		heaterIndex = strstr(recvBuff+pos, "KeepConnect"); 
		if((heaterIndex !=NULL)||(cmdIndex!=NULL))
		{
	      		//手机端发送:##000bKeepConnect&&   终端回复：##000bKeepConnect&&    lenth:19
			if(heaterIndex !=NULL)   
			{
				//Tcp_para.Fd_Para[number].Fd_time_count = TCP_SOCKET_KEEPTIME;
				//memset(send_heater_buf,0x0,sizeof(send_heater_buf));
				//A_STRCPY(send_heater_buf,"##000bKeepConnect&&");
				//TCP_Server_Send_API(number, send_heater_buf, 19,0) ;                    //回  KeepConnect
				pos += 19;
			}
			else if(cmdIndex!=NULL)                 //有命令
			{
				memset(&mRecvdata,0x0,sizeof(S_RECV_APP_DATA));
				//Tcp_para.Fd_Para[number].Fd_time_count = TCP_SOCKET_KEEPTIME;
				//anlysis app data
				ret = m_AnalysisData(recvBuff+pos,len-pos,&mRecvdata);  //解析命令和参数数据
				if(ret <0){
					printf("m_AnalysisData fail,ret=%d\n ",ret);
					pos = ret;
					break;
				}  
				cmd=atoi(mRecvdata.par1);
				//printf("get cmd:%d pos:%d,str=%s\n",cmd,pos,mRecvdata.par1);
				pos += ret;
				memset(wifi_server_tx_buf, 0x0, UDP_MAX_BUF);
				switch(cmd)
				{
					case m_APP_STATUS:
						memset(cmd_buf,0,sizeof(cmd_buf));
#if 1
						//sprintf(cmd_buf+strlen(cmd_buf), "00-05-00-%d-%d-%d-%d",color_temp_status.warm,color_temp_status.cool,Light_status.brightness,Light_status.onoff);//#Dev_basic_para.Dev_Version#data1#
						//sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
						sprintf(cmd_buf+strlen(cmd_buf), "00-%02d-00-%d-%d-%d-%d",DEVICE_TYPE,Light_status.Cw.cool,Light_status.Cw.warm,Light_status.brightness,Light_status.onoff);//#Dev_basic_para.Dev_Version#data1#
						sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",Light_status.Rgb.r,Light_status.Rgb.g,Light_status.Rgb.b);
#else
						sprintf(cmd_buf+strlen(cmd_buf), "%d-%d-%d-%d",Light_status.mode,Light_status.onoff,Light_status.brightness,Light_status.ratio);//#Dev_basic_para.Dev_Version#data1#
						sprintf(cmd_buf+strlen(cmd_buf), "-%d-%d-%d",rgb_status.r,rgb_status.g,rgb_status.b);
#endif
						sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"61\",\"imei\":\"%s\",\"data1\":\"%s\",\"suc\":\"%d\"}",Device_IMEI,cmd_buf,0);
						break;
					case APP_LIAN_Transfer:
						//ota_socket=receive_fd;
						sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"data3\":\"",Device_IMEI);
						APP_DATA_PASS_THROUGH(mRecvdata.par4,strlen(mRecvdata.par4),wifi_server_tx_buf+6+strlen(wifi_server_tx_buf+6),TCP_SERVER_MAX_BUF-6-strlen(wifi_server_tx_buf+6));
						if(strlen(wifi_server_tx_buf+6)>0)
							sprintf(wifi_server_tx_buf+6+strlen(wifi_server_tx_buf+6),"\",\"suc\":\"%d%d\",\"seq\":\"%s\"}",0,0,mRecvdata.par5);						
						//sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"suc\":\"%d\",\"seq\":\"%02x\"}",Device_IMEI,0,223);
						break;
					case APP_SET_AP_CONFIG:
						//A_PRINTF("tian debug APP_SET_AP_CONFIG,%s\n",mRecvdata.par1);
						m_APP_AP_CONFIG(mRecvdata.par2,strlen(mRecvdata.par2),&wifi_server_tx_buf[6]);
						break;
					default:
						break;
				}
				strcpy(wifi_server_tx_buf,"##");                               //## 包识别头
				send_len = strlen(&wifi_server_tx_buf[6]);                       //回复信息长度
				memset(cmd_buf,0,sizeof(cmd_buf));
				sprintf(cmd_buf,"%04x",send_len);
				memcpy(&wifi_server_tx_buf[2],cmd_buf,4);      //将信息长度存入buf[2] 
				strcpy(&wifi_server_tx_buf[send_len+6],"&&");        //将数据包尾标识符&& 存入send_buf尾
				//A_PRINTF("tcp server send:%s \n",wifi_server_tx_buf);
				if((sendLen = sendto(remote_service_par.group_udp_fd, wifi_server_tx_buf,strlen(wifi_server_tx_buf), 0, &to, sizeof(struct sockaddr))) < 0)
					printf("ERROR: sendto 16000 port broadcast,ret==%d\n",sendLen);
				else
					printf("16000 port brocast:%s\n",wifi_server_tx_buf);
#if 0
				if(ret>0){
					printf("local send:%s\n",wifi_server_tx_buf);

					if(connect_flag == 1)
					{
						vTaskDelay(2000 / portTICK_RATE_MS);
						connect_flag = 0;
#if 1					
						while(value != 0)
							{
							 	value = connect_get_ssid(&read_setting,0);	//suggest delay
							 	if(value != 0)
									vTaskDelay(20);
							 	if(++k > 2)
							 	{	
							 		k =0;
									break;
							 	}
								printf("test times : %d\n",k);
							}
#endif
							//value = 0;
							printf("return value=%d\n",value);				//value can tell app which wrong if fail,  if success value = 0
							//format is fix,so can write simple
							memset(wifi_server_tx_buf, 0x0, TCP_SERVER_MAX_BUF);
							sprintf(wifi_server_tx_buf,"##002a{\"wifi_cmd\":\"65\",\"data\":\"03%02x\",\"suc\":\"00\"}&&",value);
							
							ret = write(receive_fd,wifi_server_tx_buf,strlen(wifi_server_tx_buf)); 
							printf("write len : %d,write buf : %s\n",ret,wifi_server_tx_buf);
							//vTaskDelay(10000 / portTICK_RATE_MS);		//change interval 3s
							if(ret>0)
								printf("send 03:%s\n",wifi_server_tx_buf);
							if(value == 0)
							{
								//close ap,write to flash
								//Mode_enable(1,2);
								//vTaskDelay(7000 / portTICK_RATE_MS);
								connect_get_ssid(&read_setting,1);
								atcmd_wifi_write_info_to_flash(&read_setting, 1);	//only wifi_connect success ,also DHCP_start also need success
								wac_configure_sucess=1;
//								#if REMOTE_SERVER==AWS_IOT_CLOUD
//								printf("into config AWS_IOT_CLOUD\n");
//								example_amazon_awsiot();
//								#endif
							}
							else		//else reboot
							{
								printf("config error ,reboot\n");
								system_reboot=1;	//certify fail,suggest reboot,beacuse low-level autoreconnect,ap still on but can,t use
							}
					}

				}else{
									
					printf("local sent failed,ret=%d\n",ret);
				}
				//TCP_Server_Send_MsgBody(cmd,receive_fd);
#endif
			}
			//return  0;
		}
		else
		{
			//A_PRINTF("TCP Server Recv Data Error\n");
			break;
		}
	}
	if (wifi_server_tx_buf != NULL)
	{
		free(wifi_server_tx_buf); 
		wifi_server_tx_buf=NULL;
	}
	return 0;
}


#endif

void socket_timeout_process(void)
{
	int i;
	for(i=0;i<MAX_SOCKETS;i++){
		if(socket_used[i]==1){
			//socket_timeout[i]++;
			if((i==remote_service_par.local_server_fd) || (i==remote_service_par.probe_fd))
			{
				continue;
			}
			socket_timeout[i]++;
			if(socket_timeout[i]>SOCKET_CLOSE_TIMEOUT){
				socket_timeout[i]=0;
				socket_used[i]=0;
				close(i);
				if(i==remote_service_par.remote_socket_fd){
					printf("remote no answer reset sys\n");
					//wifi_off();			//this correspond to reset
					//sys_reset();
					remote_service_par.remote_socket_fd=-1;		//this correspond to reconnect
					printf("remote socket close\n");
				}else{
					printf("socket %d timeout,close it!\n",i);
				}
			}
		}
	}
}
int ota_done_ack(int socket_fd,int result_code)
{
	char wifi_server_tx_buf[128];
	char len_buffer[8];
	int send_len;
	memset(wifi_server_tx_buf,0,sizeof(wifi_server_tx_buf));
	strcpy(wifi_server_tx_buf,"##");                               //## 包识别头
	sprintf(&wifi_server_tx_buf[6],"{\"wifi_cmd\":\"62\",\"imei\":\"%s\",\"suc\":\"%d\",\"seq\":\"%02x\"}",Device_IMEI,result_code,0xff);
	send_len = strlen(&wifi_server_tx_buf[6]);                       //回复信息长度
	memset(len_buffer,0,sizeof(len_buffer));
	sprintf(len_buffer,"%04x",send_len);
	memcpy(&wifi_server_tx_buf[2],len_buffer,4);
	strcpy(&wifi_server_tx_buf[send_len+6],"&&");        //将数据包尾标识符&& 存入send_buf尾
	//printf("tcp server send:%s \n",wifi_server_tx_buf);
	send_len = write(socket_fd,wifi_server_tx_buf,strlen(wifi_server_tx_buf));  //将信息发送出去
	if(send_len>0){
		printf("ota send:%s,sucess,len=%d\n",wifi_server_tx_buf,send_len);
	}else{
		printf("ota send failed,ret=%d\n",send_len);
	}
	return 0;
}

void power_save_start(void)
{
	if(remote_service_par.devide_mode==MODE_STA)
	{
		printf("--------\n");
		wifi_set_power_mode(0, 0); //ips,lps disable
		wifi_set_lps_dtim(10); //dtim set 10(100ms)
		wifi_set_power_mode(1, 1);//ips,lps disable
		pmu_release_wakelock(BIT(PMU_OS)); //never use it,or system will never wakeup
		//pmu_tickless_debug(ENABLE); /* open tickless log */
		//pmu_sysactive_timer_init();
		//pmu_set_sysactive_time(PMU_LOGUART_DEVICE, 5000);
		//pmu_release_wakelock(PMU_OS);
		
	}else{
		//ap_timeout=1;
	}
}

#endif

int send_recv_info(char *buffer,int len)
{
	int ret = 0,tmp_len=0,write_pos=0;
	printf("send len=%d\n",len);
	if(write_to_info_fd > 0)
	{
		tmp_len = len;
		do
		{
			ret = write(write_to_info_fd,&buffer[write_pos],tmp_len-ret);
			printf("write len=%d\n",ret);
			tmp_len -= ret;
			write_pos += ret;
			wait_ms(3);
		}while(tmp_len > 0);
	}
	return 0;
}

void tcp_socket_select_thread(void *param)
{
	int max_socket_fd = -1,ret;
	//struct sockaddr_in remote_addr;
	int remote_fd = -1;
	// ip_addr_t remote_server_address;
	 err_t err;
	//int socket_used[MAX_SOCKETS];
	//	if(strlen(Device_IMEI) == 0)
	//	{
	//		printf("into tcp_socket_select_thread\n");
   //    		generate_imei_from_mac(Device_IMEI);
	//	}
	//	memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
	// Delay to wait for IP by DHCP
restart_thread:
	//if(wac_configure_sucess == 0)				//restart should dns、query、register.because close fd but not set -1 
	//	remote_service_par.remote_status=REMOTE_WAIT;
	//remote_socket_fd=-1;
	//memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
	printf("enter tcp_socket_select_thread,soft versions\n");
	while(1) {
		vTaskDelay(1000 / portTICK_RATE_MS);
		//watchdog_refresh();		//test watchdog
		//if((wifi_is_ready_to_transceive(RTW_AP_INTERFACE)==RTW_SUCCESS)){
		//	printf("ap ok\n");
		//	break;
		//}else
		if(wifi_is_ready_to_transceive(RTW_STA_INTERFACE) == RTW_SUCCESS){
			//remote_service_par.devide_mode=MODE_STA;
			printf("sta ok\n");
			break;
		}else{
			printf("\nwait network ready \n");
		}
       }
	memset(socket_used, 0, sizeof(socket_used));
	//memset(socket_timeout,0,sizeof(socket_timeout));
	//printf("network ok,mode=%d\n");
#if 0
	if(remote_service_par.devide_mode==MODE_STA){
		if(wac_configure_sucess==0){
			//start_hap_server();
		}
		else  if(wac_configure_sucess==2){
			wac_configure_sucess=0;
		}
#if 0				//i m not sure goto sentence,so use call func 
#if REMOTE_SERVER==JINGXUN_CLOUND			
				err=dns_gethostbyname(REMOTE_HOST, &remote_server_address, remote_dns_found, NULL);
			//remote_service_par.remote_status=REMOTE_DNS;		//fu add because test server needn,t dns
				if(err==ERR_INPROGRESS){
					remote_service_par.remote_status=REMOTE_DNS;
					printf("dns in progeress %s\n",__FUNCTION__);
				}else if (err == ERR_OK) {
					memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
					strcpy(remote_service_par.serverip,ipaddr_ntoa(&remote_server_address));
					printf("dns ok,ip=%s\n",remote_service_par.serverip);
			//remote_socket_create();    //try connect remote server
			//if(remote_service_par.remote_socket_fd >0){
			//	remote_service_par.remote_status=REMOTE_SERVER_QUERY;
			//}
				}else{
					printf("something wrong when dns,code=%d\n",err);
				}		
#endif

#if REMOTE_SERVER==JINGXUN_CLOUND

	if(remote_service_par.remote_status==REMOTE_WAIT)
		GetHostByName();
#endif
#endif

#ifdef 0
	TmrLed_SntpGetTime();
	Timer_LED_Onoff_Task();
#endif
	}
#endif
	//power_save_start();
//	Probe_init();
	//Group_probe_init();
	local_socket_create();
	while(1) {
		int socket_fd;
		//unsigned char buf[512];
		fd_set read_fds;
		struct timeval timeout;
		int ota_time=0;
		
		FD_ZERO(&read_fds);
		max_socket_fd=-1;
		timeout.tv_sec = SELECT_TIMEOUT_SOCKET;
		timeout.tv_usec = 0;
#if 0
		if(ota_started){
			vTaskDelay(1000 / portTICK_RATE_MS);
			ota_time++;
			if(ota_result==-1){
				printf("ota failed\n");
				ota_done_ack(ota_socket,1);
				vTaskDelay(2000 / portTICK_RATE_MS);
				ota_platform_reset();
			}
			else if(ota_result ==1){
				printf("ota sucess\n");
				ota_done_ack(ota_socket,2);
				vTaskDelay(2000 / portTICK_RATE_MS);
				ota_platform_reset();
			}
			if(ota_time>300){
				printf("ota timeout:%d,reset!!!\n");
				ota_done_ack(ota_socket,3);
				vTaskDelay(2000 / portTICK_RATE_MS);
				ota_platform_reset();
			}
			watchdog_refresh();		//test watchdog
			continue;
		}
#endif
#if 0
		if(remote_service_par.devide_mode==MODE_STA){
#if REMOTE_SERVER==JINGXUN_CLOUND
		if(dns_flag == 0) {
			if(dns_count == 0) {
				dns_count = DNS_Interval_Time;
				printf("time %d:%d:%d\n",sys_tm.tm_hour,sys_tm.tm_min,sys_tm.tm_sec);
				err=dns_gethostbyname(REMOTE_HOST, &remote_server_address, remote_dns_found, NULL);
			//remote_service_par.remote_status=REMOTE_DNS;		//fu add because test server needn,t dns
				if(err==ERR_INPROGRESS){
					remote_service_par.remote_status=REMOTE_DNS;
					printf("dns in progeress %s\n",__FUNCTION__);
				}else if (err == ERR_OK) {
					dns_flag = 1;
					memset(remote_service_par.serverip,0,sizeof(remote_service_par.serverip));
					strcpy(remote_service_par.serverip,ipaddr_ntoa(&remote_server_address));
					printf("dns ok,ip=%s\n",remote_service_par.serverip);
			//remote_socket_create();    //try connect remote server
			//if(remote_service_par.remote_socket_fd >0){
			//	remote_service_par.remote_status=REMOTE_SERVER_QUERY;
			//}
				}else{
					printf("something wrong when dns,code=%d\n",err);
				}
			}
		}
#endif
		}
#endif
#if 0
		if(remote_service_par.remote_socket_fd>=0)
		{
			socket_used[remote_service_par.remote_socket_fd]=1;
			//if(remote_service_par.remote_socket_fd > max_socket_fd)
			//	max_socket_fd=remote_service_par.remote_socket_fd;
		}
#endif

		if(local_server_fd>=0)
		{
			socket_used[local_server_fd]=1;
			//if(remote_service_par.local_server_fd > max_socket_fd)
			//	max_socket_fd=remote_service_par.local_server_fd;
		}

#if 0
		if(remote_service_par.probe_fd>=0)
		{
			socket_used[remote_service_par.probe_fd]=1;
			//if(remote_service_par.local_server_fd > max_socket_fd)
			//	max_socket_fd=remote_service_par.local_server_fd;
		}
#endif
#if	0
		if(remote_service_par.group_udp_fd>=0)
		{
			socket_used[remote_service_par.group_udp_fd]=1;
			//if(remote_service_par.local_server_fd > max_socket_fd)
			//	max_socket_fd=remote_service_par.local_server_fd;
		}
#endif
		for(socket_fd = 0; socket_fd < MAX_SOCKETS; socket_fd ++)
		{
			if(socket_used[socket_fd]){
				if(socket_fd>max_socket_fd)   
					max_socket_fd=socket_fd;
				FD_SET(socket_fd, &read_fds);
			}
		}

		if(select(max_socket_fd + 1, &read_fds, NULL, NULL, &timeout)) {
			for(socket_fd = 0; socket_fd < MAX_SOCKETS; socket_fd ++) {
				if(socket_used[socket_fd] && FD_ISSET(socket_fd, &read_fds)) {
					//socket_timeout[socket_fd]=0;
					if(socket_fd == local_server_fd) {
						struct sockaddr_in client_addr;
						unsigned int client_addr_size = sizeof(client_addr);
						int fd = accept(local_server_fd, (struct sockaddr *) &client_addr, &client_addr_size);
						if(fd >= 0 && fd< MAX_SOCKETS) {
							printf("^^^^^^^^^^^^^^^^^\n");
							printf("accept socket fd(%d)\n", fd);
							socket_used[fd] = 1;
							write_to_info_fd = fd;		//i need sure no another into it
							
							if(fd > max_socket_fd)
								max_socket_fd = fd;
						}
						else {
							printf("accept error\n");
							if(fd>0){
								printf("too much socket,so refuse connect,max=%d\n",fd);
								close(fd);
							}
						}
					}
					#if 0
					else if(socket_fd==remote_service_par.remote_socket_fd)
					{
						ret=m_RecvData_remote();
						if(ret<0){
							printf("remote socket close\n");
							remote_service_par.remote_socket_fd=-1;
							socket_used[socket_fd] = 0;
							close(socket_fd);
						}
					}
					#endif
					#if 0
					else if(socket_fd == remote_service_par.probe_fd)
					{
						Probe_proc();
					}
					#endif
					#if 0
					else if(socket_fd == remote_service_par.group_udp_fd)
					{
						printf("into group control\n");
						Group_probe_proc();
					}
					#endif
					else {
						ret=m_RecvData_local(socket_fd);
						if(ret<0){
							printf("local socket close\n");
							printf("\n\r%s(%d),  middle Available heap 0x%x\n", __FUNCTION__, __LINE__, xPortGetFreeHeapSize());
							socket_used[socket_fd] = 0;
							close(socket_fd);
						}
					}
				}
			}
			//test_count=0;
		}
		else {
#if 0
			if(test_count++ >300) {
				test_count = 0;
				if(test_flag==0)
					test_flag=1;
				else
					test_flag=0;
				printf("%d\n",test_flag);
				light_turn_onoff_handler(test_flag);
				printf("TCP server: no data in %d seconds  local_fd=%d,probe_fd=%d,remote status=%d\n", SELECT_TIMEOUT,remote_service_par.local_server_fd,remote_service_par.probe_fd,remote_service_par.remote_status);
			}
#endif

#if 0
			//continue_data_flag = 1;
			if(status_change_flag == 1)
			{
				status_change_flag =0;
				//continue_data_flag =0;
				initiative_report_status();			//initative status to app
			}
#endif
			//printf("TCP server: no data in %d seconds  local_fd=%d,probe_fd=%d,remote status=%d\n", SELECT_TIMEOUT,remote_service_par.local_server_fd,remote_service_par.probe_fd,remote_service_par.remote_status);
		}
#if 0
		start_remote_service();
#endif

#if 0
		if(system_reboot==1){
			system_reboot=0;
			system_soft_reset();
		}else if(wac_configure_sucess==1){
			wac_configure_sucess=2;
			for(ret=0;ret<MAX_SOCKETS;ret++){
				if(socket_used[ret]==1){
					close(ret);
					printf("close socket %d\n",ret);
				}
			}
			remote_service_par.devide_mode=MODE_STA;
			printf("wac config sucess,mode change!");
			goto restart_thread;
		}
#endif
//		watchdog_refresh();		//test watchdog
//		socket_timeout_process();				//can,t timeout accurate
		vTaskDelay(1000);
	}
	//vTaskDelete(NULL);
}
void tcp_socket_select(void)
{
	if(xTaskCreate(tcp_socket_select_thread, ((const char*)"tcp_socket_select_thread"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);		
}

