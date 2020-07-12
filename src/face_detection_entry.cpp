/*
 *
 * This file is part of the open-source SeetaFace engine, which includes three modules:
 * SeetaFace Detection, SeetaFace Alignment, and SeetaFace Identification.
 *
 * This file is an example of how to use SeetaFace engine for face detection, the
 * face detection method described in the following paper:
 *
 *
 *   Funnel-structured cascade for multi-view face detection with alignment awareness,
 *   Shuzhe Wu, Meina Kan, Zhenliang He, Shiguang Shan, Xilin Chen.
 *   In Neurocomputing (under review)
 *
 *
 * Copyright (C) 2016, Visual Information Processing and Learning (VIPL) group,
 * Institute of Computing Technology, Chinese Academy of Sciences, Beijing, China.
 *
 * The codes are mainly developed by Shuzhe Wu (a Ph.D supervised by Prof. Shiguang Shan)
 *
 * As an open-source face recognition engine: you can redistribute SeetaFace source codes
 * and/or modify it under the terms of the BSD 2-Clause License.
 *
 * You should have received a copy of the BSD 2-Clause License along with the software.
 * If not, see < https://opensource.org/licenses/BSD-2-Clause>.
 *
 * Contact Info: you can send an email to SeetaFace@vipl.ict.ac.cn for any problems.
 *
 * Note: the above information must be kept whenever or wherever the codes are used.
 *
 */


#include <QImage>



#include "face_detection.h"
#include "face_detection_entry.h"

using namespace std;
static const char * face_detetion_bin ="/home/robot/thirdLibs/SeetaFaceEngine/FaceDetection/model/seeta_fd_frontal_v1.0.bin";
static const char * img_path = "/home/robot/Pictures/2.jpg";           //! second parament

//!
//! \brief av_frame_to_bmp
//! \param pFrameRGB
//! \param width
//! \param height
//! \param iFrame
//! \return
//!
bool av_frame_to_bmp(uint8_t *pFrameRGB, int width, int height, int iFrame)
{

      char fileName[32];
      // open file
      sprintf(fileName, "frame%d.bmp", iFrame);
      FILE *fp_bmp=NULL;
      if((fp_bmp=fopen(fileName,"wb"))==NULL){
          printf("Error: Cannot open output BMP file.\n");
          return -1;
      }
      // Part.1 Create Bitmap File Header
      BITMAPFILEHEADER fileHeader = { 0 };

      fileHeader.bfType = 0x4D42;//!'BM'
      fileHeader.bfReserved1 = 0;
      fileHeader.bfReserved2 = 0;
      fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 3*width*height;
      fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

      // Part.2 Create Bitmap Info Header
      BITMAPINFOHEADER bitmapHeader = { 0 };
      bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
      bitmapHeader.biHeight = -height;
      //bitmapHeader.biHeight = height;
      bitmapHeader.biWidth = width;
      bitmapHeader.biPlanes = 1;
      bitmapHeader.biBitCount = 24;
      bitmapHeader.biSizeImage = 3*width*height;
      bitmapHeader.biCompression = 0; //BI_RGB

      fwrite(&fileHeader,1,sizeof(BITMAPFILEHEADER),fp_bmp);
      fwrite(&bitmapHeader,1,sizeof(BITMAPINFOHEADER),fp_bmp);

      //!BMP save B1|G1|R1,B2|G2|R2 format
      //!It saves pixel data in Little Endian

      //uint8_t *buffer = pFrameRGB->data[0];
      uint8_t *buffer = pFrameRGB;
      uint8_t g_buf[]= {15};
      uint8_t b_buf[]= {15};
      int size = width * height;
      for (int i = 0; i< size; i++)
      {
//          fwrite(buffer+size+i, 1, 1, fp_bmp);
//          fwrite(buffer+i, 1, 1, fp_bmp);
//          fwrite(buffer+2*size+i, 1, 1, fp_bmp);
          fwrite(g_buf, 1, 1, fp_bmp);
          fwrite(buffer+i, 1, 1, fp_bmp);
          fwrite(b_buf, 1, 1, fp_bmp);
      }

      fclose(fp_bmp);
      std::cout << fileName << '\n';
      return true;
}



int face_detection_entry(void* pFrame)
{
    int run_count = 0;
    SDL_UserEvent event;

    ///> @note initilize seeta::FaceDetection
     seeta::FaceDetection detector((const char*)face_detetion_bin);   //!  third parament
     detector.SetMinFaceSize(40);
     detector.SetScoreThresh(2.f);
     detector.SetImagePyramidScaleFactor(0.8f);
     detector.SetWindowStep(4, 4);

    for(;;)
    {
    SDL_WaitEvent((SDL_Event*)&event);
    if(event.type == FACE_DETECTION_EVENT)
    {
      cv::Mat img;
      cv::Mat img_gray;
     ///> Frame change to picture
      run_count += 1 ;
      int code = event.code;
      //AVFrame *pFrame = (AVFrame *)event.data1;

      char * filePath = "./frame25.bmp";
      //av_frame_to_bmp(pFrame,pFrame->width, pFrame->height, run_count);
      long t0 = cv::getTickCount();
    ///> @param input: picture store path
      img = cv::imread(filePath, cv::IMREAD_UNCHANGED);
      if (img.channels() != 1)
        cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
      else
        img_gray = img;

    ///> @param input: seeta::ImageData
      seeta::ImageData img_data;
      img_data.data = img_gray.data;
      img_data.width = img_gray.cols;
      img_data.height = img_gray.rows;
      img_data.num_channels = 1;

    ///> @note caculate use time, detect face feature

      std::vector<seeta::FaceInfo> faces = detector.Detect(img_data);
      long t1 = cv::getTickCount();
      double secs = (t1 - t0)/cv::getTickFrequency();
      cout << "Detections takes " << secs << " seconds " << endl;
      //cout << "picture linesize: " << pFrame->height << endl;
    #ifdef USE_OPENMP
      cout << "OpenMP is used." << endl;
    #else
      cout << "OpenMP is not used. " << endl;
    #endif

    #ifdef USE_SSE
      cout << "SSE is used." << endl;
    #else
      cout << "SSE is not used." << endl;
    #endif

      cout << "Image size (wxh): " << img_data.width << "x"
          << img_data.height << endl;
    ///> @note : face feature, output Rect bbox
      cv::Rect face_rect;
      int32_t num_face = static_cast<int32_t>(faces.size());

      for (int32_t i = 0; i < num_face; i++) {
        face_rect.x = faces[i].bbox.x;
        face_rect.y = faces[i].bbox.y;
        face_rect.width = faces[i].bbox.width;
        face_rect.height = faces[i].bbox.height;
        cv::rectangle(img, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
      }
      /// 直接转换 模式
      QImage image(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
      image.save("./frame555.jpg");
      cout << "run times: " << run_count << " face width: "<< face_rect.width << " height: "<< face_rect.height << '\n';
    ///> @note :display image
//      cv::namedWindow("Test", cv::WINDOW_AUTOSIZE);
//      cv::imshow("Test", img);
//      SDL_Delay(200);
//      cv::destroyAllWindows();
  }else if(event.type==SDL_QUIT){
      cout << "event read not mach type: " << event.type << '\n';
      SDL_Delay(200);
      break;
  }else{//!idle task
    SDL_Delay(50);
    }
  }
  SDL_Quit();

  cv::destroyAllWindows();
  return 0;
}
