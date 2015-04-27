#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadImage(QString _in_fileName)
{
    image = QImage(_in_fileName);

    QPixmap picture = QPixmap::fromImage(image).scaled(500, 500, Qt::KeepAspectRatio);
    ui->labelPicture->setPixmap(picture);
}

void MainWindow::on_btnSelectPicture_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор изображения", QString(),
                                                    "Изображения (*.png *.jpg *.bmp *.gif)");
    if (fileName != QString())
        loadImage(fileName);
}

void MainWindow::on_btnProcess_clicked()
{
    QImage processedImage;

    if (ui->rBtnNonLinearFilter->isChecked()) {
        int (*filter)(QVector<int>) = NULL;
        if (ui->rBtnMedian->isChecked())
            filter = median;
        if (ui->rBtnMaximum->isChecked())
            filter = max;
        if (ui->rBtnMinimum->isChecked())
            filter = min;

        processedImage = nonlinearFilter(image, ui->sBoxArea->value(), filter);
    }

    if (ui->rBtnNiblackThresholding->isChecked())
        processedImage = niblackThresholding(image, ui->sBoxArea->value(), ui->sBoxK->value());

    if (ui->rBtnAdaptiveThresholding->isChecked())
        processedImage = adaptiveThresholding(image, ui->sBoxArea->value(), ui->sBoxA->value());

    QPixmap picture = QPixmap::fromImage(processedImage).scaled(500, 500, Qt::KeepAspectRatio);
    ui->labelResult->setPixmap(picture);
}

QImage MainWindow::nonlinearFilter(QImage img, int areaSize, int (*filter)(QVector<int>))
{
    QImage output(img.width(), img.height(), QImage::Format_RGB32);
    for (int i = 0; i < img.width(); i++)
        for (int j = 0; j < img.height(); j++) {
            QVector<int> red, green, blue;
            for (int ai = -areaSize; ai <= areaSize; ai++)
                for (int aj = -areaSize; aj <= areaSize; aj++)
                    if (i + ai >= 0 && i + ai < img.width() && j + aj >= 0
                        && j + aj < img.height()) {
                        QRgb pixel = img.pixel(i + ai, j + aj);
                        red.append(qRed(pixel));
                        green.append(qGreen(pixel));
                        blue.append(qBlue(pixel));
                    }
            output.setPixel(i, j, qRgb(filter(red), filter(green), filter(blue)));
        }
    return output;
}

QImage MainWindow::niblackThresholding(QImage img, int areaSize, int k)
{
    QImage output(img.width(), img.height(), QImage::Format_Mono);
    for (int i = 0; i < img.width(); i++)
        for (int j = 0; j < img.height(); j++) {
            QVector<double> pixel;
            for (int ai = -areaSize; ai <= areaSize; ai++)
                for (int aj = -areaSize; aj <= areaSize; aj++)
                    if (i + ai >= 0 && i + ai < img.width() && j + aj >= 0 && j + aj < img.height())
                        pixel.append(brightness(img.pixel(i + ai, j + aj)));
            double m = mean(pixel);
            double e = mse(pixel, m);
            double t = m + k * e;
            QRgb currentPixel = img.pixel(i, j);
            output.setPixel(i, j, brightness(currentPixel) > t ? 1 : 0);
        }
    return output;
}

QImage MainWindow::adaptiveThresholding(QImage img, int k, double a)
{
    QImage output(img.width(), img.height(), QImage::Format_Mono);
    double **p = new double *[img.width()];

    for (int m = 0; m < img.width(); m++) {
        p[m] = new double[img.height()];
        for (int n = 0; n < img.height(); n++) {
            int nPixels = 0;
            double sum = 0;
            for (int i = -k; i <= k; i++)
                for (int j = -k; j <= k; j++)
                    if (m + i >= 0 && m + i < img.width() && n + j >= 0 && n + j < img.height()) {
                        sum += brightness(img.pixel(m + i, n + j));
                        nPixels++;
                    }
            p[m][n] = sum / nPixels;
        }
    }

    for (int m = 0; m < img.width(); m++)
        for (int n = 0; n < img.height(); n++) {
            double t, _p = p[m][n];
            double min, max;
            bool stop = false;
            for (int _k = k; !stop; _k++) {
                min = 256;
                max = 0;

                for (int i = -_k; i <= _k; i++)
                    for (int j = -_k; j <= _k; j++)
                        if (m + i >= 0 && m + i < img.width() && n + j >= 0
                            && n + j < img.height()) {
                            double currentBrightness = brightness(img.pixel(m + i, n + j));
                            if (min > currentBrightness)
                                min = currentBrightness;
                            if (max < currentBrightness)
                                max = currentBrightness;
                        }

                double dmax = fabs(max - _p);
                double dmin = fabs(min - _p);

                stop = true;

                if (dmax > dmin)
                    t = a * (2. / 3. * min + 1. / 3. * _p);
                else if (dmax < dmin)
                    t = a * (1. / 3. * min + 2. / 3. * _p);
                else if (dmax == dmin && max == min)
                    t = a * _p;
                else
                    stop = false;
            }

            bool b = true;
            for (int i = -k; i <= k; i++)
                for (int j = -k; j <= k; j++)
                    if (m + i >= 0 && m + i < img.width() && n + j >= 0 && n + j < img.height()
                        && (i != 0 || j != 0))
                        if (p[m + i][n + j] - brightness(img.pixel(m, n)) <= t)
                            b = false;

            output.setPixel(m, n, b ? 0 : 1);
        }

    for (int i = 0; i < img.width(); i++)
        delete[] p[i];
    delete[] p;

    return output;
}

double MainWindow::brightness(QRgb value)
{
    return (qRed(value) + qGreen(value) + qBlue(value)) / 3.;
}

int MainWindow::median(QVector<int> vector)
{
    if (vector.isEmpty())
        return 0;
    qSort(vector.begin(), vector.end(), qLess<int>());
    return vector[vector.size() / 2];
}

int MainWindow::max(QVector<int> vector)
{
    int max = 0;
    for (int i = 0; i < vector.size(); i++)
        if (max < vector[i])
            max = vector[i];
    return max;
}

int MainWindow::min(QVector<int> vector)
{
    int min = 256;
    for (int i = 0; i < vector.size(); i++)
        if (min > vector[i])
            min = vector[i];
    return min;
}

double MainWindow::mean(QVector<double> vector)
{
    if (vector.isEmpty())
        return 0;
    double out = 0;
    for (int i = 0; i < vector.size(); i++)
        out += vector[i];
    return out / vector.size();
}

double MainWindow::mse(QVector<double> vector, double arithmeticMean)
{
    if (vector.isEmpty())
        return 0;
    double out = 0;
    for (int i = 0; i < vector.size(); i++)
        out += (vector[i] - arithmeticMean) * (vector[i] - arithmeticMean);
    return sqrt(out / vector.size());
}
