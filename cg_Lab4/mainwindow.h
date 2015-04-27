#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadImage(QString _in_fileName);

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage nonlinearFilter(QImage img, int areaSize, int (*filter)(QVector<int>));
    QImage niblackThresholding(QImage img, int areaSize, int k);
    QImage adaptiveThresholding(QImage img, int k, double a);
    double brightness(QRgb value);
    static int median(QVector<int> vector);
    static int min(QVector<int> vector);
    static int max(QVector<int> vector);
    double mean(QVector<double> vector);
    double mse(QVector<double> vector, double arithmeticMean);

private slots:
    void on_btnSelectPicture_clicked();
    void on_btnProcess_clicked();
};

#endif // MAINWINDOW_H
