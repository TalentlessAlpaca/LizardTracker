#include "ocvframe.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

OcvFrame::OcvFrame(std::vector<ColorFilter> *inFilters)
{
    videoStream = cv::VideoCapture("Data/video_zenfone.mp4");
    qDebug() << "Video Open";
    filters = inFilters;
}

OcvFrame::OcvFrame(cv::VideoCapture vc){
    videoStream = vc;
}

QRectF OcvFrame::boundingRect() const
{
    int width  = displayFrame.cols;
    int height = displayFrame.rows;
    return QRectF(0 , 0 ,
                  width, height);
}

QPainterPath OcvFrame::shape() const
{
    QPainterPath path;
    int width  = displayFrame.cols;
    int height = displayFrame.rows;
    path.addRect(0, 0, width, height);
    return path;
}

void OcvFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    prepareGeometryChange();
    painter->drawImage(0,0,displayImage,0,0,displayImage.size().width(),displayImage.size().height());
    //qDebug() << "Display: " <<displayImage.size();
}

void OcvFrame::advance(int step)
{
    if (!step)
        return;

    // Load newFrame and do some pre-processing
    //qDebug() << "Retrieving Frame";
    int e1 = cv::getTickCount();
    if(play || nextFrame){
        videoStream >> currentFrame;
        nextFrame = false;
        //qDebug() << "Retrieved Frame";
        if (currentFrame.empty()){
            //cvSetCaptureProperty(videoStream, CV_CAP_PROP_POS_AVI_RATIO , 0);
            videoStream.set(CV_CAP_PROP_POS_AVI_RATIO,0);
            videoStream >> currentFrame;
        }
        //qDebug() << "Waka";
        e1 = cv::getTickCount();
        int tHeight = currentFrame.rows;
        int tWidth = currentFrame.cols;
        if(((double)tWidth/(double)tHeight)>1.48){
            // Rotate image
            cv::transpose(currentFrame,currentFrame);
            cv::flip(currentFrame,currentFrame,-1);
            tHeight = currentFrame.rows;
            tWidth = currentFrame.cols;
        }

        //qDebug() << tWidth << "," << tHeight;
        cv::blur( currentFrame, currentFrame,cv:: Size(3,3) );
        cv::Size newSize(containerWidth,containerWidth*tHeight/tWidth);
        cv::resize(currentFrame,currentFrame,newSize);

        nextFrame = false;
    }

    cv::cvtColor( currentFrame, intermediateFrame_A, CV_BGR2HSV );
    // Apply Filters
    for(int i = 0; filteringActive && (i<filters->size()) && (activeFilter<0); i++){
        //qDebug() << "Filtering Frame: All Filters";
        ColorFilter *cf= &filters->at(i);

        // Filter by HSV Vals
        cv::inRange(intermediateFrame_A,cf->get_minVals(),cf->get_maxVals(),intermediateFrame_B);
        // Erode
        for(int e = 0; e < cf->get_erode().size(); e++){
            cv::Mat erodeElement = cv::getStructuringElement(cf->get_erode_geometry(),
                                                     cv::Size(cf->get_erode().at(e),cf->get_erode().at(e)));
            cv::erode(intermediateFrame_B,intermediateFrame_B,erodeElement);
        }
        // Dilate
        for(int d = 0; d < cf->get_dilate().size(); d++){
            cv::Mat dilateElement = cv::getStructuringElement(cf->get_dilate_geometry(),
                                                     cv::Size(cf->get_dilate().at(d),cf->get_dilate().at(d)));
            cv::dilate(intermediateFrame_B,intermediateFrame_B,dilateElement);
        }

        if(cf->getEdgeActive()){
            Canny( intermediateFrame_B, intermediateFrame_B, cf->getCannyThreshold()/3, cf->getCannyThreshold(), 3);
        }
        else if(cf->getHoughActive()){
            HoughCircles( intermediateFrame_B, circles, CV_HOUGH_GRADIENT, 1, (double)(cf->getMinCenterDistance()), (double)(cf->getCannyThreshold()),
                          (double)(cf->getCentersThreshold()), (double)(cf->get_minVals(), cf->getMaxRadius()) );
            qDebug() << "Filter " << i << ": " << circles.size() << " Circles found";

        }

        if(i==0) displayFrame = cv::Mat::zeros( cv::Size(currentFrame.cols,currentFrame.rows), CV_8U );
        cv::bitwise_or(intermediateFrame_B,displayFrame,displayFrame);

    }

    if(activeFilter>=0){
        //qDebug() << "Filtering Frame: Single Filter " << activeFilter;
        ColorFilter *cf= &filters->at(activeFilter);

        // Filter by HSV Vals
        cv::inRange(intermediateFrame_A,cf->get_minVals(),cf->get_maxVals(),intermediateFrame_B);
        // Erode
        for(int e = 0; e < cf->get_erode().size(); e++){
            cv::Mat erodeElement = cv::getStructuringElement(cf->get_erode_geometry(),
                                                     cv::Size(cf->get_erode().at(e),cf->get_erode().at(e)));
            cv::erode(intermediateFrame_B,intermediateFrame_B,erodeElement);
        }
        // Dilate
        for(int d = 0; d < cf->get_dilate().size(); d++){
            cv::Mat dilateElement = cv::getStructuringElement(cf->get_dilate_geometry(),
                                                     cv::Size(cf->get_dilate().at(d),cf->get_dilate().at(d)));
            cv::dilate(intermediateFrame_B,intermediateFrame_B,dilateElement);
        }

        if(cf->getEdgeActive()){
            Canny( intermediateFrame_B, intermediateFrame_B, cf->getCannyThreshold()/2, cf->getCannyThreshold(), 3);
        }
        else if(cf->getHoughActive()){
            HoughCircles( intermediateFrame_B, circles, CV_HOUGH_GRADIENT, 1, (double)(cf->getMinCenterDistance()), (double)(cf->getCannyThreshold()),
                          (double)(cf->getCentersThreshold()), cf->getMinRadius(), cf->getMaxRadius() );

            //qDebug() << "Filter " << activeFilter << ": " << circles.size() << " Circles found";
        }

        displayFrame = intermediateFrame_B;
    }


    //cv::GaussianBlur(displayFrame, displayFrame, cv::Size(9, 9), 2, 2);
    if(activeFilter>=0 && filters->at(activeFilter).getHoughActive() && filteringActive){
        ColorFilter *cf= &filters->at(activeFilter);

        displayFrame = cv::Mat::zeros( cv::Size(currentFrame.cols,currentFrame.rows), CV_8UC3);

        for( size_t i = 0; i < circles.size(); i++ ){

            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            //cv::Scalar fColor((double)(cf->getfilterColor().blue()),(double)(cf->getfilterColor().green()),(double)(cf->getfilterColor().red()));
            cv::Scalar fColor(255-(cf->getfilterColor().blue()),255-(cf->getfilterColor().green()),255-(cf->getfilterColor().red()));
            qDebug() << "Circle of color " << fColor[0] << fColor[1] << fColor[2] << "and radius " << radius;
            circle( displayFrame, center, 3, fColor, -1, 8, 0 );
            // circle outline
            circle( displayFrame, center, radius, fColor, 3, 8, 0 );
        }
    }
    else if(filteringActive) cv::cvtColor(displayFrame,displayFrame,CV_GRAY2RGB);
    else cv::cvtColor(currentFrame,displayFrame,CV_BGR2RGB);

    cv::GaussianBlur(displayFrame, displayFrame, cv::Size(9, 9), 2, 2);

    displayImage = Mat2QImage(displayFrame);

    int e2 = cv::getTickCount();

    update();

    if(fr >10){
        qDebug() << "Processing Time: " << (e2 - e1)/ cv::getTickFrequency();
        qDebug() << "Width: " << displayFrame.cols << ", Height: " << displayFrame.rows;
        qDebug() << "AR = " << ((double)displayFrame.rows/(double)displayFrame.cols);
        fr = 0;
    }
    else fr++;

}

QImage OcvFrame::Mat2QImage(const cv::Mat3b &src) {
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y) {
        const cv::Vec3b *srcrow = src[y];
        QRgb *destrow = (QRgb*)dest.scanLine(y);
        for (int x = 0; x < src.cols; ++x) {
            destrow[x] = qRgba(srcrow[x][0], srcrow[x][1], srcrow[x][2], 255);
        }
    }
    return dest;
}

void OcvFrame::closeStream(){
    videoStream.release();
}


void OcvFrame::activateFilter(bool state){
    filteringActive = state;
}

void OcvFrame::setActiveFilter(int filter){
    activeFilter = filter;
}

void OcvFrame::playVideo(bool val){
    play = val;
}

void OcvFrame::advanceFrame(bool val){
    nextFrame = val;
}
