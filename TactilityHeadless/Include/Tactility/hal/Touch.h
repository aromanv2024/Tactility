#pragma once

#include "Device.h"

#include <lvgl.h>

namespace tt::hal {

class Display;

class Touch : public Device {

public:

    Type getType() const override { return Type::SdCard; }

    virtual bool start(lv_display_t* display) = 0;
    virtual bool stop() = 0;

    virtual lv_indev_t* _Nullable getLvglIndev() = 0;
};

}
