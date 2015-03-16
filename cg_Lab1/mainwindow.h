#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <colourmodel.h>

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

protected:
    ColourModel *colourModel;
    void changeBackGroundColourFromModel();
    void updateRGBfromModel();
    void updateHSVfromModel();
    void updateCMYKfromModel();
    void updateLUVfromModel();

private slots:
    void rgbSliderMoved();
    void rgbSBoxChanged();
    void hsvSliderMoved();
    void hsvSBoxChanged();
    void cmykSliderMoved();
    void cmykSBoxChanged();
    void luvSliderMoved();
    void luvSBoxChanged();
    void colourModelChanged(QString _in_model);
    void on_btnPalette_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
