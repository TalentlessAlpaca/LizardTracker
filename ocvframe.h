#ifndef OCVFRAME_H
#define OCVFRAME_H

#include <QGraphicsItem>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "colorfilter.h"

class OcvFrame : public QGraphicsItem
{
public:
    OcvFrame(std::vector<ColorFilter> * inFilters);
    OcvFrame(cv::VideoCapture vc);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;
    void closeStream();
    void activateFilter(bool state);
    void setActiveFilter(int filter);

protected:
    void advance(int step) Q_DECL_OVERRIDE;

private:
    cv::VideoCapture videoStream;
    cv::Mat currentFrame;
    cv::Mat intermediateFrame_A;
    cv::Mat intermediateFrame_B;
    cv::Mat displayFrame;
    QImage displayImage;
    std::vector<ColorFilter> *filters;
    int activeFilter = -1;
    bool filteringActive = false;

    int containerHeight = 721;
    int containerWidth = 561;

    bool still = false;
    bool play = true;
    bool replay = true;
    bool nextFrame = false;

    QImage Mat2QImage(const cv::Mat3b &src);

    std::vector<cv::Vec3f> circles;

};

#endif // OCVFRAME_H
