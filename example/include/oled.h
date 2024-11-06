#pragma once
#include <U8g2lib.h>

class OLED {
public:
    OLED();
    void setup();
    void displayWarning(const char* message);
    void updateDisplay(const char* data);

private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
};
