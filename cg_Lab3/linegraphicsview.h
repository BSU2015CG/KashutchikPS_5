#ifndef LINEGRAPHICSVIEW_H
#define LINEGRAPHICSVIEW_H

#include <QGraphicsView>
#include "rasteralogithm.h"

class LineGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    LineGraphicsView(QWidget *parent = NULL);
    void setRasterAlgorithm(RasterAlgorithm _in_algorithm);

signals:
    void timeElapsed(qint64 _in_time);

protected:
    QGraphicsScene *scene;
    RasterAlgorithm algorithm;
    QPoint firstPoint;
    QPoint secondPoint;
    QSize pixelSize;
    void drawLine(QPoint _in_begin, QPoint _in_end);
    void drawStepwiseLine(QPoint _in_begin, QPoint _in_end);
    void drawDDALine(QPoint _in_begin, QPoint _in_end);
    QPoint switchToZeroOctant(int _in_octant, QPoint _in_p);
    void drawBresenhamLine(QPoint _in_begin, QPoint _in_end);
    void setPixel(QPoint _in_pixel, Qt::GlobalColor _in_colour);
    void wheelEvent(QWheelEvent *event);
    void enterEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawGrid(QRect rect);
};

#endif // LINEGRAPHICSVIEW_H
