/*
 *
 * This file is part of the open-source SeetaFace engine, which includes three modules:
 * SeetaFace Detection, SeetaFace Alignment, and SeetaFace Identification.
 *
 * This file is an example of how to use SeetaFace engine for face alignment, the
 * face alignment method described in the following paper:
 *
 *
 *   Coarse-to-Fine Auto-Encoder Networks (CFAN) for Real-Time Face Alignment, 
 *   Jie Zhang, Shiguang Shan, Meina Kan, Xilin Chen. In Proceeding of the
 *   European Conference on Computer Vision (ECCV), 2014
 *
 *
 * Copyright (C) 2016, Visual Information Processing and Learning (VIPL) group,
 * Institute of Computing Technology, Chinese Academy of Sciences, Beijing, China.
 *
 * The codes are mainly developed by Jie Zhang (a Ph.D supervised by Prof. Shiguang Shan)
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

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
///> qt libs
#include <QImage>
#include <QDebug>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>

#include "opencv/cv.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
///> seeta libs
#include "recognizer.h"
#include "math_functions.h"
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"

///> user header
#include "face_alignment_entry.h"
#include "mat_to_qimage.h"
#include "forlder_util.h"

#ifdef _WIN32
std::string DATA_LIB = "/home/robot/tmp/";
std::string MODEL_LIB = "../camera_qml/model/";
#else
std::string DATA_LIB = "/home/robot/tmp/";
std::string MODEL_LIB = "../camera_qml/model/";
#endif

//using namespace seeta; 此命名空间会导致类函数出现模棱两可的定义错误

int FaceAlignment::face_alignment(QString argv, QString number)
{
    if(argv == NULL ){
        qDebug() << "argv: " << argv << "argv is NULL..,EXIT";
        return 0;
    }
  QString fileName = QCoreApplication::applicationFilePath();

  //char * imgPath = (char *)argv;
  QString imgPath = argv;

  // Initialize face detection model
  seeta::FaceDetection detector( (const char*)(MODEL_LIB+"seeta_fd_frontal_v1.0.bin").c_str());
  detector.SetMinFaceSize(40);
  detector.SetScoreThresh(2.f);
  detector.SetImagePyramidScaleFactor(0.8f);
  detector.SetWindowStep(4, 4);

  // Initialize face alignment model 
  seeta::FaceAlignment point_detector((MODEL_LIB + "seeta_fa_v1.1.bin").c_str());

  //load image
  IplImage *img_grayscale = NULL;
  std::string path = imgPath.toStdString();

  img_grayscale = cvLoadImage((const char*)path.data(), CV_LOAD_IMAGE_GRAYSCALE);
  if (img_grayscale == NULL)
  {
    qDebug() << "img_grayscale is NULL..,EXIT";
    return -1;
  }

  IplImage *img_color = cvLoadImage((const char*)path.data(), CV_LOAD_IMAGE_COLOR);

  ///> gray image
  int im_width = img_grayscale->width;
  int im_height = img_grayscale->height;
  unsigned char* data = new unsigned char[im_width * im_height];
  unsigned char* data_ptr = data;
  unsigned char* image_data_ptr = (unsigned char*)img_grayscale->imageData;
  int h = 0;
  for (; h < im_height; h++) {
    memcpy(data_ptr, image_data_ptr, im_width);
    data_ptr += im_width;
    image_data_ptr += img_grayscale->widthStep;
  }


  seeta::ImageData image_data;
  image_data.data = data;
  image_data.width = im_width;
  image_data.height = im_height;
  image_data.num_channels = 1;

  // Detect faces
  std::vector<seeta::FaceInfo> faces = detector.Detect(image_data);
  int32_t face_num = static_cast<int32_t>(faces.size());

  if (face_num != 1) //!图片中多张脸也是错误
  {
    delete[]data;
    cvReleaseImage(&img_grayscale);
    cvReleaseImage(&img_color);
    return -2;
  }

  /// Detect 5 facial landmarks
  seeta::FacialLandmark points[5];
  point_detector.PointDetectLandmarks(image_data, faces[0], points);

  int x = faces[0].bbox.x;
  if(x - 15 < 0){
      x=0;
  }else{
      x -= 15;
  }
  int y = faces[0].bbox.y;
  if( y-55 < 0){
      y=0;
  }else{
      y -= 55;
  }

  int width = faces[0].bbox.width;
  width = width*125/100;
  if(width > im_width){
      width = im_width;
  }
  int height = faces[0].bbox.height;
  height = height*155/100;
  if(height > im_height){
      height = im_height;
  }

  //! \brief CvRect image
  cv::Mat src = cv::imread((const char*)path.data());
  CvRect rect = cvRect(x, y, width, height);
  //CvRect rect = cvRect(0,0,src.cols,src.rows);
  cv::Mat img = src(rect);

  int pos = path.find_last_of("/");
  std::string f_name = path.substr(pos+1,path.size()-pos-1);
  f_name = DATA_LIB + f_name;
  cv::imwrite(f_name,img);
  emit this->faceAlignmentResult(path.c_str());   /// sender signal alignment result
/// 归一化图片尺寸,cvSaveImage 失败问题不祥
//  IplImage * image = cvLoadImage(f_name.c_str());
//  IplImage * out = cvCreateImage(CvSize(220,160),
//                                image->depth, image->nChannels);
//  cvResize(image,out);
//  cvShowImage("after",out);
//  cvSaveImage((const char*)f_name.data(),out);
//  cvReleaseImage(&image);
//  cvReleaseImage(&out);

  //! Visualize the results, draw rectangle and circle
//  cvRectangle(img_grayscale, cvPoint(faces[0].bbox.x, faces[0].bbox.y), cvPoint(faces[0].bbox.x + faces[0].bbox.width - 1,
//                faces[0].bbox.y + faces[0].bbox.height - 1), CV_RGB(255, 0, 0));
//  int pts_num = 5;
//  for (int i = 0; i<pts_num; i++)
//  {
//    cvCircle(img_color, cvPoint(points[i].x, points[i].y), 2, CV_RGB(0, 255, 0), CV_FILLED);
//  }
//  QImage image((uchar*)img_color->imageData, img_color->width, img_color->height,
//               img_color->widthStep, QImage::Format_RGB888);
//  std::string name = number.toStdString();
//  name = name + ".jpg";
//  path = DATA_LIB + name;
//  image.save( path.c_str() );


  // Release memory
  cvReleaseImage(&img_color);
  cvReleaseImage(&img_grayscale);
  delete[]data;
  return 0;
}




int FaceAlignment::faceVerification( QString f_name)
{
  float similarity_good = 0.75;
  std::string RESULT_LIB = "../../data/";
  // Initialize face detection model
  seeta::FaceDetection detector( (MODEL_LIB + "seeta_fd_frontal_v1.0.bin").c_str() );
  detector.SetMinFaceSize(40);
  detector.SetScoreThresh(2.f);
  detector.SetImagePyramidScaleFactor(0.8f);
  detector.SetWindowStep(4, 4);

  // Initialize face alignment model
  seeta::FaceAlignment point_detector( (MODEL_LIB + "seeta_fa_v1.1.bin").c_str() );

  // Initialize face Identification model
  seeta::FaceIdentification face_recognizer((MODEL_LIB + "seeta_fr_v1.0.bin").c_str());

  std::string result_path = DATA_LIB;
  int file_count;
  int i = 0;

  std::vector<std::string> list;
  list_files_recursively(DATA_LIB.c_str(),list);
  file_count = list.size();
  for(;i<file_count;i++){
      //! load image libs
      cv::Mat gallery_img_color = cv::imread(list.at(i), 1);
      cv::Mat gallery_img_gray;
      cv::cvtColor(gallery_img_color, gallery_img_gray, CV_BGR2GRAY);
      //! load probe image file
      std::string proble = f_name.toStdString();
      cv::Mat probe_img_color = cv::imread(proble, 1);
      cv::Mat probe_img_gray;
      cv::cvtColor(probe_img_color, probe_img_gray, CV_BGR2GRAY);

      //! load libs and probe image file
      seeta::ImageData gallery_img_data_color(gallery_img_color.cols, gallery_img_color.rows, gallery_img_color.channels());
      gallery_img_data_color.data = gallery_img_color.data;

      seeta::ImageData gallery_img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
      gallery_img_data_gray.data = gallery_img_gray.data;

      seeta::ImageData probe_img_data_color(probe_img_color.cols, probe_img_color.rows, probe_img_color.channels());
      probe_img_data_color.data = probe_img_color.data;

      seeta::ImageData probe_img_data_gray(probe_img_gray.cols, probe_img_gray.rows, probe_img_gray.channels());
      probe_img_data_gray.data = probe_img_gray.data;

      ///> @part Detect faces
      std::vector<seeta::FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
      int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());

      std::vector<seeta::FaceInfo> probe_faces = detector.Detect(probe_img_data_gray);
      int32_t probe_face_num = static_cast<int32_t>(probe_faces.size());

      if (gallery_face_num == 0 || probe_face_num==0)
      {
        std::cout << "Faces are not detected.";
        continue;
        //return 0;
      }

      // Detect 5 facial landmarks
      seeta::FacialLandmark gallery_points[5];
      point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[0], gallery_points);

      seeta::FacialLandmark probe_points[5];
      point_detector.PointDetectLandmarks(probe_img_data_gray, probe_faces[0], probe_points);

      ///> @part Extract face identity feature
      float gallery_fea[2048];
      float probe_fea[2048];
      face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, gallery_fea);
      face_recognizer.ExtractFeatureWithCrop(probe_img_data_color, probe_points, probe_fea);

      ///> @part Caculate similarity of two faces
      float sim = face_recognizer.CalcSimilarity(gallery_fea, probe_fea);
      if(sim < similarity_good){
          continue;
      }else{
          ///>@part record result picture
          std::string r_name = "IMG_00000235.jpg";
          cv::imwrite(result_path + r_name, probe_img_color);
          std::cout << "similarity: " << sim << endl;
          return 0;
      }
  }

  return -1;
}




