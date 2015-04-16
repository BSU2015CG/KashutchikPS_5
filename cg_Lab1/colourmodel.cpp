#include "colourmodel.h"
#include "math.h"

ColourModel::ColourModel(QObject *parent) : QObject(parent) { rgb.r = rgb.g = rgb.b = 255; }

void ColourModel::setColourInRGB(int _in_r, int _in_g, int _in_b)
{
    if (_in_r >= 0 && _in_r <= 255 && _in_g >= 0 && _in_g <= 255 && _in_b >= 0 && _in_b <= 255) {
        rgb.r = _in_r;
        rgb.g = _in_g;
        rgb.b = _in_b;
        emit colourChanged("RGB");
    }
}

void ColourModel::setColourInHSV(double _in_h, double _in_s, double _in_v)
{
    if (_in_h >= 0 && _in_h < 360 && _in_s >= 0 && _in_s <= 1 && _in_v >= 0 && _in_v <= 1) {
        _in_v *= 255;

        int i = _in_h / 60;
        int m = (1 - _in_s) * _in_v;
        int a = (_in_v - m) * fmod(_in_h, 60) / 60;
        int k = m + a;
        int n = _in_v - a;

        switch (i) {
        case 0:
            rgb.r = _in_v;
            rgb.g = k;
            rgb.b = m;
            break;
        case 1:
            rgb.r = n;
            rgb.g = _in_v;
            rgb.b = m;
            break;
        case 2:
            rgb.r = m;
            rgb.g = _in_v;
            rgb.b = k;
            break;
        case 3:
            rgb.r = m;
            rgb.g = n;
            rgb.b = _in_v;
            break;
        case 4:
            rgb.r = k;
            rgb.g = m;
            rgb.b = _in_v;
            break;
        case 5:
            rgb.r = _in_v;
            rgb.g = m;
            rgb.b = n;
            break;
        }
        emit colourChanged("HSV");
    }
}

void ColourModel::setColourInCMYK(int _in_c, int _in_m, int _in_y)
{
    if (_in_c >= 0 && _in_c <= 100 && _in_m >= 0 && _in_m <= 100 && _in_y >= 0 && _in_y <= 100) {

        rgb.r = round(2.55 * (100 - _in_c));
        rgb.g = round(2.55 * (100 - _in_m));
        rgb.b = round(2.55 * (100 - _in_y));

        emit colourChanged("CMYK");
    }
}

void ColourModel::setColourInLUV(int _in_l, int _in_u, int _in_v)
{
    double x = 9. * _in_l * _in_u / (4. * _in_v);
    double y = _in_l;
    double z = (-3. * _in_l * _in_u - 20. * _in_l * _in_v + 1200. * _in_l) / (4. * _in_v);

    x /= 100.;
    y /= 100.;
    z /= 100.;

    double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
    double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
    double b = x * 0.0557 + y * -0.2040 + z * 1.0570;

    if (r > 0.0031308)
        r = 1.055 * pow(r, 1 / 2.4) - 0.055;
    else
        r = 12.92 * r;
    if (g > 0.0031308)
        g = 1.055 * pow(g, 1 / 2.4) - 0.055;
    else
        g = 12.92 * g;
    if (b > 0.0031308)
        b = 1.055 * pow(b, 1 / 2.4) - 0.055;
    else
        b = 12.92 * b;

    rgb.r = r < 0 ? 0 : r > 1 ? 255 : r * 255;
    rgb.g = g < 0 ? 0 : g > 1 ? 255 : g * 255;
    rgb.b = b < 0 ? 0 : b > 1 ? 255 : b * 255;
    emit colourChanged("LUV");

    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1)
        emit colourChanged("ERROR");
}

RGB ColourModel::getRGB() { return rgb; }

HSV ColourModel::getHSV()
{
    HSV _out_hsv;

    double r = rgb.r / 255.;
    double g = rgb.g / 255.;
    double b = rgb.b / 255.;

    double max;
    max = r > g ? r : g;
    max = max > b ? max : b;

    double min;
    min = r < g ? r : g;
    min = min < b ? min : b;

    if (max == min)
        _out_hsv.h = 0;
    else if (max == r && g >= b)
        _out_hsv.h = 60 * (g - b) / (max - min);
    else if (max == r && g < b)
        _out_hsv.h = 60 * (g - b) / (max - min) + 360;
    else if (max == g)
        _out_hsv.h = 60 * (b - r) / (max - min) + 120;
    else if (max == b)
        _out_hsv.h = 60 * (r - g) / (max - min) + 240;

    _out_hsv.s = max == 0 ? 0 : 1 - min / max;
    _out_hsv.v = max;

    return _out_hsv;
}

CMYK ColourModel::getCMYK()
{
    CMYK _out_cmyk;

    double c, m, y, k;

    c = 1 - rgb.r / 255.;
    m = 1 - rgb.g / 255.;
    y = 1 - rgb.b / 255.;

    int max;
    max = rgb.r > rgb.g ? rgb.r : rgb.g;
    max = max > rgb.b ? max : rgb.b;
    k = 1 - max / 255.;

    _out_cmyk.c = c * 100;
    _out_cmyk.m = m * 100;
    _out_cmyk.y = y * 100;
    _out_cmyk.k = k * 100;

    return _out_cmyk;
}

LUV ColourModel::getLUV()
{
    LUV _out_luv;

    double r = rgb.r / 255.;
    double g = rgb.g / 255.;
    double b = rgb.b / 255.;

    if (r > 0.04045)
        r = pow((r + 0.055) / 1.055, 2.4);
    else
        r = r / 12.92;
    if (g > 0.04045)
        g = pow((g + 0.055) / 1.055, 2.4);
    else
        g = g / 12.92;
    if (b > 0.04045)
        b = pow((b + 0.055) / 1.055, 2.4);
    else
        b = b / 12.92;

    r *= 100;
    g *= 100;
    b *= 100;

    double x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    double y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    double z = r * 0.0193 + g * 0.1192 + b * 0.9505;

    _out_luv.l = y;
    _out_luv.u = 400 * x / (x + 15 * y + 3 * z);
    _out_luv.v = 900 * y / (x + 15 * y + 3 * z);

    return _out_luv;
}
