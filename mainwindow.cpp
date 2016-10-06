#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnDisplay_clicked()
{
    cv::namedWindow(inputImage);
    cv::namedWindow(outputImage);
    cv::VideoCapture cap(0);
    do{
        cap >> inputFrame;
        if(hsvEnabled){
            cv::cvtColor( inputFrame, outputFrame, CV_BGR2HSV );
            cv::blur( outputFrame, outputFrame,cv:: Size(3,3) );
            cv::inRange(outputFrame,cv::Scalar(minHueVal,minSatVal,minValVal),cv::Scalar(maxHueVal,maxSatVal,maxValVal),outputFrame);
            if(erodeDilateEnabled){
                erodeElement = cv::getStructuringElement(erodeGeom,
                                                         cv::Size(erodeSize,erodeSize));
                for(int i = 0; i < erodeReps; i++)
                    cv::erode(outputFrame,outputFrame,erodeElement);

                dilateElement = cv::getStructuringElement(dilateGeom,
                                                         cv::Size(dilateSize,dilateSize));
                for(int i = 0; i < dilateReps; i++)
                    cv::dilate(outputFrame,outputFrame,dilateElement);
            }
            if(trackEnabled){
                //QString myString = "waka/n";
                //std::count << myString.toStdString().data();
                if(!erodeDilateEnabled) cv::cvtColor( inputFrame, outputFrame, CV_BGR2GRAY );
                cv::GaussianBlur(outputFrame, outputFrame, cv::Size(9, 9), 2, 2);
               // cv::Canny(outputFrame,outputFrame,(double)cannyThresold,3*(double)cannyThresold,kerSize);
                std::vector<cv::Vec3f> circles;
                cv::HoughCircles( outputFrame, circles, CV_HOUGH_GRADIENT, 1, outputFrame.rows/16, hc_p1, hc_p2,0,0);
                QString num = "Circles: " + QString::number(circles.size());
                ui->trackedNumber->setPlainText(num);
                for(size_t i = 0; i < circles.size();i++){
                    //cv::Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
                    cv::Point center(std::round(circles[i][0]), std::round(circles[i][1]));
                    int radius = std::round(circles[i][2]);
                    cv::circle(inputFrame, center, radius, cv::Scalar(0, 255, 0), 5);
                }
                if(circles.size() < 1)
                    drawSquare(inputFrame,cv::Point(-10+inputFrame.rows/2,-10+inputFrame.cols/2),cv::Point(10+inputFrame.rows/2,10+inputFrame.rows/2));
            }
        }
        else outputFrame = inputFrame.clone();
        imshow(inputImage,inputFrame);
        imshow(outputImage,outputFrame);
    }while(cv::waitKey(30)<0);
    cv::destroyAllWindows();
}

void MainWindow::on_minHueSlider_sliderMoved(int position)
{
    minHueVal = position;
}
void MainWindow::on_maxHueSlider_sliderMoved(int position)
{
    maxHueVal = position;
}

void MainWindow::on_minSatSlider_sliderMoved(int position)
{
    minSatVal = position;
}
void MainWindow::on_maxSatSlider_sliderMoved(int position)
{
    maxSatVal = position;
}

void MainWindow::on_minValSlider_sliderMoved(int position)
{
    minValVal = position;
}
void MainWindow::on_maxValSlider_sliderMoved(int position)
{
    maxValVal = position;
}

void MainWindow::on_erodeCB_currentIndexChanged(int index)
{
    switch(index){
        case 0:
            erodeGeom = cv::MORPH_RECT;
        break;
        case 1:
            erodeGeom = cv::MORPH_CROSS;
        break;
        default:
            erodeGeom = cv::MORPH_ELLIPSE;
        break;
    }
}

void MainWindow::on_dilateCB_currentIndexChanged(int index)
{
    switch(index){
        case 0:
            dilateGeom = cv::MORPH_RECT;
        break;
        case 1:
            dilateGeom = cv::MORPH_CROSS;
        break;
        default:
            dilateGeom = cv::MORPH_ELLIPSE;
        break;
    }
}

void MainWindow::on_erodeSizeBox_valueChanged(int arg1)
{
    erodeSize = arg1;
}

void MainWindow::on_dilateSizeBox_valueChanged(int arg1)
{
    dilateSize = arg1;
}

void MainWindow::on_erodeRepsBox_valueChanged(int arg1)
{
    erodeReps = arg1;
}

void MainWindow::on_dilateRepsBox_valueChanged(int arg1)
{
    dilateReps = arg1;
}

void MainWindow::on_Color_GB_toggled(bool arg1)
{
    hsvEnabled = !hsvEnabled;
}

void MainWindow::on_ErodeDilate_GB_toggled(bool arg1)
{
    erodeDilateEnabled = !erodeDilateEnabled;
}

void MainWindow::on_Tracking_GB_toggled(bool arg1)
{
    trackEnabled = !trackEnabled;
}

void MainWindow::drawCirle(cv::Mat &frame,cv::Point pos, int radious){
    int thickness = 3;
    int lineType = 8;

    cv::circle(frame, pos, radious, cv::Scalar(0, 255, 0), 5);

//    cv::ellipse( frame,
//             pos,
//             cv::Size( radious*2, radious*2 ),
//             0,
//             0,
//             360,
//             cv::Scalar( 255, 0, 0 ),
//             thickness,
//             lineType );

}

void MainWindow::drawSquare(cv::Mat &frame,cv::Point p1, cv::Point p2){
    int thickness = 3;
    int lineType = 8;

    cv::rectangle( frame,
                   p1,
                   p2,
                   cv::Scalar( 0, 255, 255 ),
                   thickness,
                   lineType );

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    kerSize = arg1;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    cannyThresold = position;
}

void MainWindow::on_Param1_HT_sliderMoved(int position)
{
    hc_p1 = position;
}

void MainWindow::on_Param2_HT_sliderMoved(int position)
{
    hc_p2 = position;
}