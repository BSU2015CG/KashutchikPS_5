#ifndef COLOURMODEL_H
#define COLOURMODEL_H

#include <QObject>
#include "colourmodels.h"

class ColourModel : public QObject
{
    Q_OBJECT
public:
    explicit ColourModel(QObject *parent = 0);

public:
    ColourModel();
    void setColourInRGB(int _in_r, int _in_g, int _in_b);
    void setColourInHSV(double _in_h, double _in_s, double _in_v);
    void setColourInCMYK(int _in_c, int _in_m, int _in_y);
    void setColourInLUV(int _in_l, int _in_u, int _in_v);
    RGB getRGB();
    HSV getHSV();
    CMYK getCMYK();
    LUV getLUV();

protected:
    RGB rgb;

signals:
    void colourChanged(QString _in_model);
};

#endif // COLOURMODEL_H
