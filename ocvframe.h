#ifndef OCVFRAME_H
#define OCVFRAME_H

#include <QGraphicsItem>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "colorfilter.h"

class OcvFrame
{
public:
    OcvFrame();

private:
    cv::VideoCapture videoStream;
    cv::Mat currentFrame;
    cv::Mat intermediateFrame;
    cv::Mat displayFrame;
    QImage displayImage;
    std::vector<ColorFilter> filters;
    ColorFilter filter;
    QImage Mat2QImage(const cv::Mat3b &src);
};

#endif // OCVFRAME_H
