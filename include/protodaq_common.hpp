#pragma once

#include <spdlog/spdlog.h>

enum class DAQ_Protocol {
    INVALID = -1,
    MAVLINK = 0,
    UDP = 1,
};

enum class DAQ_Mode {
    INVALID=-1,
    REAL_TIME=0,
    DATA_PLAYBACK = 1,
    SIMULATION = 2,
};
