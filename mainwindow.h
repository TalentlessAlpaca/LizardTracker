#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
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
#include "calibrationwindow.h"
#include "colorfilter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initialize_filters(std::vector<ColorFilter> * inFilters);
    ~MainWindow();

private slots:
    void on_calibration_Button_clicked();
    void set_filters();
    void cancel_filters();

private:
  // Variables
    Ui::MainWindow *ui;
    QElapsedTimer timer;
    std::vector<ColorFilter> *filters;
    CalibrationWindow *calibration;
    std::vector<cv::Vec3f> circlesA;
    std::vector<cv::Vec3f> circlesB;
    bool cicle = false;

    QGraphicsSceneMouseEvent inScene;
    QGraphicsSceneMouseEvent outScene;
  // Functions
    void update_filter_list();
};

#endif // MAINWINDOW_H
