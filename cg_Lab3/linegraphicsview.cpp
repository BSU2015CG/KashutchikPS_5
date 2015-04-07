#include "linegraphicsview.h"
#include <QWheelEvent>
#include <math.h>
#include <QElapsedTimer>

#include <QDebug>

LineGraphicsView::LineGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    pixelSize = QSize(1, 1);
    algorithm = Bresenham;
    QRect rect(-5000, -5000, 10000, 10000);

    scene = new QGraphicsScene(this);
    setScene(scene);
    setSceneRect(rect);
    scale(15, 15);
    setDragMode(ScrollHandDrag);
    drawGrid(rect);
}

void LineGraphicsView::setRasterAlgorithm(RasterAlgorithm _in_algorithm)
{
    algorithm = _in_algorithm;
}

void LineGraphicsView::drawLine(QPoint _in_begin, QPoint _in_end)
{
    switch (algorithm) {
    case Stepwise:
        drawStepwiseLine(_in_begin, _in_end);
        break;
    case DDA:
        drawDDALine(_in_begin, _in_end);
        break;
    case Bresenham:
        drawBresenhamLine(_in_begin, _in_end);
        break;
    }
}

void LineGraphicsView::drawStepwiseLine(QPoint _in_begin, QPoint _in_end)
{
    int deltaX = _in_end.x() - _in_begin.x();
    int deltaY = _in_end.y() - _in_begin.y();

    if (deltaX != 0) {
        double k = (double)(deltaY) / (deltaX);
        double b = _in_begin.y() - _in_begin.x() * k;

        if (fabs(k) > 1) {
            int begin = deltaY > 0 ? _in_begin.y() : _in_end.y();
            int end = deltaY > 0 ? _in_end.y() : _in_begin.y();

            for (int y = begin + 1; y < end; y++) {
                double x = (y - b) / k;
                int int_x = x > 0 ? (int)(x + 0.5) : (int)(x - 0.5);
                setPixel(QPoint(int_x, y), Qt::blue);
            }
        } else {
            int begin = deltaX > 0 ? _in_begin.x() : _in_end.x();
            int end = deltaX > 0 ? _in_end.x() : _in_begin.x();

            for (int x = begin + 1; x < end; x++) {
                double y = x * k + b;
                int int_y = y > 0 ? (int)(y + 0.5) : (int)(y - 0.5);
                setPixel(QPoint(x, int_y), Qt::blue);
            }
        }
    } else {
        int begin = deltaY > 0 ? _in_begin.y() : _in_end.y();
        int end = deltaY > 0 ? _in_end.y() : _in_begin.y();

        for (int y = begin + 1; y < end; y++)
            setPixel(QPoint(_in_begin.x(), y), Qt::blue);
    }
}

void LineGraphicsView::drawDDALine(QPoint _in_begin, QPoint _in_end)
{
    int deltaX = _in_end.x() - _in_begin.x();
    int deltaY = _in_end.y() - _in_begin.y();
    int length = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

    double dx = (double)deltaX / length;
    double dy = (double)deltaY / length;

    double x = _in_begin.x();
    double y = _in_begin.y();

    for (int i = 0; i < length - 1; i++) {
        x += dx;
        y += dy;
        int a = x > 0 ? (int)(x + 0.5) : (int)(x - 0.5);
        int b = y > 0 ? (int)(y + 0.5) : (int)(y - 0.5);
        setPixel(QPoint(a, b), Qt::red);
    }
}

QPoint LineGraphicsView::switchToZeroOctant(int _in_octant, QPoint _in_p)
{
    switch (_in_octant) {
    case 0:
        return QPoint(_in_p.x(), _in_p.y());
    case 1:
        return QPoint(_in_p.y(), _in_p.x());
    case 2:
        return QPoint(_in_p.y(), -_in_p.x());
    case 3:
        return QPoint(-_in_p.x(), _in_p.y());
    case 4:
        return QPoint(-_in_p.x(), -_in_p.y());
    case 5:
        return QPoint(-_in_p.y(), -_in_p.x());
    case 6:
        return QPoint(-_in_p.y(), _in_p.x());
    case 7:
        return QPoint(_in_p.x(), -_in_p.y());
    default:
        return _in_p;
    }
}

void LineGraphicsView::drawBresenhamLine(QPoint _in_begin, QPoint _in_end)
{
    int octant;
    int dx = _in_end.x() - _in_begin.x();
    int dy = _in_end.y() - _in_begin.y();

    if (abs(dy) > abs(dx)) {
        if (dx > 0)
            octant = dy > 0 ? 1 : 6;
        else
            octant = dy > 0 ? 2 : 5;
    } else {
        if (dx > 0)
            octant = dy > 0 ? 0 : 7;
        else
            octant = dy > 0 ? 3 : 4;
    }

    QPoint endT = switchToZeroOctant(octant, _in_end - _in_begin);

    if (octant == 2)
        octant = 6;
    else if (octant == 6)
        octant = 2;

    dx = endT.x();
    dy = endT.y();

    int D = 2 * dy - dx;

    for (int x = 1, y = 0; x < endT.x(); x++) {
        if (D > 0) {
            y++;
            D += 2 * dy - 2 * dx;
        } else
            D += 2 * dy;

        QPoint p = switchToZeroOctant(octant, QPoint(x, y)) + _in_begin;
        setPixel(p, Qt::green);
    }
}

void LineGraphicsView::setPixel(QPoint _in_pixel, Qt::GlobalColor _in_colour)
{
    scene->addRect(QRect(_in_pixel, pixelSize), Qt::NoPen, _in_colour);
}

void LineGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(AnchorUnderMouse);
    double scaleFactor = 1.15;
    if (event->delta() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void LineGraphicsView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void LineGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    viewport()->setCursor(Qt::ClosedHandCursor);
    if (event->button() == Qt::RightButton) {
        QPointF pt = mapToScene(event->pos());
        int x = pt.x() > 0 ? (int)pt.x() : (int)pt.x() - 1;
        int y = pt.y() > 0 ? (int)pt.y() : (int)pt.y() - 1;

        setPixel(QPoint(x, y), Qt::black);

        if (firstPoint == QPoint())
            firstPoint = QPoint(x, y);
        else {
            secondPoint = QPoint(x, y);

            QElapsedTimer timer;
            timer.start();
            drawLine(firstPoint, secondPoint);
            emit timeElapsed(timer.nsecsElapsed());

            firstPoint = secondPoint = QPoint();
        }
    }
}

void LineGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void LineGraphicsView::drawGrid(QRect rect)
{
    QPen grayPen(Qt::lightGray);
    grayPen.setStyle(Qt::DotLine);
    grayPen.setWidth(0);

    QPen blackPen(Qt::black);
    blackPen.setStyle(Qt::SolidLine);
    blackPen.setWidth(0);

    for (int i = rect.left(); i < rect.right(); i++)
        scene->addLine(i, rect.top(), i, rect.bottom(), grayPen);
    for (int i = rect.top(); i < rect.bottom(); i++)
        scene->addLine(rect.left(), i, rect.right(), i, grayPen);
    scene->addLine(0, rect.top(), 0, rect.bottom(), blackPen);
    scene->addLine(rect.left(), 0, rect.right(), 0, blackPen);
}
