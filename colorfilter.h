#ifndef COLORFILTER_H
#define COLORFILTER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ColorFilter
{
public:
    ColorFilter();
    ColorFilter(int minH, int maxH, int minS, int maxS, int minV, int maxV, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec);
    ColorFilter(cv::Scalar _minVals, cv::Scalar _maxVals, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec);
    std::vector<int> get_erode();
    std::vector<int> get_dilate();
    cv::Scalar get_minVals();
    cv::Scalar get_maxVals();
private:
    std::vector<int> erode_size;
    std::vector<int> dilate_size;

    cv::Scalar minVals;
    cv::Scalar maxVals;
};

#endif // COLORFILTER_H
