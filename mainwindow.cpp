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
    updateFilterList();
}

void MainWindow::on_calibration_Button_clicked()
{
    std::vector<ColorFilter> *temp = new std::vector<ColorFilter>();
    calibration = new CalibrationWindow(this,temp,"Data/defaultFilters.txt");
    calibration->setModal(true);
    connect(calibration,SIGNAL(validate()),this,SLOT(setFilters()));
    connect(calibration,SIGNAL(rejected()),this,SLOT(cancelFilters()));
    calibration->exec();
}

void MainWindow::setFilters(){
    qDebug() << "Accepted Filters";
    filters->clear();
    for(int i =0; i < calibration->getFilters()->size(); i++){
        filters->push_back(calibration->getFilters()->at(i));
        qDebug() << calibration->getFilters()->at(i).get_name();
    }
    calibration->onExit();
    delete calibration;
    updateFilterList();
}

void MainWindow::cancelFilters(){
    calibration->onExit();
    qDebug() << "Filters Configuration Dismissed";
    delete calibration;
}


void MainWindow::updateFilterList(){
    ui->filterList->clear();
    for(int i = 0; i < filters->size(); i++){
        ui->filterList->addItem(filters->at(i).get_name());
        qDebug() << filters->at(i).get_name();
    }
}
