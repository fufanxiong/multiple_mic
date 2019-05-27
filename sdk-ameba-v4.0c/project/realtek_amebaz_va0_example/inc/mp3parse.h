#ifndef __MP3PARSE_H__
#define __MP3PARSE_H__

typedef struct
{
#if 0
	unsigned int sync:11;			//ͬ����Ϣ
	unsigned int version:2;			//�汾
	unsigned int layer:2;			//��
	unsigned int if_crc:1;			//crcУ��
	unsigned int bitrate_index:4;	//λ��
	unsigned int sample_fre:2;		//������
	unsigned int padding:1;			//���
	unsigned int private:1;			//����
	unsigned int mode:2;			//ģʽ
	unsigned int mode_extension:2;	//ģʽ��չ
	unsigned int copyright:1;		//��Ȩ
	unsigned int original:1;		//ԭ���־
	unsigned int emphasis:2;		//ǿ��ģʽ
#else
	
	unsigned int emphasis:2;		//ǿ��ģʽ
	unsigned int original:1;		//ԭ���־
	unsigned int copyright:1;		//��Ȩ
	unsigned int mode_extension:2;	//ģʽ��չ
	unsigned int mode:2;			//ģʽ
	unsigned int private:1;			//����
	unsigned int padding:1;			//���
	unsigned int sample_fre:2;		//������
	unsigned int bitrate_index:4;	//λ��
	unsigned int if_crc:1;			//crcУ��
	unsigned int layer:2;			//��
	unsigned int version:2;			//�汾
	unsigned int sync:11;			//ͬ����Ϣ
	
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