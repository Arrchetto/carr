#include "oled.h"
#include "config.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE,/*clock=*/ 18, /*data=*/ 17);

OLED::OLED() : u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /*clock=*/ 18, /*data=*/ 17) {}

void OLED::setup() {
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);  
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.print("12,3456");
    u8g2.sendBuffer();
}

void OLED::displayWarning(const char* message) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setCursor(0, 15);
    u8g2.print(message);
    u8g2.sendBuffer();
}

void OLED::updateDisplay(const char* data) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setCursor(0, 15);
    u8g2.print(data);
    u8g2.sendBuffer();
}