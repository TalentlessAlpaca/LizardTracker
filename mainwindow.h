#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QLinkedList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QElapsedTimer>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnDisplay_clicked();

    void on_minHueSlider_sliderMoved(int position);
    void on_maxHueSlider_sliderMoved(int position);
    void on_minSatSlider_sliderMoved(int position);
    void on_maxSatSlider_sliderMoved(int position);
    void on_minValSlider_sliderMoved(int position);
    void on_maxValSlider_sliderMoved(int position);

    void on_Color_GB_toggled(bool arg1);

    void on_erodeCB_currentIndexChanged(int index);

    void on_dilateCB_currentIndexChanged(int index);

    void on_ErodeDilate_GB_toggled(bool arg1);

    void on_erodeSizeBox_valueChanged(int arg1);

    void on_dilateSizeBox_valueChanged(int arg1);

    void on_erodeRepsBox_valueChanged(int arg1);

    void on_dilateRepsBox_valueChanged(int arg1);

    void on_Tracking_GB_toggled(bool arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_sliderMoved(int position);

    void on_Param1_HT_sliderMoved(int position);

    void on_Param2_HT_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    // Window names
    cv::String inputImage = "input";
    cv::String outputImage = "output";
    // Working Frames
    cv::Mat inputFrame;
    cv::Mat outputFrame;
    cv::Mat erodeElement;
    cv::Mat dilateElement;
    // Filter Parameters
    int minHueVal = 0;
    int maxHueVal = 255;
    int minSatVal = 0;
    int maxSatVal = 255;
    int minValVal = 0;
    int maxValVal = 255;
    // Erode Dilate Parameters
    int erodeSize  = 1;
    int erodeGeom  = 2;
    int erodeReps  = 1;
    int dilateSize = 1;
    int dilateGeom = 2;
    int dilateReps = 1;
    // Tracking Parameters
    int trackGeometry = 0;
    int hc_p1 = 100;
    int hc_p2 = 20;
    std::vector<cv::Vec3f> detectedObjects;
    int cannyThresold = 10;
    int kerSize = 3;
    // Enabled Options
    bool hsvEnabled         = false;
    bool erodeDilateEnabled = false;
    bool trackEnabled       = false;
    void drawCirle(cv::Mat &frame,cv::Point pos, int radious);
    void drawSquare(cv::Mat &frame,cv::Point p1, cv::Point p2);
    QLinkedList<QString> trackData;
    unsigned int frameNo = 0;
    QElapsedTimer timer;
};

#endif // MAINWINDOW_H
