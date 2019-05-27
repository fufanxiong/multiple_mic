#include <stdio.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"


#include "mp3parse.h"
//#include "ttsmain.h"

#define cycle_buf_len	2048
#define MP3_FRAME_HEAD_LEN	4

unsigned char parse_buf[cycle_buf_len];
int start_pos = 0 ,end_pos = 0;
int start_len = 0 ,end_len = 0;

unsigned char parse_flag=0;		//0:not parse , 1: parse but not sure , 2:parse and ensure
unsigned int FrameSize;

int FrameHeadRecv[4];

QueueHandle_t parse_recv_mp3_queue;


/*
	i find there are some different, i take one of these representations
*/
unsigned int rate_bit_val[][5]={
0,	0,	0,	0,	0,
32,	32,	32,	32,	8,
64,	48,	40,	48,	16,
96,	56,	48,	56,	24,
128,64,	56,	64,	32,
160,80,	64,	80,	40,
192,96,	80,	96,	48,
224,112,96,	112,56,
256,128,112,128,64,
288,160,128,144,80,
320,192,160,160,96,
352,224,192,176,112,
384,256,224,192,128,
416,320,256,224,144,
448,384,320,256,160
};

unsigned int sample_frequence[][3]={
44100,	22050,	11025,
48000,	24000,	12000,
32000,	16000,	8000
};

unsigned int every_frame_date_len[][3]={
384,	384,	384,
1152,	1152,	1152,
1152,	576,	576
};

void mp3_parse_recv(void *param)
{
	RING_CACHE_T emp_entity;
	LPRING_CACHE_T tmp_recv = &emp_entity;
	char pbuf[1024];
	while(1)
	{
		if(xQueueReceive(parse_recv_mp3_queue,&tmp_recv,5000/portTICK_RATE_MS))
		{
			//printf("mp3 queue has data push\n");
			printf("mp3 queue recv addr=%p,recv all data=%d,recv start addr=%p,start len=%d\n", \
					tmp_recv->recv_addr,tmp_recv->recv_len,tmp_recv->start_addr,tmp_recv->from_start_len);
		}
		else
		{
			printf("queue last 5 s time dont have data\n");
		}
	}

}

void init_queue_to_recv_parse_mp3(void)
{
	int SetQueueLength = 1;
	int SetItemSize;
	SetItemSize = sizeof(LPRING_CACHE_T);
	parse_recv_mp3_queue = xQueueCreate(SetQueueLength,SetItemSize);

	if(parse_recv_mp3_queue != NULL)
	{
		if(xTaskCreate(mp3_parse_recv,((const char*)"mp3 parse recv"),1024,NULL,tskIDLE_PRIORITY+5,NULL) != pdTRUE)
		{
			printf("create parse mp3 recv task failed\n");
		}
	}
	else
	{
		printf("mp3 recv queue create faild");
	}

}


