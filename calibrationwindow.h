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
#include <QInputDialog>
#include <QDateTime>
#include <QGraphicsScene>
#include "ocvframe.h"
#include <QTimer>

namespace Ui {
class CalibrationWindow;
}

class CalibrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationWindow(QWidget *parent = 0);
    explicit CalibrationWindow(QWidget *parent = 0,std::vector<ColorFilter> *inFilters = NULL, QString filterPath = "Data/defaultFilters.csv");
    explicit CalibrationWindow(QWidget *parent, QString filterPath, QString videoPath,bool still);
    ~CalibrationWindow();
    std::vector<ColorFilter> *getFilters();
    void onExit();

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

    void on_AddButton_clicked();

    void on_DeleteButton_clicked();

    void on_SaveButton_clicked();

    void on_ClearButton_clicked();

    void on_applyCB_stateChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

signals:
    void validate();

private:
    Ui::CalibrationWindow *ui;
    std::vector<ColorFilter> *filters;
    void loadFilters(QString filePath);
    void loadCurrentFilter(ColorFilter filter);
    QGraphicsScene frame;
    OcvFrame *ocvf;
    QTimer timer;
};

#endif // CALIBRATIONWINDOW_H
