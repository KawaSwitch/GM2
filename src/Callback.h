#pragma once

void Display();
void Resize(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Wheel(int wheel_num, int direction, int x, int y);
