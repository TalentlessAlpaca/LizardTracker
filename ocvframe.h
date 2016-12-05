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
    void close_stream();
    void activate_filter(bool state);
    void set_active_filter(int filter);
    void play_video(bool val);
    void advance_frame(bool val);

protected:
    void advance(int step) Q_DECL_OVERRIDE;

private:
    cv::VideoCapture video_stream;
    cv::Mat current_frame;
    cv::Mat intermediate_frame_A;
    cv::Mat intermediate_frame_B;
    cv::Mat display_frame;
    QImage display_image;
    std::vector<ColorFilter> *filters;
    int active_filter = -1;
    bool filtering_active = false;

    int container_height = 721;
    int container_width = 500;

    bool still = false;
    bool play = true;
    bool replay = true;
    bool next_frame = false;

    QImage Mat_to_QImage(const cv::Mat3b &src);

    std::vector<cv::Vec3f> circles;
    int fr = 0;

};

#endif // OCVFRAME_H
