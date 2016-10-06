#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

/*
    if( argc != 2)
        {
         cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
         return -1;
        }

        Mat image;
        image = imread(argv[1]);   // Read the file

        if(! image.data )         // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }

        // Create a window for display.
        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );

        // Show our image inside it.
        imshow( "Display window", image );

        waitKey(0);              // Wait for a keystroke in the window
        return 0;
*/
}
