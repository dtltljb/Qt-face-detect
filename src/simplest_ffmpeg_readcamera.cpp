/**
 * 最简单的基于FFmpeg的AVDevice例子（读取摄像头）
 * Simplest FFmpeg Device (Read Camera)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了本地摄像头数据的获取解码和显示。是基于FFmpeg
 * 的libavdevice类库最简单的例子。通过该例子，可以学习FFmpeg中
 * libavdevice类库的使用方法。
 * 本程序在Windows下可以使用2种方式读取摄像头数据：
 *  1.VFW: Video for Windows 屏幕捕捉设备。注意输入URL是设备的序号，
 *          从0至9。
 *  2.dshow: 使用Directshow。注意作者机器上的摄像头设备名称是
 *         “Integrated Camera”，使用的时候需要改成自己电脑上摄像头设
 *          备的名称。
 * 在Linux下可以使用video4linux2读取摄像头设备。
 * 在MacOS下可以使用avfoundation读取摄像头设备。
 *
 * This software read data from Computer's Camera and play it.
 * It's the simplest example about usage of FFmpeg's libavdevice Library. 
 * It's suiltable for the beginner of FFmpeg.
 * This software support 2 methods to read camera in Microsoft Windows:
 *  1.gdigrab: VfW (Video for Windows) capture input device.
 *             The filename passed as input is the capture driver number,
 *             ranging from 0 to 9.
 *  2.dshow: Use Directshow. Camera's name in author's computer is 
 *             "Integrated Camera".
 * It use video4linux2 to read Camera in Linux.
 * It use avfoundation to read Camera in MacOS.
 * 
 */


#include <stdio.h>
#include <iostream>
//! thirty libs
#include "face_detection.h"
#include "face_identification.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//!user header
#include "face_verification_entry.h"
#include "face_detection_entry.h"
#include "face_alignment_entry.h"
#include "ffmpeg_YUV_to_JPEG.h"

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "SDL/SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <SDL/SDL.h>

#ifdef __cplusplus
};
#endif
#endif

//Output YUV420P 
#define OUTPUT_YUV420P 0
//'1' Use Dshow 
//'0' Use VFW
#define USE_DSHOW 0

using namespace std;

//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)



int thread_exit=0;
const char * face_detection_file = "/home/robot/Pictures/ljb.png";           //! second parament

//!
//! \brief CopyImageToYUV
//! \param pY
//! \param pU
//! \param pV
//! \param src
//! \param width
//! \param height
//!
void CopyImageToYUV(uint8_t *pY, uint8_t *pU, uint8_t *pV, uchar * src ,int width, int height)
{
    uint32_t size = width * height;
    memcpy(pY, src, size);
    memcpy(pU, src + size, size / 4);
    memcpy(pV, src + size + size / 4, size / 4);
}

//!
//! \brief CopyYUVToImage
//! \param dst
//! \param pY
//! \param pU
//! \param pV
//! \param width
//! \param height
//!
void CopyYUVToImage(uchar * dst ,uint8_t *pY, uint8_t *pU , uint8_t *pV,int width, int height)
{
    uint32_t size = width * height;
    memcpy(dst, pY, size);
    memcpy(dst + size, pU, size /4);
    memcpy(dst + size + size /4, pV, size / 4);
}

//!
//! \brief test
//!
void test()
{
    /**
    cv::Mat mainYuvImage;
    mainYuvImage.create( mainHeight * 3 / 2, mainWidth, CV_8UC1);
    CopyYUVToImage(mainYuvImage.data, mainY, mainU, mainV, mainWidth, mainHeight);

    cv::Mat mainRgbImage;
    cv::cvtColor(mainYuvImage, mainRgbImage, CV_YUV2BGR_I420);

    cv::Mat subYuvImage;
    subYuvImage.create(subHeight * 3 / 2,subWidth,  CV_8UC1);
    CopyYUVToImage(subYuvImage.data, subY, subU, subV, subWidth, subHeight);

    cv::Mat subRgbImage;
    cv::cvtColor(subYuvImage, subRgbImage, CV_YUV2BGR_I420);
    */
}

//!
//! \brief sfp_refresh_thread
//! \param opaque
//! \return
//!
int sfp_refresh_thread(void *opaque)
{
	thread_exit=0;
	while (!thread_exit) {
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	thread_exit=0;
	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}


//!
//! \brief show_dshow_device
//!
void show_dshow_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_devices","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("========Device Info=============\n");
	avformat_open_input(&pFormatCtx,"video=dummy",iformat,&options);
	printf("================================\n");
}

