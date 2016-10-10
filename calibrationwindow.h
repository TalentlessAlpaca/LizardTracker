#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QDialog>
#include "colorfilter.h"
#include <QFileDialog>

namespace Ui {
class CalibrationWindow;
}

class CalibrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationWindow(QWidget *parent = 0);
    explicit CalibrationWindow(QWidget *parent = 0,QString filePath = "defaultFilters.csv");
    ~CalibrationWindow();

private:
    Ui::CalibrationWindow *ui;
    loadFilters(QString filePath);
    loadCurrentFilter(ColorFilter filter);
};

#endif // CALIBRATIONWINDOW_H
