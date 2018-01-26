//
//  main1.cpp
//  LearningOpenCV
//
//  Created by liuxiang on 2018/1/26.
//  Copyright © 2018年 Yourtion. All rights reserved.
//



//#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define CV_VERSION_ID       CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>//创建界面的头文件

#pragma comment(lib, cvLIB("core"))
#pragma comment(lib, cvLIB("imgproc"))
#pragma comment(lib, cvLIB("highgui"))
using namespace cv;
using namespace std;
#define N 10       //载入数字图片个数
string base_path = "/Users/liuxiang/Documents/opencv-workspace/opencvReadNu/";
string testPic[] =
{
    "test1.jpg", "test2.jpg",  "test3.jpg",  "test4.jpg",
    "test5.jpg", "test6.jpg",  "test7.jpg",  "test8.jpg",
    "test9.jpg", "test10.jpg", "test11.png",
};
int thres = 115;      //二值化阀值
//int flag0 = 1;      //准确标志  1为不符合，0为符合
int n_min = 80;       //识别数字轮廓长度的下限 单位（像素）
int n_max = 1400;      //识别数字轮廓长度的上限
// 数字轮廓的长度和宽度  单位（像素）
int n_width_min  = 5,   n_width_max = 100;
int n_height_min = 30,  n_height_max = 150;
// 数组成员之间的距离小于一个阀值视为一个数
int n_width_n_min = 15, n_width_n_max = 40;

