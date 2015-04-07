#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rasteralogithm.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rBtnBresenham->click();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_rBtnStepwise_clicked(bool checked)
{
    if (checked)
        ui->graphicsView->setRasterAlgorithm(Stepwise);
}

void MainWindow::on_rBtnDDA_clicked(bool checked)
{
    if (checked)
        ui->graphicsView->setRasterAlgorithm(DDA);
}

void MainWindow::on_rBtnBresenham_clicked(bool checked)
{
    if (checked)
        ui->graphicsView->setRasterAlgorithm(Bresenham);
}

void MainWindow::on_graphicsView_timeElapsed(qint64 _in_time)
{
    ui->plainTextEdit->appendPlainText(QString::number(_in_time) + " ns.");
}
