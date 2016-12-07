#ifndef VIDEO_H
#define VIDEO_H

#include "buffer.h"
#include "colorfilter.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>
#include <QFileInfo>
#include <QFile>

class Video
{
public:
    // Constructors
    Video();
    Video(QString path);
    Video(QString path, int buffer_size);
    // Destructor
};

#endif // VIDEO_H
