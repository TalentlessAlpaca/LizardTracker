#ifndef COLORFILTER_H
#define COLORFILTER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>
#include <QColor>
#include <QDebug>

class ColorFilter
{
public:
    ColorFilter();
    ColorFilter(QString fName,int minH, int maxH, int minS, int maxS, int minV, int maxV, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo);
    ColorFilter(QString fName,cv::Scalar _min_vals, cv::Scalar _max_vals, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo);
    std::vector<int> get_erode();
    std::vector<int> get_dilate();
    cv::Scalar get_min_vals();
    cv::Scalar get_max_vals();
    void set_min_vals(int mh, int ms, int mv);
    void set_max_vals(int mh, int ms, int mv);
    QString get_name();
    void recalc_erode(int size, int reps, bool dec);
    void recalc_dilate(int size, int reps, bool dec);

    int get_erode_geometry();
    int get_dilate_geometry();

    void set_erode_geometry(int id);
    void set_dilate_geometry(int id);

    bool get_hough_active();
    void set_hough_active(bool state);

    bool get_edge_active();
    void set_edge_active(bool state);

    int get_canny_threshold();
    void set_canny_threshold(int val);

    int get_centers_threshold();
    void set_centers_threshold(int val);

    int get_min_center_distance();
    void set_min_center_distance(int val);

    int get_min_radius();
    void set_min_radius(int val);

    int get_max_radius();
    void set_max_radius(int val);

    QColor get_filter_color();
    void set_filter_color(int h, int s, int v);

private:
    std::vector<int> erode_size;
    std::vector<int> dilate_size;
    int erode_geometry = cv::MORPH_ELLIPSE;
    int dilate_geometry = cv::MORPH_ELLIPSE;

    cv::Scalar min_vals;
    cv::Scalar max_vals;

    QString filter_name;

    bool hough_active = false;
    bool edge_active = false;
    int canny_threshold = 100;
    int centers_threshold = 20;
    int min_center_distance = 5;
    int min_radius = 2;
    int max_radius = 20;

    QColor filter_color;

};

#endif // COLORFILTER_H
