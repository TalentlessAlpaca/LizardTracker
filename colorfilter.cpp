#include "colorfilter.h"

ColorFilter::ColorFilter()
{
    erode_size = std::vector<int>(3,3);
    dilate_size = std::vector<int>(3,3);
    minVals = cv::Scalar(0,0,0);
    maxVals = cv::Scalar(255,255,255);
}

ColorFilter::ColorFilter(QString fName, int minH, int maxH, int minS, int maxS, int minV, int maxV, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec){
    int _eDec = (eDec == 1) ? 1:0;
    int _dDec = (dDec == 1) ? 1:0;
    int erodes[eReps];
    int dilates[dReps];
    for(int i=0; i<eReps;i++) erodes[i]  = std::abs(eSize-(i*_eDec));
    for(int i=0; i<dReps;i++) dilates[i] = std::abs(dSize-(i*_dDec));
    erode_size  = std::vector<int>(erodes,erodes+(sizeof(erodes)/sizeof(int)));
    dilate_size = std::vector<int>(dilates,dilates+(sizeof(dilates)/sizeof(int)));

    minVals = cv::Scalar(minH,minS,minV);
    maxVals = cv::Scalar(maxH,maxS,maxV);
    filterName = fName;
}

ColorFilter::ColorFilter(QString fName,cv::Scalar _minVals, cv::Scalar _maxVals, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec){
    int _eDec = (eDec == 1) ? 1:0;
    int _dDec = (dDec == 1) ? 1:0;
    int erodes[eReps];
    int dilates[dReps];
    for(int i=0; i<eReps;i++) erodes[i]  = std::abs(eSize-(i*_eDec));
    for(int i=0; i<dReps;i++) dilates[i] = std::abs(dSize-(i*_dDec));
    erode_size  = std::vector<int>(erodes,erodes+(sizeof(erodes)/sizeof(int)));
    dilate_size = std::vector<int>(dilates,dilates+(sizeof(dilates)/sizeof(int)));

    minVals = cv::Scalar(_minVals[0],_minVals[1],_minVals[1]);
    maxVals = cv::Scalar(_maxVals[0],_maxVals[1],_maxVals[1]);

    filterName = fName;
}

std::vector<int> ColorFilter::get_erode(){
    return erode_size;
}

std::vector<int> ColorFilter::get_dilate(){
    return dilate_size;
}

cv::Scalar ColorFilter::get_minVals(){
    return minVals;
}

cv::Scalar ColorFilter::get_maxVals(){
    return maxVals;
}

QString ColorFilter::get_name(){
    return filterName;
}
