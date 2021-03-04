//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_COLOR_H
#define DR_COLOR_H

#include <string>


//####################################################################################
//##    Easy Colors
//############################
enum Colors {
    DROP_COLOR_TRANSPARENT =    0x00000000,

    DROP_COLOR_WHITE =          0xFFFFFFFF,
    DROP_COLOR_BLACK =          0xFF000000,
    DROP_COLOR_GRAY =           0xFF808080,
    DROP_COLOR_GREY =           0xFF808080,

    DROP_COLOR_RED =            0xFFFF0000,
    DROP_COLOR_GREEN =          0xFF00FF00,
    DROP_COLOR_BLUE =           0xFF0000FF,

    DROP_COLOR_MAGENTA =        0xFFFF00FF,
    DROP_COLOR_CYAN =           0xFF00FFFF,
    DROP_COLOR_YELLOW =         0xFFFFFF00,

    DROP_ORANGE =               0xFFFFA500,
    DROP_COLOR_PURPLE =         0xFF800080,
    DROP_COLOR_BROWN =          0xFF5D4037,
};


//####################################################################################
//##    Struct for Hue, Saturation, Value
//############################
struct DrHsv {
    double hue =            0;      // 0.0 to 360.0
    double saturation =     0;      // 0.0 to   1.0
    double value =          0;      // 0.0 to   1.0
};

struct DrRgb {
    int red =               0;      // 0 to 255
    int green =             0;      // 0 to 255
    int blue =              0;      // 0 to 255
};


//####################################################################################
//##    DrColor
//##        Useful rgba Color Class
//############################
class DrColor
{
private:
    unsigned char r = 0;                // Red      Range: 0 to 255
    unsigned char g = 0;                // Green    Range: 0 to 255
    unsigned char b = 0;                // Blue     Range: 0 to 255
    unsigned char a = 255;              // Alpha    Range: 0 to 255

public:
    // Constructors
    DrColor();
    DrColor(const unsigned char &r_, const unsigned char &g_, const unsigned char &b_, const unsigned char &a_ = static_cast<unsigned char>(255));
    DrColor(int r_, int g_, int b_, int a_ = 255);
    DrColor(float r_, float g_, float b_, float a_ = 1.0f);
    DrColor(double r_, double g_, double b_, double a_ = 1.0);
    DrColor(unsigned int ui);

    // Conversions
    unsigned int    rgb();
    unsigned int    rgba();
    std::string     name();

    // Color Editing
    DrColor         redistributeRgb(double r, double g, double b);
    DrColor         darker(int percent =  200);
    DrColor         lighter(int percent = 150);

    // Getters / Setters
    const unsigned char&    red() const     { return r; }
    const unsigned char&    green() const   { return g; }
    const unsigned char&    blue() const    { return b; }
    const unsigned char&    alpha() const   { return a; }
    double                  redF() const    { return static_cast<double>(r) / 255.0; }
    double                  greenF() const  { return static_cast<double>(g) / 255.0; }
    double                  blueF() const   { return static_cast<double>(b) / 255.0; }
    double                  alphaF() const  { return static_cast<double>(a) / 255.0; }
    float                   redf() const    { return static_cast<float>(r) / 255.0; }
    float                   greenf() const  { return static_cast<float>(g) / 255.0; }
    float                   bluef() const   { return static_cast<float>(b) / 255.0; }
    float                   alphaf() const  { return static_cast<float>(a) / 255.0; }

    void        setRed(int red);
    void        setRedF(double red);
    void        setGreen(int green);
    void        setGreenF(double green);
    void        setBlue(int blue);
    void        setBlueF(double blue);
    void        setAlpha(int alpha);
    void        setAlphaF(double alpha);

    void        setRgbF(double r, double g, double b, double a);


    // Operator Overloads
    DrColor&    operator=   (const DrColor &other);
    DrColor     operator+   (const DrColor &other) const;
    DrColor     operator-   (const DrColor &other) const;
    DrColor     operator*   (int k) const;
    DrColor     operator/   (int k) const;
    bool        operator==  (const DrColor &other) const;
    bool        operator!=  (const DrColor &other) const;


    // Hsv 
    DrHsv       getHsv();
    void        setFromHsv(DrHsv hsv);

};


#endif // DR_COLOR_H








