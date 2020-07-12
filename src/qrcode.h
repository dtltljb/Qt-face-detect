#ifndef QRCODE_H
#define QRCODE_H

#include <iostream>
#include <iostream>
#include <vector>

///> third libs
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

/**
 * @brief qr code header
  */
#include "zbar.h"
#include "qrencode.h"

class ex_QRcode
{
public:
    ex_QRcode();

    /**
     * @brief           :createQRcode
     * @param s         :create code of content
     * @param f_name    :save to path
     * @param width     :QR code width
     * @param height    :QR code height
     * @return          :0 success,< 0 failure
     */
    int createQRcode(QString s, const char *f_name, int width, int height );

    /**
     * @brief QRcode::scannerQRcode
     * @param input img : const char *img,image file path
     * @param output    : std::string & decode,& content
     * @return 0 success, < 0  failure
     */
    int scannerQRcode(const char *img, std::string & decode, std::string &content);

};

#endif // QRCODE_H
