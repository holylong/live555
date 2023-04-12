extern "C"
{
	#include <libavcodec/avcodec.h>  
	#include <libavformat/avformat.h>  
	#include <libavutil/avutil.h>  
	#include <libswscale/swscale.h>  
};



//��ʼ��ffmpeg������
int H264_Init(void);

//ת�뺯��
int H264_2_RGB(unsigned char *inputbuf, int frame_size, unsigned char *outputbuf, unsigned int*outsize, int *nWidth, int *nHeight);

//�ͷ���Դ
void H264_Release(void);
