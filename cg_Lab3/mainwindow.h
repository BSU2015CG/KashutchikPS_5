#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_rBtnStepwise_clicked(bool checked);
    void on_rBtnDDA_clicked(bool checked);
    void on_rBtnBresenham_clicked(bool checked);
    void on_graphicsView_timeElapsed(qint64 _in_time);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
