#include "colorfilter.h"

ColorFilter::ColorFilter()
{
    erode_size = std::vector<int>(3,3);
    dilate_size = std::vector<int>(3,3);
    minVals = cv::Scalar(0,0,0);
    maxVals = cv::Scalar(255,255,255);
}

ColorFilter::ColorFilter(QString fName, int minH, int maxH, int minS, int maxS, int minV, int maxV, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo){

    erode_size = std::vector<int>(eReps,eSize);
    for(int i=0; eDec && (i<eReps); i++) erode_size[i] = eSize-i;

    dilate_size = std::vector<int>(dReps,dSize);
    for(int i=0; dDec && (i<dReps); i++) dilate_size[i] = dSize-i;

    minVals = cv::Scalar(minH,minS,minV);
    maxVals = cv::Scalar(maxH,maxS,maxV);

    filterName = fName;

    erode_geometry = eGeo;
    dilate_geometry = dGeo;
}

ColorFilter::ColorFilter(QString fName, cv::Scalar _minVals, cv::Scalar _maxVals, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo){
    erode_size = std::vector<int>(eReps,eSize);
    for(int i=0; eDec && (i<eReps); i++) erode_size[i] = eSize-i;

    dilate_size = std::vector<int>(dReps,dSize);
    for(int i=0; dDec && (i<dReps); i++) dilate_size[i] = dSize-i;;

    minVals = cv::Scalar(_minVals[0],_minVals[1],_minVals[1]);
    maxVals = cv::Scalar(_maxVals[0],_maxVals[1],_maxVals[1]);

    filterName = fName;

    erode_geometry = eGeo;
    dilate_geometry = dGeo;
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

void ColorFilter::set_minVals(int mh, int ms, int mv){
    minVals = cv::Scalar(mh,ms,mv);
}

void ColorFilter::set_maxVals(int mh, int ms, int mv){
    maxVals = cv::Scalar(mh,ms,mv);
}


void ColorFilter::recalc_erode(int size, int reps, bool dec){
    erode_size = std::vector<int>(reps,size);
    for(int i = 0; dec && (i<reps);i++)
        erode_size[i] = size-i;
}

void ColorFilter::recalc_dilate(int size, int reps, bool dec){
    dilate_size = std::vector<int>(reps,size);
    for(int i = 0; dec && (i<reps);i++)
        dilate_size[i] = size-i;
}

int ColorFilter::get_erode_geometry(){
    return erode_geometry;
}

int ColorFilter::get_dilate_geometry(){
    return dilate_geometry;
}

void ColorFilter::set_erode_geometry(int id){
    erode_geometry = id;
}

void ColorFilter::set_dilate_geometry(int id){
    dilate_geometry = id;
}

bool ColorFilter::getHoughActive(){
    return houghActive;
}

void ColorFilter::setHoughActive(bool state){
    houghActive = state;
}

bool ColorFilter::getEdgeActive(){
    return edgeActive;
}

void ColorFilter::setEdgeActive(bool state){
    edgeActive = state;
}

int ColorFilter::getCannyThreshold(){
    return cannyThreshold;
}

void ColorFilter::setCannyThreshold(int val){
    cannyThreshold = val;
}

int ColorFilter::getCentersThreshold(){
    return centersThreshold;
}

void ColorFilter::setCentersThreshold(int val){
    centersThreshold = val;
}

int ColorFilter::getMinCenterDistance(){
    return minCenterDistance;
}

void ColorFilter::setMinCenterDistance(int val){
    minCenterDistance = val;
}

int ColorFilter::getMinRadius(){
    return minRadius;
}

void ColorFilter::setMinRadius(int val){
    minRadius = val;
}

int ColorFilter::getMaxRadius(){
    return maxRadius;
}

void ColorFilter::setmMxRadius(int val){
    maxRadius = val;
}

QColor ColorFilter::getfilterColor(){
    return filterColor;
}

void ColorFilter::setfilterColor(int h, int s, int v){
    filterColor.setHsv(h,s,v);
}
