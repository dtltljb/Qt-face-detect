
#include "mat_to_qimage.h"


QImage MatToQImage(cv::Mat mtx)
{
    switch (mtx.type())
    {
    case CV_8UC1:
        {
            QImage image(mtx.cols, mtx.rows, QImage::Format_Indexed8);
            // Set the color table (used to translate colour indexes to qRgb values)
            image.setColorCount(256);
            for(int i = 0; i < 256; i++)
            {
                image.setColor(i, qRgb(i, i, i));
            }
            // Copy input Mat
            uchar *pSrc = mtx.data;
            for(int row = 0; row < mtx.rows; row ++)
            {
                uchar *pDest = image.scanLine(row);
                memcpy(pDest, pSrc, mtx.cols);
                pSrc += mtx.step;
            }
            return image;
        }
        break;
    case CV_8UC3:
        {
            QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.step, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        break;
    case CV_8UC4:
        {
            QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.step, QImage::Format_ARGB32);
            return img;
        }
        break;
    default:
        {
            QImage img;
            return img;
        }
        break;
    }
}
