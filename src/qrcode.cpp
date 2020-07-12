
#include <QPainter>
#include <QDebug>

#include "opencv2/imgproc.hpp"

#include "qrcode.h"



ex_QRcode::ex_QRcode()
{

}


/**
 * @brief QRcode::createQRcode
 * @param s         :create code of content
 * @param f_name    :save to path,out file is png format
 * @param width     :QR code width
 * @param height    :QR code height
 * @return          :0 success,< 0 failure
 */
int ex_QRcode::createQRcode(QString s, const char *f_name,int width, int height )
{
    if(s.isEmpty()){
        return -1;
    }
    if(f_name == NULL){
        return -2;
    }
    QRcode *qrcode; //二维码数据
    char *msg;
    QByteArray a = s.toLatin1();
    msg = a.data();
    qrcode = QRcode_encodeString(msg, 0, QR_ECLEVEL_L, QR_MODE_8, 1); ///QR_ECLEVEL_Q 容错等级

    int32_t qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    double scale_x = (double)width / (double)qrcode_width; //二维码图片的缩放比例
    double scale_y = (double)height / (double)qrcode_width;
    QImage img = QImage(width, height, QImage::Format_ARGB32);

    QPainter painter(&img);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width, height);
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for( int32_t y = 0; y < qrcode_width; y++) {
        for(int32_t x = 0; x < qrcode_width; x++) {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if(b & 0x01) {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    img.save(f_name);

    //QPixmap pix_img = QPixmap::fromImage(img,Qt::AutoColor);
    //pixImg.save(f_name);
    return 0;
}

/**
 * @brief QRcode::scannerQRcode
 * @param input img : const char *img,image file path
 * @param output    : std::string & decode,& content
 * @return 0 success, < 0  failure
 */
std::string tmp = "/home/robot/qtWorkspace/FaceEngine/img/qrcode.png";
std::string tmp1 = "/home/robot/qtWorkspace/FaceEngine/img/qrcode1.png";
std::string tmp2 = "/home/robot/qtWorkspace/FaceEngine/img/qrcode2.png";
int ex_QRcode::scannerQRcode(const char *img, std::string & decode,std::string &content)
{
    cv::Mat  imgGray;
    //cv::Mat image = cvLoadImage(img,CV_IMWRITE_PAM_FORMAT_NULL);
    cv::Mat image = cv::imread(img);
    //imgGray.create(image.size(), CV_8UC1);
    cv::cvtColor(image,imgGray,CV_BGR2GRAY);
    //cv::imwrite(tmp,imgGray);

    cv::resize(imgGray,image,cv::Size(imgGray.cols*0.5,imgGray.rows*0.5));
    //cv::imwrite(tmp1,image);

    cv::threshold(image,imgGray,178,255,2);
    //cv::imwrite(tmp2,imgGray);
    //cv::imshow("window",imgGray);

    unsigned char *pdata = (unsigned char *)imgGray.data;
    int width   = imgGray.cols;
    int height  = imgGray.rows;

    // create a scanner
    zbar::ImageScanner scanner;
    // configure the reader
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
    int n = scanner.scan(imageZbar);
    if(n>0)
    {
        for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
            symbol != imageZbar.symbol_end();++symbol) {
            decode = symbol->get_type_name();
            qDebug() << "decoded " << decode.c_str() << '\n';
            content=symbol->get_data();
            qDebug() << "symbol " << content.c_str() << '\n';

        }
        return 0;
    }else{
        qDebug() << "QR code don't known...";
        return -1;
    }

}
