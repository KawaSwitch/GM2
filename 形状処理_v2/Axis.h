#pragma once

#include "GV.h"
#include "Object.h"

class Axis
{
protected:
    double _length; // “·‘Ì•”•ª‚Ì’·‚³

public:
    Axis(double len) { _length = len; }

    // –îˆó•”•ª‚ðŠÜ‚ß‚½’·‚³‚ð•Ô‚·
    double GetLength() { return _length + _length / 4; }
};

// Cggl•—‚ÌŽ²
class NormalAxis : public Axis
{
private:
    void DrawInternal();

public:
    NormalAxis(double len) : Axis(len) { }
    void Draw();
};