#if 0
int GetRateBit(int RateBitIndex, int version, int layer)
{
	//ASSERT(x);
	int BitRateVal;
	int row_index = 0;
	int column_index = 0;
	row_index = RateBitIndex;

	if(version == MPEG1)
	{
		if(layer == LayerI)
		{
			column_index = 0;
		}
		else if(layer == LayerII)
		{
			column_index = 1;
		}
		else if(layer == LayerIII)
		{
			column_index = 2; 
		}
		else
		{
			printf("layer is unknow\n");
		}
	}
	else if((version == MPEG2) || (version == MPEG2P5))
	{
		if(layer == LayerI)
		{
			column_index = 3;
		}
		else if((layer == LayerII) || (layer == LayerIII))
		{
			column_index = 4;
		}
		else
		{
			printf("layer is unknow\n");
		}
	}
	else
	{
		printf("unknow version\n");
	}
#if 1
	if(RateBitIndex == 0)
	{
		printf("bit rate is variable\n");
		BitRateVal = 0;
	}
	else if((RateBitIndex > 0) && (RateBitIndex < 0xf))
	{
		printf("bit index=%d,column=%d\n",RateBitIndex,column_index);
		BitRateVal = rate_bit_val[RateBitIndex][column_index];
	}
	else if(RateBitIndex == 0xf)
	{
		printf("bad rate\n");
		//BitRateVal = 0xff;
	}
#else
	switch(index)
	{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
	}
#endif

	return BitRateVal;
}
#else
int GetRateBit(const FRAME_HEAD_INFO *parse_info)
{
	//ASSERT(x);
	int BitRateVal;
	int row_index = 0;
	int column_index = 0;
	row_index = parse_info->bitrate;
	unsigned int fre;
	unsigned char fre_ind;
	unsigned char fre_index;	//Êý×éindex
	unsigned char valid_frame_data_index;
	unsigned int padding;
	unsigned int valid_frame_data;
	unsigned int frame_size_get;

	padding = parse_info->frame_adj;
	
	if(parse_info->layer == LayerI)
	{
		valid_frame_data_index = 0;
		padding = padding*4;
	}
	else if(parse_info->layer == LayerII)
	{
		valid_frame_data_index = 1;
	}
	else if(parse_info->layer == LayerIII)
	{ 
		valid_frame_data_index = 2;
	}
	
	if(parse_info->ver == MPEG1)
	{
		if(parse_info->layer == LayerI)
		{
			column_index = 0;
		}
		else if(parse_info->layer == LayerII)
		{
			column_index = 1;
		}
		else if(parse_info->layer == LayerIII)
		{
			column_index = 2;
		}
		else
		{
			printf("layer is unknow\n");
		}

		fre_index = 0;
	}
	else if((parse_info->ver == MPEG2) || (parse_info->ver == MPEG2P5))
	{
		if(parse_info->layer == LayerI)
		{
			column_index = 3;
		}
		else if((parse_info->layer == LayerII) || (parse_info->layer == LayerIII))
		{
			column_index = 4;
		}
		else
		{
			printf("layer is unknow\n");
		}

		if(parse_info->ver == MPEG2)
		{
			fre_index = 1;
		}
		else
		{
			fre_index = 2;
		}
	}
	else
	{
		printf("unknow version\n");
	}
	

#if 1
	if(row_index == 0)
	{
		printf("bit rate is variable\n");
		BitRateVal = 0;
	}
	else if((row_index > 0) && (row_index < 0xf))
	{
		printf("bit index=%d,column=%d\n",row_index,column_index);
		BitRateVal = rate_bit_val[row_index][column_index];
		fre_ind = parse_info->sample_fre;
		if(fre_ind >= 3)
		{
			printf("this is unknow fre_ind\n");
		}
		else
		{
			fre = sample_frequence[fre_ind][fre_index];
			valid_frame_data = every_frame_date_len[valid_frame_data_index][fre_index];
			frame_size_get = (valid_frame_data/8)*BitRateVal*1000/fre;
		}
		
	}
	else if(row_index == 0xf)
	{
		printf("bad rate\n");
		//BitRateVal = 0xff;
	}
#else
	switch(index)
	{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
	}
#endif

	return frame_size_get;
}


#endif

