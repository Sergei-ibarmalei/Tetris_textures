#pragma once

#include "videoconst.h"

constexpr int ROOMWIDTH_PIXELS{ 10 };
constexpr int ROOMHEIGHT_PIXELS{ 20 };
constexpr int ROOMPIXELSIDE{ 46 };

constexpr int ROOMWIDTH{ ROOMWIDTH_PIXELS * ROOMPIXELSIDE };
constexpr int ROOMHEIGHT{ ROOMHEIGHT_PIXELS * ROOMPIXELSIDE };

constexpr int ROOMLEFTSIDE_X{ SCREEN_MIDDLE_X - (ROOMWIDTH / 2) };
constexpr int ROOMLEFTSIDE_Y{ 300 };


