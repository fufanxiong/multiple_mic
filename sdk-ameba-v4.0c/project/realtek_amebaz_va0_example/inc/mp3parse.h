#ifndef __MP3PARSE_H__
#define __MP3PARSE_H__

typedef struct
{
#if 0
	unsigned int sync:11;			//同步信息
	unsigned int version:2;			//版本
	unsigned int layer:2;			//层
	unsigned int if_crc:1;			//crc校验
	unsigned int bitrate_index:4;	//位率
	unsigned int sample_fre:2;		//采样率
	unsigned int padding:1;			//填充
	unsigned int private:1;			//保留
	unsigned int mode:2;			//模式
	unsigned int mode_extension:2;	//模式拓展
	unsigned int copyright:1;		//版权
	unsigned int original:1;		//原版标志
	unsigned int emphasis:2;		//强调模式
#else
	
	unsigned int emphasis:2;		//强调模式
	unsigned int original:1;		//原版标志
	unsigned int copyright:1;		//版权
	unsigned int mode_extension:2;	//模式拓展
	unsigned int mode:2;			//模式
	unsigned int private:1;			//保留
	unsigned int padding:1;			//填充
	unsigned int sample_fre:2;		//采样率
	unsigned int bitrate_index:4;	//位率
	unsigned int if_crc:1;			//crc校验
	unsigned int layer:2;			//层
	unsigned int version:2;			//版本
	unsigned int sync:11;			//同步信息
	
#endif
}MP3_FRAME_HEAD_H,*LPMP3_FRAME_HEAD_H;

typedef union
{
	unsigned int frame_head_int;
	MP3_FRAME_HEAD_H frame_head_byte;
}FrameIntAndByte;

#if 0
typedef struct
{
	
	unsigned int bitrate;

}MP3_FRAME_INFO;
#endif

typedef struct
{
	unsigned int sync_info;
	unsigned int ver;
	unsigned int layer;
	unsigned int crc_flag;
	unsigned int bitrate;
	unsigned int sample_fre;
	unsigned int frame_adj;
	unsigned int reserve;
	unsigned int voice_mode;
	unsigned int mode_ext;
	unsigned int copyright;
	unsigned int origin;
	unsigned int emphasis;
}FRAME_HEAD_INFO,*LPFRAME_HEAD_INFO;

enum MPEG_TYPE
{
 	MPEG1=3,
	MPEG2=2,
	MPEG2P5=0
};

enum LAYOR_TYPE
{
	LayerI=3,
	LayerII=2,
	LayerIII=1
};

typedef struct
{
	void *recv_addr;		//start recv data addr
	void *start_addr;		//when we read end,what is the start addr
	int  recv_len;			//all recv len
	int  from_start_len;	//when we have to afresh to read data because we have read ring cache end,so we need kown how remaining we need 

}RING_CACHE_T,*LPRING_CACHE_T;

#endif