#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadImage(":/image.jpg");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadImage(QString _in_fileName)
{
    QImage image = QImage(_in_fileName);

    QPixmap picture = QPixmap::fromImage(image).scaled(500, 500, Qt::KeepAspectRatio);
    ui->labelPicture->setPixmap(picture);

    QVector<int> red(256);
    QVector<int> green(256);
    QVector<int> blue(256);

    uchar *bits = image.bits();
    for (int i = 0; i < image.width() * image.height() * 3; i += 4) {
        red[(int)bits[i]]++;
        green[(int)bits[i + 1]]++;
        blue[(int)bits[i + 2]]++;
    }

    ui->histogramRed->setData(red, QColor(Qt::red));
    ui->histogramGreen->setData(green, QColor(Qt::green));
    ui->histogramBlue->setData(blue, QColor(Qt::blue));
}

void MainWindow::on_btnSelectPicture_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор изображения", QString(),
                                                    "Изображения (*.png *.jpg *.bmp *.gif)");
    if (fileName != QString())
        loadImage(fileName);
}
