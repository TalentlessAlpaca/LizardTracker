#include "colorfilter.h"

ColorFilter::ColorFilter()
{
    erode_size = std::vector<int>(3,3);
    dilate_size = std::vector<int>(3,3);
    min_vals = cv::Scalar(0,0,0);
    max_vals = cv::Scalar(255,255,255);
}

ColorFilter::ColorFilter(QString fName, int minH, int maxH, int minS, int maxS, int minV, int maxV, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo){

    erode_size = std::vector<int>(eReps,eSize);
    for(int i=0; eDec && (i<eReps); i++) erode_size[i] = eSize-i;

    dilate_size = std::vector<int>(dReps,dSize);
    for(int i=0; dDec && (i<dReps); i++) dilate_size[i] = dSize-i;

    min_vals = cv::Scalar(minH,minS,minV);
    max_vals = cv::Scalar(maxH,maxS,maxV);

    filter_name = fName;

    erode_geometry = eGeo;
    dilate_geometry = dGeo;
}

ColorFilter::ColorFilter(QString fName, cv::Scalar _min_vals, cv::Scalar _max_vals, int eSize, int eReps, bool eDec, int dSize, int dReps, bool dDec, int eGeo, int dGeo){
    erode_size = std::vector<int>(eReps,eSize);
    for(int i=0; eDec && (i<eReps); i++) erode_size[i] = eSize-i;

    dilate_size = std::vector<int>(dReps,dSize);
    for(int i=0; dDec && (i<dReps); i++) dilate_size[i] = dSize-i;;

    min_vals = cv::Scalar(_min_vals[0],_min_vals[1],_min_vals[1]);
    max_vals = cv::Scalar(_max_vals[0],_max_vals[1],_max_vals[1]);

    filter_name = fName;

    erode_geometry = eGeo;
    dilate_geometry = dGeo;
}

std::vector<int> ColorFilter::get_erode(){
    return erode_size;
}

std::vector<int> ColorFilter::get_dilate(){
    return dilate_size;
}

cv::Scalar ColorFilter::get_min_vals(){
    return min_vals;
}

cv::Scalar ColorFilter::get_max_vals(){
    return max_vals;
}

QString ColorFilter::get_name(){
    return filter_name;
}

void ColorFilter::set_min_vals(int mh, int ms, int mv){
    min_vals = cv::Scalar(mh,ms,mv);

    set_filter_color((min_vals[0]+max_vals[0])/2,(min_vals[1]+max_vals[1])/2,(min_vals[2]+max_vals[2])/2);
}

void ColorFilter::set_max_vals(int mh, int ms, int mv){
    max_vals = cv::Scalar(mh,ms,mv);
    set_filter_color((min_vals[0]+max_vals[0])/2,(min_vals[1]+max_vals[1])/2,(min_vals[2]+max_vals[2])/2);
}


void ColorFilter::recalc_erode(int size, int reps, bool dec){
    erode_size = std::vector<int>(reps,size);
    for(int i = 0; dec && (i<reps);i++)
        erode_size[i] = size-i;

    qDebug() << "-- Erode --";

    for(int i = 0; i < erode_size.size(); i++)
        qDebug() << erode_size.at(i);
}

void ColorFilter::recalc_dilate(int size, int reps, bool dec){
    dilate_size = std::vector<int>(reps,size);
    for(int i = 0; dec && (i<reps);i++){
        dilate_size[i] = size-i;
    }

    qDebug() << "-- Dilate --";

    for(int i = 0; i < dilate_size.size(); i++)
        qDebug() << dilate_size.at(i);
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

bool ColorFilter::get_hough_active(){
    return hough_active;
}

void ColorFilter::set_hough_active(bool state){
    hough_active = state;
}

bool ColorFilter::get_edge_active(){
    return edge_active;
}

void ColorFilter::set_edge_active(bool state){
    edge_active = state;
}

int ColorFilter::get_canny_threshold(){
    return canny_threshold;
}

void ColorFilter::set_canny_threshold(int val){
    canny_threshold = val;
}

int ColorFilter::get_centers_threshold(){
    return centers_threshold;
}

void ColorFilter::set_centers_threshold(int val){
    centers_threshold = val;
}

int ColorFilter::get_min_center_distance(){
    return min_center_distance;
}

void ColorFilter::set_min_center_distance(int val){
    min_center_distance = val;
}

int ColorFilter::get_min_radius(){
    return min_radius;
}

void ColorFilter::set_min_radius(int val){
    min_radius = val;
}

int ColorFilter::get_max_radius(){
    return max_radius;
}

void ColorFilter::set_max_radius(int val){
    max_radius = val;
}

QColor ColorFilter::get_filter_color(){
    return filter_color;
}

void ColorFilter::set_filter_color(int h, int s, int v){
    filter_color.setHsv(h,s,v);
}
