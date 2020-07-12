/*
*
* This file is part of the open-source SeetaFace engine, which includes three modules:
* SeetaFace Detection, SeetaFace Alignment, and SeetaFace Identification.
*
* This file is part of the SeetaFace Identification module, containing codes implementing the
* face identification method described in the following paper:
*
*
*   VIPLFaceNet: An Open Source Deep Face Recognition SDK,
*   Xin Liu, Meina Kan, Wanglong Wu, Shiguang Shan, Xilin Chen.
*   In Frontiers of Computer Science.
*
*
* Copyright (C) 2016, Visual Information Processing and Learning (VIPL) group,
* Institute of Computing Technology, Chinese Academy of Sciences, Beijing, China.
*
* The codes are mainly developed by Jie Zhang(a Ph.D supervised by Prof. Shiguang Shan)
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

#include<iostream>
using namespace std;

#ifdef _WIN32
#pragma once
#include <opencv2/core/version.hpp>

#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) \
  CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif //_DEBUG

#pragma comment( lib, cvLIB("core") )
#pragma comment( lib, cvLIB("imgproc") )
#pragma comment( lib, cvLIB("highgui") )

#endif //_WIN32

#if defined(__unix__) || defined(__APPLE__)

#ifndef fopen_s

#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL

#endif //fopen_s

#endif //__unix
//! c++ libs
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

//! openCV libs
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//! face analysis libs
#include "face_identification.h"
#include "recognizer.h"
#include "face_detection.h"
#include "face_alignment.h"
#include "math_functions.h"
#include "face_alignment_entry.h"

using namespace seeta;

#define TEST(major, minor) major##_##minor##_Tester()
#define EXPECT_NE(a, b) if ((a) == (b)) std::cout << "ERROR: "
#define EXPECT_EQ(a, b) if ((a) != (b)) std::cout << "ERROR: "



int face_verification_entry(QString * args)
{
  float similarity_good =0.75;
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

  int i = 0;
  std::vector<std::string> nameList;
  int file_count = nameList.size();


  for(;i<file_count;i++){
      //! load image libs
      cv::Mat gallery_img_color = cv::imread(DATA_LIB + nameList[i], 1);
      cv::Mat gallery_img_gray;
      cv::cvtColor(gallery_img_color, gallery_img_gray, CV_BGR2GRAY);
      //! load probe image file
      std::string proble = args->toStdString();
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
        return 0;
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


