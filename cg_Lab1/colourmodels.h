#ifndef COLOURMODELS
#define COLOURMODELS

struct RGB
{
    int r;
    int g;
    int b;
};

struct HSV
{
    double h;
    double s;
    double v;
};

struct CMYK
{
    int c;
    int m;
    int y;
    int k;
};

struct LUV
{
    int l;
    int u;
    int v;
};

#endif // COLOURMODELS
