#ifndef OCVFRAME_H
#define OCVFRAME_H

#include <QGraphicsItem>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "colorfilter.h"

class OcvFrame : public QGraphicsItem
{
public:
    OcvFrame();
    OcvFrame(cv::VideoCapture vc);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;
protected:
    void advance(int step) Q_DECL_OVERRIDE;

private:
    cv::VideoCapture videoStream;
    cv::Mat currentFrame;
    cv::Mat intermediateFrame_A;
    cv::Mat intermediateFrame_B;
    cv::Mat displayFrame;
    QImage displayImage;
    std::vector<ColorFilter> filters;
    bool filteringActive = false;
    QImage Mat2QImage(const cv::Mat3b &src);

};

#endif // OCVFRAME_H
