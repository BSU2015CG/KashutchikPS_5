#include "histogram.h"
#include <QPainter>
#include <QPaintEvent>

Histogram::Histogram(QWidget *parent) : QWidget(parent) { max = 0; }

void Histogram::setData(QVector<int> _in_data, QColor _in_colour)
{
    colour = _in_colour;
    data = _in_data;

    max = 0;
    int counter = 0;
    for (int i = 0; i < data.size(); i++) {
        max = max < data[i] ? data[i] : max;
        mean += data[i] * i;
        counter += data[i];
    }
    mean /= counter;
}

void Histogram::paintEvent(QPaintEvent *event)
{
    if (max != 0) {
        int height = event->rect().height();

        QPainter painter;
        painter.begin(this);

        int textHeight = painter.fontMetrics().height();
        painter.setPen(QPen(Qt::transparent));
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, data.size(), height);

        painter.setPen(QPen(colour));
        for (int i = 0; i < data.size(); i++) {
            painter.drawLine(i, height - textHeight, i,
                             (height - textHeight) * (max - data[i]) / max);
        }

        QString text = "Mean: " + QString::number(mean);
        painter.setPen(QPen(Qt::black));
        QRect rect = painter.fontMetrics().boundingRect(text);
        QRect textRect(painter.fontMetrics().width(" "), height - rect.height(), rect.width(),
                       rect.height());
        painter.drawText(textRect, Qt::AlignCenter, text);

        painter.end();
    }
}