//return frame size
//
//int parse_frame_head( const LPMP3_FRAME_HEAD_H FrameHead, LPFRAME_HEAD_INFO ParseInfo)
int parse_frame_head( const FrameIntAndByte FrameHead, LPFRAME_HEAD_INFO ParseInfo)
{
	FRAME_HEAD_INFO tmp_frame_info;
	int frame_size;
	//ASSERT(x);
	//if(FrameHead != NULL)
	{
#if 0
			tmp_frame_info.sync_info = FrameHead.frame_head_byte.sync & 0xffff;
			tmp_frame_info.ver = FrameHead.frame_head_byte.version & 0xf;
			tmp_frame_info.layer = FrameHead.frame_head_byte.layer & 0xf;
			tmp_frame_info.crc_flag = FrameHead.frame_head_byte.if_crc & 0xf;
			tmp_frame_info.bitrate = FrameHead.frame_head_byte.bitrate_index & 0xf;
			tmp_frame_info.sample_fre = FrameHead.frame_head_byte.sample_fre & 0xf;
			tmp_frame_info.frame_adj = FrameHead.frame_head_byte.padding & 0xf;
			tmp_frame_info.reserve = FrameHead.frame_head_byte.private & 0xf;
			tmp_frame_info.voice_mode = FrameHead.frame_head_byte.mode & 0xf;
			tmp_frame_info.mode_ext = FrameHead.frame_head_byte.mode_extension & 0xf;
			tmp_frame_info.copyright = FrameHead.frame_head_byte.copyright & 0xf;
			tmp_frame_info.origin = FrameHead.frame_head_byte.original & 0xf;
			tmp_frame_info.emphasis = FrameHead.frame_head_byte.emphasis & 0xf;
#else
			tmp_frame_info.sync_info = FrameHead.frame_head_byte.sync;
			tmp_frame_info.ver = FrameHead.frame_head_byte.version;
			tmp_frame_info.layer = FrameHead.frame_head_byte.layer;
			tmp_frame_info.crc_flag = FrameHead.frame_head_byte.if_crc;
			tmp_frame_info.bitrate = FrameHead.frame_head_byte.bitrate_index;
			tmp_frame_info.sample_fre = FrameHead.frame_head_byte.sample_fre;
			tmp_frame_info.frame_adj = FrameHead.frame_head_byte.padding;
			tmp_frame_info.reserve = FrameHead.frame_head_byte.private;
			tmp_frame_info.voice_mode = FrameHead.frame_head_byte.mode;
			tmp_frame_info.mode_ext = FrameHead.frame_head_byte.mode_extension;
			tmp_frame_info.copyright = FrameHead.frame_head_byte.copyright;
			tmp_frame_info.origin = FrameHead.frame_head_byte.original;
			tmp_frame_info.emphasis = FrameHead.frame_head_byte.emphasis;
#endif

			if(ParseInfo != NULL)
			{
				memcpy(ParseInfo,&tmp_frame_info,sizeof(FRAME_HEAD_INFO));
			}
			printf("sync=%d,ver=%d,layer=%d,crc=%d\n",tmp_frame_info.sync_info,tmp_frame_info.ver,\
													tmp_frame_info.layer,tmp_frame_info.crc_flag);

			printf("bit rate=%d,sample=%d,padding=%d\n",tmp_frame_info.bitrate,tmp_frame_info.sample_fre,tmp_frame_info.frame_adj);

			//frame_size = GetRateBit(tmp_frame_info.bitrate,tmp_frame_info.ver,tmp_frame_info.layer);
			frame_size = GetRateBit(&tmp_frame_info);
	}
	//else
	//{
	//	printf("frame head is null\n");
	//	frame_size = 0;
	//}



	return frame_size;

}

