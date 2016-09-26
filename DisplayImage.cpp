/*
 * DisplayImage.cpp
 *
 *  Created on: Sep 12, 2016
 *      Author: Alpalentless
 */


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

const int HSV_MAX_VAL = 256;
int h_max = 255;
int h_min = 0;
int s_max = 255;
int s_min = 0;
int v_max = 255;
int v_min = 0;

const string oriWin = "Original Image";
const string hsvWin = "HSV filtered version";
const string ctrlWin = "Filter Controls";

Mat bgrImg;
Mat hsvImg;
Mat outImg;

void on_trackbar( int, void* ){
	inRange(hsvImg,Scalar(h_min,s_min,v_min),Scalar(h_max,s_max,v_max),outImg);
	imshow(hsvWin,outImg);
}

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: ./DisplayImage ImageToLoadAndDisplay" << endl;
     return -1;
    }

    bgrImg = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! bgrImg.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    cvtColor( bgrImg, hsvImg, CV_BGR2HSV );
    cvtColor( bgrImg, outImg, CV_BGR2HSV );
    imwrite("hsv_Image.png",hsvImg);

    namedWindow(hsvWin, WINDOW_AUTOSIZE);
    namedWindow(oriWin, WINDOW_AUTOSIZE);
    namedWindow(ctrlWin, WINDOW_AUTOSIZE);


    createTrackbar("HUE Min", ctrlWin, &h_min, HSV_MAX_VAL,on_trackbar);
    createTrackbar("HUE Max", ctrlWin, &h_max, HSV_MAX_VAL,on_trackbar);
    createTrackbar("SAT Min", ctrlWin, &s_min, HSV_MAX_VAL,on_trackbar);
    createTrackbar("SAT Max", ctrlWin, &s_max, HSV_MAX_VAL,on_trackbar);
    createTrackbar("VAL Min", ctrlWin, &v_min, HSV_MAX_VAL,on_trackbar);
    createTrackbar("VAL Max", ctrlWin, &v_max, HSV_MAX_VAL,on_trackbar);

    imshow(oriWin, bgrImg );
    imshow(hsvWin,outImg);

    waitKey(0);
    return 0;
}

