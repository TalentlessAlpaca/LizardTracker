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

void MainWindow::initialize_filters(std::vector<ColorFilter> *inFilters){
    filters = inFilters;
    update_filter_list();
}

void MainWindow::on_calibration_Button_clicked()
{
    std::vector<ColorFilter> *temp = new std::vector<ColorFilter>();
    calibration = new CalibrationWindow(this,temp,"Data/defaultFilters.txt");
    calibration->setModal(true);
    connect(calibration,SIGNAL(validate()),this,SLOT(set_filters()));
    connect(calibration,SIGNAL(rejected()),this,SLOT(cancel_filters()));
    calibration->exec();
}

void MainWindow::set_filters(){
    qDebug() << "Accepted Filters";
    filters->clear();
    for(int i =0; i < calibration->get_filters()->size(); i++){
        filters->push_back(calibration->get_filters()->at(i));
        qDebug() << calibration->get_filters()->at(i).get_name();
    }
    calibration->onExit();
    delete calibration;
    update_filter_list();
}

void MainWindow::cancel_filters(){
    calibration->onExit();
    qDebug() << "Filters Configuration Dismissed";
    delete calibration;
}


void MainWindow::update_filter_list(){
    ui->filterList->clear();
    for(int i = 0; i < filters->size(); i++){
        ui->filterList->addItem(filters->at(i).get_name());
        qDebug() << filters->at(i).get_name();
    }
}
