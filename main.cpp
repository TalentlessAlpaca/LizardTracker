#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "colorfilter.h"
#include "buffer.h"

using namespace std;
using namespace cv;



void printBuffer(Buffer<int> * inB);

int main(int argc, char *argv[])
{
    Buffer <int> waka(10);
    printBuffer(&waka);
    for(int i = 0; i<2*waka.size(); i++){
        waka.push_back(i+1);
        printBuffer(&waka);
    }

    std::vector<ColorFilter> *filters = new std::vector<ColorFilter>();
    QApplication a(argc, argv);
    //a.setWindowIcon(QIcon("./Data/icon.ico"));
    MainWindow w;
    w.iniFilters(filters);
    w.show();

    return a.exec();
}


void printBuffer(Buffer<int> * inB){
    qDebug() << "----------------------";
    for(int i = 0; i < inB->size(); i++)
        qDebug() << inB->at(i);
    qDebug() << "----------------------";
}
