//
//  Main.cpp
//  LearningOpenCV
//
//  Created by liuxiang on 2018/1/25.
//  Copyright © 2018年 Yourtion. All rights reserved.
//

#include <iostream>
#include <thread>
#include "cv.h"
#include "highgui.h"
#include "imgcodecs.hpp"
#include "objdetect.hpp"


#define SHOW_PROC_IMG 1

using namespace cv;
CvMat *p_src_img  = NULL;
CvMat *p_gray_img = NULL;
void Smooth()
{
    cvSmooth(p_src_img, &p_src_img, CV_GAUSSIAN, 5, 0);
}
void Gray()
{
    cvCvtColor(p_src_img, &p_gray_img, CV_BGR2GRAY);
    
//    //灰度图像
//#ifdef SHOW_PROC_IMG
//    cvNamedWindow(PIC_GLAY_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
//    cvShowImage(PIC_GLAY_WINDOW_NAME, gray_img);
//#endif
}

int main(int argc, const char* argv[])
{
    cvNamedWindow("SrcImg", CV_WINDOW_AUTOSIZE);
    Mat img = imread("/Users/liuxiang/Desktop/src_img/1.png");
    CvMat src_img = img;
    p_src_img = &src_img;
    
    IplImage display_img(img);
    cvShowImage("Read", &display_img);

    cvWaitKey(0);
    
    Smooth();
//    Gray();
    cvWaitKey(0);
    
    
    return 0;
}
