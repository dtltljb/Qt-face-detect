
#ifdef __cplusplus
 #define __STDC_CONSTANT_MACROS
 #ifdef _STDINT_H
  #undef _STDINT_H
 #endif
 # include <stdint.h>
#endif

extern "C"
{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libavutil/log.h>
#include<libswscale/swscale.h>
}

#include<stdio.h>
//#include <windows.h> //for saveAsBitmap

bool saveAsBitmap(AVFrame *pFrameRGB, int width, int height, int iFrame)
{

      FILE *pFile = NULL;
      BITMAPFILEHEADER bmpheader;
      BITMAPINFO bmpinfo;

      char fileName[32];
      int bpp = 24;

      // open file
      sprintf(fileName, "frame%d.bmp", iFrame);
      pFile = fopen(fileName, "wb");
      if (!pFile)
            return false;

      bmpheader.bfType = ('M' <<8)|'B';
      bmpheader.bfReserved1 = 0;
      bmpheader.bfReserved2 = 0;
      bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
      bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8;

      bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmpinfo.bmiHeader.biWidth = width;
      bmpinfo.bmiHeader.biHeight = -height; //reverse the image
      bmpinfo.bmiHeader.biPlanes = 1;
      bmpinfo.bmiHeader.biBitCount = bpp;
      bmpinfo.bmiHeader.biCompression = BI_RGB;
      bmpinfo.bmiHeader.biSizeImage = 0;
      bmpinfo.bmiHeader.biXPelsPerMeter = 100;
      bmpinfo.bmiHeader.biYPelsPerMeter = 100;
      bmpinfo.bmiHeader.biClrUsed = 0;
      bmpinfo.bmiHeader.biClrImportant = 0;

      fwrite(&bmpheader, sizeof(BITMAPFILEHEADER), 1, pFile);
      fwrite(&bmpinfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
      uint8_t *buffer = pFrameRGB->data[0];
      for (int h=0; h<height; h++)
      {
            for (int w=0; w<width; w++)
            {
                  fwrite(buffer+2, 1, 1, pFile);
                  fwrite(buffer+1, 1, 1, pFile);
                  fwrite(buffer, 1, 1, pFile);

                  buffer += 3;
            }
      }
      fclose(pFile);

      return true;
}

int main(int argc, char** argv)
{
    const char* filename = argc > 1 ? argv[1] : "flower_cif.yuv";

    FILE* fin = fopen(filename, "rb");
    if( fin == NULL )
    {
        printf("can't open the file\n");
        return -1;
    }

    int width = 352;
    int height = 288;

    AVPixelFormat src_fmt = AV_PIX_FMT_YUV420P;
    AVPixelFormat dst_fmt = AV_PIX_FMT_YUYV422;


    AVFrame* src_frame = av_frame_alloc();
    AVFrame* dst_frame = av_frame_alloc();
    if( src_frame == NULL || dst_frame == NULL )
    {
        printf("av_frame_alloc fail\n");
        return -1;
    }

    //这里FFmpeg会帮我们计算这个格式的图片，需要多少字节来存储
    //相当于前面例子中的width * height * 2
    int src_bytes_num = avpicture_get_size(src_fmt,
                                           width, height);
    int dst_bytes_num = avpicture_get_size(dst_fmt,
                                           width, height);

    //申请空间来存放图片数据。包含源数据和目标数据
    uint8_t* src_buff = (uint8_t*)av_malloc(src_bytes_num);
    uint8_t* dst_buff = (uint8_t*)av_malloc(dst_bytes_num);

    //前面的av_frame_alloc函数，只是为这个AVFrame结构体分配了内存，
    //而该类型的指针指向的内存还没分配。这里把av_malloc得到的内存和AVFrame关联起来。
    //当然，其还会设置AVFrame的其他成员
    avpicture_fill((AVPicture*)src_frame, src_buff, src_fmt,  width, height);     //该函数会自动填充AVFrame的data和linesize字段

    avpicture_fill((AVPicture*)dst_frame, dst_buff, dst_fmt,
                   width, height);


    //这里主要说明linesize这个成员的含义。不想看可以忽略
    //YUV格式中有一个很重要的等量关系，那就是有多少个像素就有多少个y。
    //linesize正如其名，一条线（即一行）的大小。对于yuv420p（planar）。data[0]存放的是y，对应地linesize[0]就
    //指明一行有多少个y。对于352*288的图像，一行有352个像素。根据刚才的等量关系。那么linesize[0]就
    //应该为352.即一行有352个y。对于linesize[1]，因为data[1]存放的是u。而一行352个像素在yuv420p格式中，
    //其只需352/2，即176个。所以linesize[1]的大小为176。同理linesize[2]也为176。

    //而对于yuyv422格式。data[0]这一行要负责存放y、u、v这三个分量。而y:u:v = 2:1:1的关系。根据前面所说的
    //等量关系，y等于352(相对于352*288大小的图像来说)，u和v都等于352/2 。所以u+v等于352。所以linesize[0]
    //等于352*2.
    printf("%d %d %d\n", src_frame->linesize[0],  src_frame->linesize[1], src_frame->linesize[2]);
    printf("%d %d %d \n", dst_frame->linesize[0],  dst_frame->linesize[1], dst_frame->linesize[2]);


    //对转换进行配置。这里要设置转换源的大小、格式和转换目标的大小、格式
    //设置后，下面就可以直接使用sws_scale函数，进行转换
    SwsContext* sws_ctx = sws_getContext(width, height,
                                         src_fmt,
                                         width, height,
                                         dst_fmt,
                                         SWS_BICUBIC,
                                         //SWS_BILINEAR,
                                         NULL,
                                         NULL,
                                         NULL);

    if( sws_ctx == NULL)
    {
        printf("sws_getContext fail ");
        return -1;
    }


    FILE* fout = fopen("yuyv422.yuv", "wb");
    int count = 0;

    while( 1 )
    {
        int ret = fread(src_frame->data[0], 1, src_bytes_num, fin);
        if( ret != src_bytes_num )
        {
            printf("don't read enough data %d\n", ret);
            break;
        }

        sws_scale(sws_ctx, src_frame->data, src_frame->linesize,
                  0, height,
                  dst_frame->data, dst_frame->linesize);


        ret = fwrite(dst_frame->data[0], 1, dst_bytes_num, fout);
        if( ret != dst_bytes_num )
            printf("don't write enough data %d \n", ret);


        //如果要保存为BMP格式，要把目标图像的格式设置为RGB24。
        //只需把前面的AVPixelFormat dst_fmt = AV_PIX_FMT_YUYV422;
        //改成AVPixelFormat dst_fmt = AV_PIX_FMT_RGB24;即可
        saveAsBitmap(dst_frame, width, height, count++);
    }


    av_free(src_frame);
    av_free(dst_frame);
    av_free(src_buff);
    av_free(dst_buff);

    sws_freeContext(sws_ctx);


    fclose(fin);
    fclose(fout);

    return 0;
}