//int parse_mp3_frame(int packet_id,char *aux_buf,int aux_len)
int parse_mp3_frame(unsigned char *aux_buf,int aux_len)
{
	int ret = 1;
	int tmp_len,head_pos=0;
	//int FrameHead[4];
	LPMP3_FRAME_HEAD_H parse_head;
	void *head=NULL;
	RING_CACHE_T tmp_entity;
	LPRING_CACHE_T tmp_info = &tmp_entity;
	int tmp_reserve_len;
	FrameIntAndByte size_x;
	
#if 0
	if(packet == 1)
	{
		start_pos = 0;
		end_pos = 0;
		printf("first packet\n");
	}
	else if(packet == 0)
	{
		printf("last packet\n");
	}
	else
	{
			
	}
#endif

#if 0
	if((end_pos + aux_len) < cycle_buf_len)
	{
		memcpy(parse_buf+end_pos,aux_buf,aux_len);
		end_pos += (aux_len-1);
	}
	else
	{
		tmp_len = aux_len+end_pos-cycle_buf_len;
		memcpy(parse_buf+end_pos,aux_buf,tmp_len);
		memcpy(parse_buf,aux_buf+tmp_len,aux_len-tmp_len);
		end_pos = aux_len-tmp_len;
	}
#else
	if(end_len + aux_len <= cycle_buf_len)
	{
		memcpy(parse_buf+end_len,aux_buf,aux_len);
		end_pos += (aux_len-1);
		end_len += aux_len;
	}
	else
	{
		tmp_len = cycle_buf_len - end_len;
		memcpy(parse_buf+end_len,aux_buf,tmp_len);
		memcpy(parse_buf,aux_buf+tmp_len,aux_len-tmp_len);
		end_len = aux_len-tmp_len;
		end_pos = end_len - 1;
	}
#endif
	do
	{
		if((parse_flag == 0))
		{
			//todo
			//we need judge this head need more than 4
			//head = parse_buf + start_pos + head_pos;
			//parse_head = (LPMP3_FRAME_HEAD_H)(head);
			printf("start = %u,%u,%u,%u\n",parse_buf[start_pos],parse_buf[start_pos+1],parse_buf[start_pos+2],parse_buf[start_pos+3]);
			size_x.frame_head_int = (parse_buf[start_pos] << 24) + (parse_buf[start_pos+1] << 16) + (parse_buf[start_pos+2] << 8) + parse_buf[start_pos+3];
#if 0
			int sync_info = parse_head->sync & 0xff;
			int ver = parse_head->version & 0xf;
			int layer = parse_head->layer & 0xf;
			int crc_flag = parse_head->if_crc & 0xf;
			int bitrate = parse_head->bitrate_index & 0xf;
			int sample_fre = parse_head->sample_fre & 0xf;
			int frame_adj = parse_head->padding & 0xf;
			int reserve = parse_head->private & 0xf;
			int voice_mode = parse_head->mode & 0xf;
			int mode_ext = parse_head->mode_extension & 0xf;
			int copyright = parse_head->copyright & 0xf;
			int origin = parse_head->original & 0xf;
			int emphasis = parse_head->emphasis & 0xf;
#endif
			//FrameSize =	parse_frame_head(parse_head,NULL);
			printf("parse head size if =%u\n",size_x.frame_head_int);
			FrameSize =	parse_frame_head(size_x,NULL);

			printf("get framesize=%d\n",FrameSize);
			if(FrameSize > 0)
			{
				parse_flag = 1;
				//memcpy(FrameHeadRecv,head,MP3_FRAME_HEAD_LEN);
			}

			//we should ensure whether we recv right
#if 0
			if((FrameSize + start + MP3_FRAME_HEAD_LEN) < end_pos)
			{
				memcmp
			}
#endif				
		}

		//todo
		//do like this is not correct
#if 0
		if(start_pos > end_pos)
		{
			tmp_reserve_len = cycle_buf_len - start_pos + end_pos;
		}
		else
		{
			tmp_reserve_len = end_pos - start_pos;
		}
#else
		if(start_len > end_len)
		{
			tmp_reserve_len = cycle_buf_len - start_len + end_len;
		}
		else
		{
			tmp_reserve_len = end_len - start_len;
		}
#endif
		//if(FrameSize > MP3_FRAME_HEAD_LEN)
		//{
			
		//}
		if(tmp_reserve_len >= FrameSize)
		{
				//this should not use addr for ring cache 

				if((start_len + FrameSize) <= cycle_buf_len)
				{
					tmp_info->recv_addr = parse_buf + start_len + MP3_FRAME_HEAD_LEN;
					tmp_info->from_start_len = 0;
					//tmp_info->len = FrameSize-MP3_FRAME_HEAD_LEN;
				}
				else
				{
					if((start_len + MP3_FRAME_HEAD_LEN) < cycle_buf_len)
					{
						tmp_info->recv_addr = parse_buf + start_len + MP3_FRAME_HEAD_LEN;
						tmp_info->start_addr = parse_buf;
						//tmp_info->from_start_len = (FrameSize - MP3_FRAME_HEAD_LEN)  - (cycle_buf_len - (start_len + MP3_FRAME_HEAD_LEN));
						tmp_info->from_start_len = FrameSize - (cycle_buf_len - start_len);
					}
					else
					{
						tmp_info->recv_addr = parse_buf + (start_len + MP3_FRAME_HEAD_LEN) - cycle_buf_len;
						tmp_info->from_start_len = 0;
					}
				}
				tmp_info->recv_len = FrameSize - MP3_FRAME_HEAD_LEN;

				printf("start_len=%d,start_pos=%d,end_len=%d,end_pos=%d\n",start_len,start_pos,end_len,end_pos);
				
				xQueueSend(parse_recv_mp3_queue,(void *)&tmp_info,1000/portTICK_RATE_MS);
				start_len += FrameSize;
#if 0
				if(start_len > cycle_buf_len)
				{
					start_len = start_len - cycle_buf_len;
					start_pos = start_len;
				}
				else if(start_len == cycle_buf_len)
				{
					start_pos = 0;
				}
				else
				{
					start_pos = start_len;
				}
#else
				if(start_len == cycle_buf_len)
				{
					start_pos = 0;
				}
				else
				{
					if(start_len > cycle_buf_len)
					{
						start_len = start_len - cycle_buf_len;
					}
					
					start_pos = start_len;
				}
#endif
				
				//start_len = start_pos;
				
		}
		else
		{
			break;
		}
	}while(1);

#if 0
	do
	{
		head = parse_buf + start + head_pos;
		parse_head = (LPMP3_FRAME_HEAD_H)(head);
		//memset(FrameHead,0,4);
		//memcpy(FrameHead,head,4);
		
	}while();
#endif
	
	
	
	
	return ret;
}
