#ifndef MAT_TO_QIMAGE_H
#define MAT_TO_QIMAGE_H

#include <QImage>
#include "opencv/cv.hpp"

QImage MatToQImage(cv::Mat mtx);

#endif // MAT_TO_QIMAGE_H
