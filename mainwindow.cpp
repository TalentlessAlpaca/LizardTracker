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

void MainWindow::iniFilters(std::vector<ColorFilter> *inFilters){
    filters = inFilters;
}

void MainWindow::on_calibration_Button_clicked()
{
    calibration = new CalibrationWindow(this,filters,"Data/defaultFilters.txt");
    calibration->setModal(true);
    connect(calibration,SIGNAL(validate()),this,SLOT(setFilters()));
    calibration->exec();
}

void MainWindow::setFilters(){
    qDebug() << "Accepted Filters";
    //filters = std::vector<ColorFilter>(calibration->getFilters()->begin(),calibration->getFilters()->end());
    for(int i = 0; i<filters->size();i++) qDebug() << filters->at(i).get_name();
    calibration->onExit();
    delete calibration;
}
