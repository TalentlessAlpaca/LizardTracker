#ifndef COLORFILTER_H
#define COLORFILTER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>

class ColorFilter
{
public:
    ColorFilter();
    ColorFilter(QString fName,int minH, int maxH, int minS, int maxS, int minV, int maxV, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo);
    ColorFilter(QString fName,cv::Scalar _minVals, cv::Scalar _maxVals, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo);
    std::vector<int> get_erode();
    std::vector<int> get_dilate();
    cv::Scalar get_minVals();
    cv::Scalar get_maxVals();
    void set_minVals(int mh, int ms, int mv);
    void set_maxVals(int mh, int ms, int mv);
    QString get_name();
    void recalc_erode(int size, int reps, bool dec);
    void recalc_dilate(int size, int reps, bool dec);

    int get_erode_geometry();
    int get_dilate_geometry();

    void set_erode_geometry(int id);
    void set_dilate_geometry(int id);

private:
    std::vector<int> erode_size;
    std::vector<int> dilate_size;
    int erode_geometry = cv::MORPH_ELLIPSE;
    int dilate_geometry = cv::MORPH_ELLIPSE;

    cv::Scalar minVals;
    cv::Scalar maxVals;

    QString filterName;
};

#endif // COLORFILTER_H
