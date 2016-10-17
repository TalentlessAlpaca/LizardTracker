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

    bool getHoughActive();
    void setHoughActive(bool state);

    bool getEdgeActive();
    void setEdgeActive(bool state);

    int getCannyThreshold();
    void setCannyThreshold(int val);

    int getCentersThreshold();
    void setCentersThreshold(int val);

    int getMinCenterDistance();
    void setMinCenterDistance(int val);

    int getMinRadius();
    void setMinRadius(int val);

    int getMaxRadius();
    void setmMxRadius(int val);

    QColor getfilterColor();
    void setfilterColor(int h, int s, int v);

private:
    std::vector<int> erode_size;
    std::vector<int> dilate_size;
    int erode_geometry = cv::MORPH_ELLIPSE;
    int dilate_geometry = cv::MORPH_ELLIPSE;

    cv::Scalar minVals;
    cv::Scalar maxVals;

    QString filterName;

    bool houghActive = false;
    bool edgeActive = false;
    int cannyThreshold = 100;
    int centersThreshold = 20;
    int minCenterDistance = 5;
    int minRadius = 2;
    int maxRadius = 20;

    QColor filterColor;

};

#endif // COLORFILTER_H