//!
//! \brief show_dshow_device_option
//!
void show_dshow_device_option(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_options","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("========Device Option Info======\n");
	avformat_open_input(&pFormatCtx,"video=Integrated Camera",iformat,&options);
	printf("================================\n");
}

//!
//! \brief show_vfw_device
//!
void show_vfw_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVInputFormat *iformat = av_find_input_format("vfwcap");
	printf("========VFW Device Info======\n");
	avformat_open_input(&pFormatCtx,"list",iformat,NULL);
	printf("=============================\n");
}

//!
//! \brief show_avfoundation_device
//!
void show_avfoundation_device(){
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("avfoundation");
    printf("==AVFoundation Device Info===\n");
    avformat_open_input(&pFormatCtx,"",iformat,&options);
    printf("=============================\n");
}

//!
//! \brief main
//! \param argc
//! \param argv
//! \return
//!
int main(int argc, char* argv[])
{

//    ffmpeg_YUV_to_JPEG(NULL);
//    while(1);

	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	
	//Open File
	//char filepath[]="src01_480x272_22.h265";
	//avformat_open_input(&pFormatCtx,filepath,NULL,NULL)

	//Register Device
	avdevice_register_all();

//Windows
#ifdef _WIN32

	//Show Dshow Device
	show_dshow_device();
	//Show Device Options
	show_dshow_device_option();
    //Show VFW Options
    show_vfw_device();

#if USE_DSHOW
	AVInputFormat *ifmt=av_find_input_format("dshow");
	//Set own video device's name
	if(avformat_open_input(&pFormatCtx,"video=Integrated Camera",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#else
	AVInputFormat *ifmt=av_find_input_format("vfwcap");
	if(avformat_open_input(&pFormatCtx,"0",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#endif
#elif defined linux
    //Linux

	AVInputFormat *ifmt=av_find_input_format("video4linux2");
	if(avformat_open_input(&pFormatCtx,"/dev/video0",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#else
    show_avfoundation_device();
    //Mac
    AVInputFormat *ifmt=av_find_input_format("avfoundation");
    //Avfoundation
    //[video]:[audio]
    if(avformat_open_input(&pFormatCtx,"0",ifmt,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
#endif


	if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	if(videoindex==-1)
	{
		printf("Couldn't find a video stream.\n");
		return -1;
	}
    //! by encoder id find decoder tool
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}
	AVFrame	*pFrame,*pFrameYUV;
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
    //! @note output YUV picture in out_buffer
//    unsigned char *out_buffer=(unsigned char *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
//    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
    ///< SDL----------------------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
    }
    int screen_w=0,screen_h=0;
	SDL_Surface *screen; 
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;
    screen_w = 640;
    screen_h = 480;
    screen = SDL_SetVideoMode(screen_w, screen_h, 0,SDL_RESIZABLE | SDL_NOFRAME );

	if(!screen) {  
		printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());  
		return -1;
	}
    SDL_Overlay *bmp_sdl_overlay;
    bmp_sdl_overlay = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height, SDL_YV12_OVERLAY, screen);
	SDL_Rect rect;
	rect.x = 0;    
	rect.y = 0;    
    rect.w = screen_w/2;
    rect.h = screen_h/2;
//    bmp_sdl_overlay->w = screen_w;
//    bmp_sdl_overlay->h = screen_h;
    ///> SDL End------------------------
	int ret, got_picture;

	AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));

#if OUTPUT_YUV420P 
    FILE *fp_yuv=fopen("output_480_272.yuv","wb+");
#endif  

	struct SwsContext *img_convert_ctx;

//    //Init Method 1
//    img_convert_ctx =sws_alloc_context();
//    //Show AVOption
//    av_opt_show2(img_convert_ctx,stdout,AV_OPT_FLAG_VIDEO_PARAM,0);
//    //Set Value
//     av_opt_set_int(img_convert_ctx,"sws_flags",SWS_BICUBIC|SWS_PRINT_INFO,0);
//     av_opt_set_int(img_convert_ctx,"srcw",src_w,0);
//     av_opt_set_int(img_convert_ctx,"srch",src_h,0);
//     av_opt_set_int(img_convert_ctx,"src_format",src_pixfmt,0);
//        //'0' for MPEG (Y:0-235);'1' for JPEG (Y:0-255)
//     av_opt_set_int(img_convert_ctx,"src_range",1,0);
//     av_opt_set_int(img_convert_ctx,"dstw",dst_w,0);
//     av_opt_set_int(img_convert_ctx,"dsth",dst_h,0);
//     av_opt_set_int(img_convert_ctx,"dst_format",dst_pixfmt,0);
//     av_opt_set_int(img_convert_ctx,"dst_range",1,0);
//     sws_init_context(img_convert_ctx,NULL,NULL);

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUVJ420P,\
                                     SWS_BICUBIC,NULL, NULL, NULL);
	//------------------------------
	SDL_Thread *video_tid = SDL_CreateThread(sfp_refresh_thread,NULL);
    //!
    //! \brief face_detection
    //!
//    SDL_Thread *face_detection = SDL_CreateThread(face_detection_entry,(void*)face_detection_file);
//    SDL_Thread *face_alignment = SDL_CreateThread(face_alignment_entry,(void*)face_detection_file);
//    SDL_Thread * face_verification = SDL_CreateThread(face_verification_entry,NULL);
    SDL_Thread * yuv_to_jpeg = SDL_CreateThread(ffmpeg_YUV_to_JPEG,NULL);
	//
	SDL_WM_SetCaption("Simplest FFmpeg Read Camera",NULL);
	//Event Loop
    int bmp_event_count = 0;
    SDL_Event event;
    SDL_UserEvent emit_bmp_event;

	for (;;) {
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SFM_REFRESH_EVENT){
            ///> @note get camera data to packet
			if(av_read_frame(pFormatCtx, packet)>=0){
				if(packet->stream_index==videoindex){
                    ///> decode from packet buffer to pFrame buffer
					ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if(ret < 0){
						printf("Decode Error.\n");
						return -1;
					}
					if(got_picture){
                        SDL_LockYUVOverlay(bmp_sdl_overlay);                ///< sdl event() ,every 40ms capture screen

                        ///> @note : pointer ==> overlay->pixels, same source
                        pFrameYUV->data[0]=bmp_sdl_overlay->pixels[0];
                        pFrameYUV->data[1]=bmp_sdl_overlay->pixels[2];
                        pFrameYUV->data[2]=bmp_sdl_overlay->pixels[1];
                        pFrameYUV->linesize[0]=bmp_sdl_overlay->pitches[0];
                        pFrameYUV->linesize[1]=bmp_sdl_overlay->pitches[2];
                        pFrameYUV->linesize[2]=bmp_sdl_overlay->pitches[1];

                        ///>@note from pFrame buffer to pFrameYUV pointer buffer(bmp_sdl_overlay data area).
                        pFrameYUV->width = bmp_sdl_overlay->w;
                        pFrameYUV->height = bmp_sdl_overlay->h;
                        sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0,
                                                    pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

#if OUTPUT_YUV420P  
						int y_size=pCodecCtx->width*pCodecCtx->height;    
						fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y   
						fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U  
						fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V  
#endif  
                        ///> @note  : add  AV_PIX_FMT_YUVJ420P convert to AV_PIX_FMT_BGR24

                        bmp_event_count += 1;
                        if( bmp_event_count >= 25){

                            //av_frame_to_bmp((uint8_t *)pFrameYUV->data[0], pFrameYUV->width, pFrameYUV->height, bmp_event_count);
                           // ffmpeg_YUV_to_JPEG((void*)pFrameYUV);
                            emit_bmp_event.type = FACE_DETECTION_EVENT;
                            emit_bmp_event.code = 0x0001;
                            emit_bmp_event.data1 = (void*)pFrameYUV;
                            SDL_PushEvent((SDL_Event*)(&emit_bmp_event));
                            bmp_event_count = 0;
                            cout << "emit bmp event: " << event.type << '\n';
                        }
                        SDL_UnlockYUVOverlay(bmp_sdl_overlay);

                        SDL_DisplayYUVOverlay(bmp_sdl_overlay, &rect);

					}
				}
				av_free_packet(packet);
			}else{
				//Exit Thread
				thread_exit=1;
			}
        }else if( (event.type == SDL_KEYDOWN)||(event.type == SDL_KEYUP) ){ /// enter key exit 1
			thread_exit=1;
		}else if(event.type==SFM_BREAK_EVENT){
			break;
        }else{
            SDL_Delay(35);
        }
	}

	sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

	SDL_Quit();

//    av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

