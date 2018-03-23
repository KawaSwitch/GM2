#pragma once

// “§‹“Š‰e’è”
struct PersParam
{
    static const double fovy; // y•ûŒü‚Ì‹–ìŠp
    static const double zNear;
    static const double zFar;
};

// “Š‰e–@
enum ProjectType
{
    Perspective,    // “§‹“Š‰e
    Ortho,          // •½s“Š‰e
};

extern ProjectType mainProjType; // ƒƒCƒ“ƒrƒ…[‚Ì“Š‰e–@
extern ProjectType axisProjType; // ²‚Ì“Š‰e–@