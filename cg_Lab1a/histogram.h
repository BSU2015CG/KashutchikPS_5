#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>

class Histogram : public QWidget
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);
    void setData(QVector<int> _in_data, QColor _in_colour);

protected:
    void paintEvent(QPaintEvent *event);
    QVector<int> data;
    QColor colour;
    int max;
    double mean;

signals:

public slots:
};

#endif // HISTOGRAM_H
