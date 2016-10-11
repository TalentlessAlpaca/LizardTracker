#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QDialog>
#include "colorfilter.h"
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

namespace Ui {
class CalibrationWindow;
}

class CalibrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationWindow(QWidget *parent = 0);
    explicit CalibrationWindow(QWidget *parent = 0,QString filePath = "Data/defaultFilters.csv");
    ~CalibrationWindow();
    std::vector<ColorFilter> *getFilters();

private slots:
    void on_FiltersList_currentRowChanged(int currentRow);

    void on_minVS_H_valueChanged(int value);
    void on_minVS_S_valueChanged(int value);
    void on_minVS_V_valueChanged(int value);
    void on_maxVS_H_valueChanged(int value);
    void on_maxVS_S_valueChanged(int value);
    void on_maxVS_V_valueChanged(int value);

    void on_LoadFileButton_clicked();

    void on_buttonBox_accepted();

    void on_minSB_H_valueChanged(int arg1);
    void on_minSB_S_valueChanged(int arg1);
    void on_minSB_V_valueChanged(int arg1);

    void on_maxSB_H_valueChanged(int arg1);
    void on_maxSB_S_valueChanged(int arg1);
    void on_maxSB_V_valueChanged(int arg1);

    void on_SizeSB_E_valueChanged(int arg1);
    void on_RepsSB_E_valueChanged(int arg1);

    void on_SizeSB_D_valueChanged(int arg1);
    void on_RepsSB_D_valueChanged(int arg1);

signals:
    void validate();

private:
    Ui::CalibrationWindow *ui;
    std::vector<ColorFilter> filters;
    void loadFilters(QString filePath);
    void loadCurrentFilter(ColorFilter filter);
};

#endif // CALIBRATIONWINDOW_H
