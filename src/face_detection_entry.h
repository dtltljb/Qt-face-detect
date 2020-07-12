#ifndef FACE_DETECTION_TEST_H
#define FACE_DETECTION_TEST_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <QtCore/QEvent>
#include <QtCore/QEventLoop>
#include <QtCore/QEventTransition>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#ifdef __cplusplus
extern "C"
{
#endif
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libavdevice/avdevice.h>
#include <SDL/SDL.h>

#ifdef __cplusplus
};
#endif

/*
 *  #pragma pack(2)必须要加。bfType是2个字节，对应“BM”，后4个字节是文件大小，又对应4字节。
 *  如果不设定对齐方式，而按默认的8字节或4字节对应，这些属性就错位了，而别人又是按标准来读取的，对齐方式不同，自然出错。
*/
#pragma pack(2)
typedef uint8_t BYTE; //BYTE表示8位无符号整数，一个字节
typedef uint16_t WORD; //WORD表示16位无符号整数，两个字节
typedef uint32_t DWORD; //DWORD表示32位无符号整数，四个个字节
typedef int32_t LONG; //LONG表示32位整数，四个字节
/*bitmap图像文件的文件头格式定义，一共14个字节*/
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType; //位图文件类型，必须是Ox424D，即字符创“BM”
    DWORD bfSize; //位图文件大小，包括头文件的四个字节
    WORD bfReserved1; //bfReserved1，bfReserved2，Windows保留字，暂时不用
    WORD bfReserved2;
    DWORD bfOffBits; //从文件头到实际位图数据的偏移字节数
} BITMAPFILEHEADER;

/*位图信息头BITMAPINFOHEADER结构定义*/
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; //本结构长度，为40个字节
    LONG biWidth; //位图的宽度，以像素为单位
    LONG biHeight; //位图的高度，以像素位单位
    WORD biPlanes; //目标设备的级别，必须是1

    /*每个像素所占的位数，
     * 其值必须为 1(黑白图像)、4(16 色图)、8(256色)、24(真彩色图),
     * 新的 BMP 格式支持 32 位色。
     */
    WORD biBitCount;

    /*位图压缩类型,有效的值为 B
     * I_RGB(未经压缩)、BI_RLE8、BI_RLE4、BI_BITFILEDS
     *(均为 Windows 定义常量) 这里只讨论未经压缩的情况, biCompression=BI_RGB。
     * */
    DWORD biCompression;
    DWORD biSizeImage; //实际的位图数据占用的字节数
    LONG biXPelsPerMeter; //指定目标设备的水平分辨率,单位是像素/米。
    LONG biYPelsPerMeter; //指定目标设备的垂直分辨率,单位是像素/米。
    DWORD biClrUsed; //位图实际用到的颜色数,如果该值为零,则用到的颜色数为 2 的 biBitCount 次幂。
    DWORD biClrImportant; //位图显示过程中重要的颜色数,如果该值为零,则认为所有的颜色都是重要的。
} BITMAPINFOHEADER;

/*颜色表的结构体
 * 颜色表实际上是一个 RGBQUAD 结构的数组,数组的长度由 biClrUsed
 指定(如果该值为零,则由 biBitCount 指定,即 2 的 biBitCount 次幂个元素)。
 * */
typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    //BYTE rgbReserved;
} RGBQUAD;
#pragma pack(pop)

//! face detection event
#define FACE_DETECTION_EVENT (SDL_USEREVENT + 3)

int face_detection_entry(void *pFrame);

bool av_frame_to_bmp(uint8_t *pFrameRGB, int width, int height, int iFrame);

#endif // FACE_DETECTION_TEST_H
