#include "ocvframe.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

OcvFrame::OcvFrame(std::vector<ColorFilter> *inFilters)
{
    video_stream = cv::VideoCapture("Data/video_zenfone.mp4");
    qDebug() << "Video Open";
    filters = inFilters;
}

OcvFrame::OcvFrame(cv::VideoCapture vc){
    video_stream = vc;
}

QRectF OcvFrame::boundingRect() const
{
    int width  = display_frame.cols;
    int height = display_frame.rows;
    return QRectF(0 , 0 ,
                  width, height);
}

QPainterPath OcvFrame::shape() const
{
    QPainterPath path;
    int width  = display_frame.cols;
    int height = display_frame.rows;
    path.addRect(0, 0, width, height);
    return path;
}

void OcvFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    prepareGeometryChange();
    painter->drawImage(0,0,display_image,0,0,display_image.size().width(),display_image.size().height());
    //qDebug() << "Display: " <<display_image.size();
}

void OcvFrame::advance(int step)
{
    if (!step)
        return;

    // Load newFrame and do some pre-processing
    //qDebug() << "Retrieving Frame";
    int e1 = cv::getTickCount();
    if(play || next_frame){
        video_stream >> current_frame;
        next_frame = false;
        //qDebug() << "Retrieved Frame";
        if (current_frame.empty()){
            //cvSetCaptureProperty(video_stream, CV_CAP_PROP_POS_AVI_RATIO , 0);
            video_stream.set(CV_CAP_PROP_POS_AVI_RATIO,0);
            video_stream >> current_frame;
        }
        //qDebug() << "Waka";
        e1 = cv::getTickCount();
        int tHeight = current_frame.rows;
        int tWidth = current_frame.cols;
        if(((double)tWidth/(double)tHeight)>1.48){
            // Rotate image
            cv::transpose(current_frame,current_frame);
            cv::flip(current_frame,current_frame,-1);
            tHeight = current_frame.rows;
            tWidth = current_frame.cols;
        }

        //qDebug() << tWidth << "," << tHeight;
        cv::blur( current_frame, current_frame,cv:: Size(3,3) );
        cv::Size newSize(container_width,container_width*tHeight/tWidth);
        cv::resize(current_frame,current_frame,newSize);

        next_frame = false;
    }

    cv::cvtColor( current_frame, intermediate_frame_A, CV_BGR2HSV );
    // Apply Filters
    for(int i = 0; filtering_active && (i<filters->size()) && (active_filter<0); i++){
        //qDebug() << "Filtering Frame: All Filters";
        ColorFilter *cf= &filters->at(i);

        // Filter by HSV Vals
        cv::inRange(intermediate_frame_A,cf->get_min_vals(),cf->get_max_vals(),intermediate_frame_B);
        // Erode
        for(int e = 0; e < cf->get_erode().size(); e++){
            cv::Mat erodeElement = cv::getStructuringElement(cf->get_erode_geometry(),
                                                     cv::Size(cf->get_erode().at(e),cf->get_erode().at(e)));
            cv::erode(intermediate_frame_B,intermediate_frame_B,erodeElement);
        }
        // Dilate
        for(int d = 0; d < cf->get_dilate().size(); d++){
            cv::Mat dilateElement = cv::getStructuringElement(cf->get_dilate_geometry(),
                                                     cv::Size(cf->get_dilate().at(d),cf->get_dilate().at(d)));
            cv::dilate(intermediate_frame_B,intermediate_frame_B,dilateElement);
        }

        if(cf->get_edge_active()){
            Canny( intermediate_frame_B, intermediate_frame_B, cf->get_canny_threshold()/3, cf->get_canny_threshold(), 3);
        }
        else if(cf->get_hough_active()){
            HoughCircles( intermediate_frame_B, circles, CV_HOUGH_GRADIENT, 1, (double)(cf->get_min_center_distance()), (double)(cf->get_canny_threshold()),
                          (double)(cf->get_centers_threshold()), (double)(cf->get_min_vals(), cf->get_max_radius()) );
            qDebug() << "Filter " << i << ": " << circles.size() << " Circles found";

        }

        if(i==0) display_frame = cv::Mat::zeros( cv::Size(current_frame.cols,current_frame.rows), CV_8U );
        cv::bitwise_or(intermediate_frame_B,display_frame,display_frame);

    }

    if(active_filter>=0){
        //qDebug() << "Filtering Frame: Single Filter " << active_filter;
        ColorFilter *cf= &filters->at(active_filter);

        // Filter by HSV Vals
        cv::inRange(intermediate_frame_A,cf->get_min_vals(),cf->get_max_vals(),intermediate_frame_B);
        // Erode
        for(int e = 0; e < cf->get_erode().size(); e++){
            cv::Mat erodeElement = cv::getStructuringElement(cf->get_erode_geometry(),
                                                     cv::Size(cf->get_erode().at(e),cf->get_erode().at(e)));
            cv::erode(intermediate_frame_B,intermediate_frame_B,erodeElement);
        }
        // Dilate
        for(int d = 0; d < cf->get_dilate().size(); d++){
            cv::Mat dilateElement = cv::getStructuringElement(cf->get_dilate_geometry(),
                                                     cv::Size(cf->get_dilate().at(d),cf->get_dilate().at(d)));
            cv::dilate(intermediate_frame_B,intermediate_frame_B,dilateElement);
        }

        if(cf->get_edge_active()){
            Canny( intermediate_frame_B, intermediate_frame_B, cf->get_canny_threshold()/2, cf->get_canny_threshold(), 3);
        }
        else if(cf->get_hough_active()){
            HoughCircles( intermediate_frame_B, circles, CV_HOUGH_GRADIENT, 1, (double)(cf->get_min_center_distance()), (double)(cf->get_canny_threshold()),
                          (double)(cf->get_centers_threshold()), cf->get_min_radius(), cf->get_max_radius() );

            //qDebug() << "Filter " << active_filter << ": " << circles.size() << " Circles found";
        }

        display_frame = intermediate_frame_B;
    }


    //cv::GaussianBlur(display_frame, display_frame, cv::Size(9, 9), 2, 2);
    if(active_filter>=0 && filters->at(active_filter).get_hough_active() && filtering_active){
        ColorFilter *cf= &filters->at(active_filter);

        display_frame = cv::Mat::zeros( cv::Size(current_frame.cols,current_frame.rows), CV_8UC3);

        for( size_t i = 0; i < circles.size(); i++ ){

            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            //cv::Scalar fColor((double)(cf->get_filter_color().blue()),(double)(cf->get_filter_color().green()),(double)(cf->get_filter_color().red()));
            cv::Scalar fColor(255-(cf->get_filter_color().blue()),255-(cf->get_filter_color().green()),255-(cf->get_filter_color().red()));
            qDebug() << "Circle of color " << fColor[0] << fColor[1] << fColor[2] << "and radius " << radius;
            circle( display_frame, center, 3, fColor, -1, 8, 0 );
            // circle outline
            circle( display_frame, center, radius, fColor, 3, 8, 0 );
        }
    }
    else if(filtering_active) cv::cvtColor(display_frame,display_frame,CV_GRAY2RGB);
    else cv::cvtColor(current_frame,display_frame,CV_BGR2RGB);

    cv::GaussianBlur(display_frame, display_frame, cv::Size(9, 9), 2, 2);

    display_image = Mat_to_QImage(display_frame);

    int e2 = cv::getTickCount();

    update();

    if(fr >10){
        qDebug() << "Processing Time: " << (e2 - e1)/ cv::getTickFrequency();
        qDebug() << "Width: " << display_frame.cols << ", Height: " << display_frame.rows;
        qDebug() << "AR = " << ((double)display_frame.rows/(double)display_frame.cols);
        fr = 0;
    }
    else fr++;

}

QImage OcvFrame::Mat_to_QImage(const cv::Mat3b &src) {
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

void OcvFrame::close_stream(){
    video_stream.release();
}


void OcvFrame::activate_filter(bool state){
    filtering_active = state;
}

void OcvFrame::set_active_filter(int filter){
    active_filter = filter;
}

void OcvFrame::play_video(bool val){
    play = val;
}

void OcvFrame::advance_frame(bool val){
    next_frame = val;
}
