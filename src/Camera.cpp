#include "Camera.h"

// “§‹“Š‰e’è”
const double PersParam::fovy = 30; // y•ûŒü‚Ì‹–ìŠp
const double PersParam::zNear = 0.1;
const double PersParam::zFar = 10000; // “K“–

// “Š‰e–@
ProjectType mainProjType = ProjectType::Perspective;
ProjectType axisProjType = ProjectType::Ortho;