// 数字图片集  这里储存白底黑子的数字图片
string picture[N] =
{
    "0.jpg", "1.jpg", "2.jpg", "3.jpg", "4.jpg",
    "5.jpg", "6.png", "7.png", "8.png", "9.png",
};
CvSeq *pic[N];  //储存数字图片轮廓
CvSeq* GetImageContour(IplImage* srcIn, int flag = 0)
{
    IplImage* src;
    CvSeq* seq = nullptr;       //储存图片轮廓信息
    int total = 0;    //轮廓总数
    //int count = 0;
    src = cvCreateImage(cvGetSize(srcIn), 8, 1);
    
    //拷贝图像
    cvCopy(srcIn,src);
    //创建空间
    CvMemStorage* mem = cvCreateMemStorage(0);
    if(!mem)
    {
        printf("mem is NULL!");
    }
    //二值化图像
    cvThreshold(src,src,thres,255,CV_THRESH_BINARY_INV);
    //计算图像轮廓  0-只获取最外部轮廓  1-获取全部轮廓
    if(flag == 0)
    {
        total = cvFindContours(src,mem,&seq,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
    }
    if(flag == 1)
    {
        total = cvFindContours(src,mem,&seq,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
    }
    
    //  printf("total = %d\n",total);
    //释放图像空间
    cvReleaseImage(&src);
    //返回轮廓信息
    return seq;
}
//数字图片轮廓计算
void Init(void)
{
    IplImage *src0, *src;
    int i;
    for(i = 0; i < N; i++)
    {
        src0 = cvLoadImage((base_path + picture[i]).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if(!src0)
        {
            printf("Couldn't load %s\n", picture[i].c_str());
            exit(1);
        }
        src = cvCloneImage(src0);
        pic[i] = GetImageContour(src, 0);   //只获取最外部轮廓
    }
}

int ReadNumber(CvSeq* contoursTemp)
{
    int i;
    double tmp = 5, min = 5;
    int num = -1;
    for(i = 0; i < N; i++)
    {
        tmp = cvMatchShapes(contoursTemp, pic[i], 1);   //匹配
        if(tmp < min)
        {
            min = tmp;
            num = i;
        }
    }
    return num;
}
void Paixu(int numList[100][2],int count)    //将数字按横坐标从左往右排列
{
    int i,j;    //循环
    int n = 0;  //融合后的数字个数
    int tem;
//    int head = 0, tail = 1;
    int width = 0;           //两数字间的距离
    int newList[100] = {0};  //数字融合后的新序列
    for(i=0;i<count-1;i++)
    {
        for(j=i+1;j<count;j++)
        {
            if(numList[i][1] > numList[j][1])
            {
                //交换坐标
                tem = numList[i][1];numList[i][1] = numList[j][1];numList[j][1] = tem;
                //交换数字
                tem = numList[i][0];numList[i][0] = numList[j][0];numList[j][0] = tem;
            }
        }
    }
    if(count == 0)
    {
        printf("no number!");
    }
    else
    {
        for(i=0;i<count;i++)
        {
            printf("1--- %d\t", numList[i][0]);
        }
    }
    //数字融合，可以自己改。。。。数字从左往右的顺序都在numList里面，[0]为数,[1]为坐标，自己可以根据数字间的距离判断是否为一个数
    if(count == 2)
    {
        width = numList[1][1] - numList[0][1];
        if((width < n_width_n_max) && (width > n_width_n_min))
        {
            tem = numList[0][0] * 10 + numList[1][0];
            newList[0] = tem;
        }
        printf("the number is %d\t",newList[0]);
    }
}

int main()
{
    int travel = 3; //如果为0，识别中间数字,如果为1，识别右边数字.其他数字，全部识别
    int i;          //循环标志
    int count=0;    //数字轮廓个数
    int num = -1;   //识别一幅图像中的一个数字
    int numList[100][2];  //一幅图像中的数字序列  一维是数字，二维是数字所在横坐标
    CvPoint pt1, pt2;
    CvRect ins;
    
    Init();   //初始化,在pic中储存所有图片轮廓
    
    //CvMemStorage* storage = cvCreateMemStorage(0);
    
    IplImage* img = cvLoadImage((base_path + testPic[10]).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    if(travel == 0)
    {
        ins.x = 170;
        ins.y = 140;
        ins.width = 300;
        ins.height = 200;
        cvSetImageROI(img,ins);
    }
    if(travel == 1)
    {
        ins.x = 470;
        ins.y = 140;
        ins.width = 165;
        ins.height = 200;
        cvSetImageROI(img,ins);
    }
    
    IplImage* imgColor = cvCreateImage(cvGetSize(img), 8, 3);
    IplImage* contoursImage = cvCreateImage(cvSize(img->width, img->height), 8, 1);
    
    CvSeq* contours = 0, *contoursTemp=0;
    cvZero(contoursImage);
    /*
     //对图像进行二值化
     cvThreshold(img,img,100,255,CV_THRESH_BINARY);
     //img的备份
     cvCvtColor(img,imgColor,CV_GRAY2BGR);
     */
    
    /*
     // 提取图像img的轮廓信息 contours指向第一个轮廓
     int total = cvFindContours( img, storage, &contours, sizeof(CvContour),
     CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );
     */
    contours = GetImageContour(img, 1);   //获取轮廓信息
    contoursTemp = contours ;
    
    //对轮廓进行循环
    for(; contoursTemp!=0; contoursTemp = contoursTemp->h_next)
    {
        //如果符合数字轮廓长度条件 保留并画出
        if(contoursTemp->total > n_min && contoursTemp->total < n_max)
        {
            num = -1;
            
            CvRect rect = cvBoundingRect(contoursTemp, 1);  //根据序列，返回轮廓外围矩形
            //如果满座数字轮廓长宽
            if((rect.width >= n_width_min && rect.width <= n_width_max) &&
               (rect.height >= n_height_min && rect.height <= n_height_max) )
            {
                //匹配该轮廓数字
                num = ReadNumber(contoursTemp);
                //计算矩形顶点
                pt1.x = rect.x;
                pt1.y = rect.y;
                pt2.x = rect.x + rect.width;
                pt2.y = rect.y + rect.height;
                if(num >= 0)
                {
                    numList[count][0] = num;         //一维存数字
                    numList[count][1] = rect.x;      //二维存数字横坐标
                }
                //在原图上绘制轮廓外矩形
                cvRectangle(imgColor, pt1, pt2, CV_RGB(0,255,0),2);
                //提取外轮廓 上的所以坐标点
                
                for(i = 0; i < contoursTemp->total; i++)
                {
                    CvPoint *pt = (CvPoint*)cvGetSeqElem(contoursTemp, i); // 读出第i个点。
                    cvSetReal2D(contoursImage , pt->y , pt->x , 255.0);
                    cvSet2D(imgColor, pt->y, pt->x, cvScalar(0,0,255,0));
                }
                count++;    //数字轮廓+1
            }
        }
    }
    Paixu(numList,count);   //将数字按横坐标从左往右的顺序排列
    //  printf("How number:%d\n",count);   //输出数字字符个数
    for(i = 0; i < count; i++)
    {
        printf("%d(%d)\t",numList[i][0], numList[i][1]);
    }
    cvNamedWindow( "image", 1 );
    cvShowImage( "image", imgColor );
    cvNamedWindow( "contours");
    cvShowImage("contours",contoursImage);
    cvWaitKey(0);
    cvResetImageROI(imgColor);
    cvResetImageROI(img);
    cvReleaseImage( &img );
    cvReleaseImage(&contoursImage);
    cvReleaseImage(&imgColor);
    return 0;
}
