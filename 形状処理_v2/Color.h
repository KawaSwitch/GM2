#pragma once
#include <gl/freeglut.h>

// êFÇ‹Ç∆Çﬂ
struct Color
{
public:
    static GLdouble red[];
    static GLdouble green[];
    static GLdouble blue[];
    static GLdouble orange[];
    static GLdouble pink[];
    static GLdouble light_green[];
    static GLdouble light_blue[];

    static GLdouble red_alpha[];
    static GLdouble orange_alpha[];
    static GLdouble pink_alpha[];
    static GLdouble green_alpha[];
    static GLdouble blue_alpha[];

    static GLdouble black[];
    static GLdouble black_alpha[];

    static void GetRandomColor(GLdouble* color)
    {
        //srand((unsigned)time(NULL));

        for (int i = 0; i < 3; i++)
            color[i] = (double)rand() / 32767.0;

        color[3] = 0.5; // ÉøílÇÕå≈íË
    }

public:
    Color() = delete;
};