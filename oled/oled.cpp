#include "common.h"
#include "graphics.h"
#include "oled.h"
#include <SoftwareSerial.h>

#define RES_ 2
#define DC_ 4
#define CS_ 3
static uint8_t oled_SDA;
static uint8_t oled_CLK;

void oled_start_commands()
{
    digitalWrite(CS_, LOW);
    digitalWrite(DC_, LOW);
}

void oled_start_data()
{
    digitalWrite(CS_, LOW);
    digitalWrite(DC_, HIGH);
}

void oled_disable() { digitalWrite(CS_, HIGH); }

#define displayOff() oled_sendByte(0xAE)
#define setClockDivide(value)                                                                      \
    oled_sendByte(0xD5);                                                                           \
    oled_sendByte(value)
#define setMultiplex(value)                                                                        \
    oled_sendByte(0xA8);                                                                           \
    oled_sendByte(value)
#define setDisplayOffset(value)                                                                    \
    oled_sendByte(0xD3);                                                                           \
    oled_sendByte(value)
#define setStartLine(value) oled_sendByte(0x40 | (value & 0x3F))
#define setChargePump(value)                                                                       \
    oled_sendByte(0x8D);                                                                           \
    oled_sendByte(value)
#define setMemoryMode(value)                                                                       \
    oled_sendByte(0x20);                                                                           \
    oled_sendByte(value)
#define setSegmentRemap(value) oled_sendByte(0xA0 | value)
#define setComScanDirection(value) oled_sendByte(0xC0 | value)
#define setComPins(value)                                                                          \
    oled_sendByte(0xDA);                                                                           \
    oled_sendByte(value)
#define setContrast(value)                                                                         \
    oled_sendByte(0x81);                                                                           \
    oled_sendByte(value)
#define setPrecharge(value)                                                                        \
    oled_sendByte(0xD9);                                                                           \
    oled_sendByte(value)
#define setVcomhDeselectLevel(value)                                                               \
    oled_sendByte(0xDB);                                                                           \
    oled_sendByte(value)
#define entireDisplayOn() oled_sendByte(0xA4)
#define setNormalMode() oled_sendByte(0xA6)
#define setInverseMode() oled_sendByte(0xA7)
#define deactivateScroll() oled_sendByte(0x2E)
#define displayOn() oled_sendByte(0xAF)
#define setPageAddress(start, stop)                                                                \
    oled_sendByte(0x22);                                                                           \
    oled_sendByte(start);                                                                          \
    oled_sendByte(stop)
#define setColumnAddress(value)                                                                    \
    oled_sendByte(0x21);                                                                           \
    oled_sendByte(value)
#define clockPulse()                                                                               \
    digitalWrite(oled_CLK, HIGH);                                                                  \
    digitalWrite(oled_CLK, LOW)

void reset()
{
    digitalWrite(RES_, HIGH);
    delay(1);
    digitalWrite(RES_, LOW);
    delay(10);
    digitalWrite(RES_, HIGH);
}

void oled_sendByte(uint8_t data)
{
    for (uint8_t mask = 0x80; mask; mask >>= 1)
    {
        digitalWrite(oled_SDA, (data & mask));
        clockPulse();
    }
#ifdef DEBUG
    SERIAL_PRINT("0x");
    SERIAL_PRINT(data, HEX);
    SERIAL_WRITE(' ');
#endif
}

void oled_home()
{
    oled_start_commands();
    setPageAddress(0x00, 0xFF);
    setColumnAddress(0x00);
    setStartLine(0x3F);
}

void oled_clearDisplay()
{
    graphics_clear_buffer();
    oled_displayBitmap();
}

void oled_init(uint8_t sda, uint8_t clk)
{
    oled_SDA = sda;
    oled_CLK = clk;
    uint8_t multiplex, com_pins, contrast;
    if (DISPLAY_ROWS == 64)
    {
        multiplex = 0x3F;
        com_pins  = 0x12;
        contrast  = 0xCF;
    }
    else
    {
        multiplex = 0x1F;
        com_pins  = 0x02;
        contrast  = 0x8F;
    }
    pinMode(RES_, OUTPUT);
    pinMode(oled_CLK, OUTPUT);
    pinMode(oled_SDA, OUTPUT);
    pinMode(DC_, OUTPUT);
    pinMode(CS_, OUTPUT);
    digitalWrite(RES_, HIGH);
    digitalWrite(oled_CLK, LOW);
    digitalWrite(CS_, LOW);
    oled_start_commands();
    reset();
    displayOff();
    setClockDivide(0x80);
    setMultiplex(multiplex);
    setDisplayOffset(0x00);
    setStartLine(0x00);
    setChargePump(0x14);
    setMemoryMode(0x00);
    setSegmentRemap(0x01);
    setComScanDirection(0x08);
    setComPins(com_pins);
    setContrast(contrast);
    setPrecharge(0xF1);
    setVcomhDeselectLevel(0x40);
    entireDisplayOn();
    setNormalMode();
    deactivateScroll();
    displayOn();
    oled_clearDisplay();
}

void oled_displayBitmap()
{
    oled_home();
    oled_start_data();
    uint8_t(*bitmap)[DISPLAY_COLS][DISPLAY_PAGES] = graphics_get_bitmap();
    for (uint8_t page = 0; page < DISPLAY_PAGES; page++)
    {
        for (uint8_t col = 0; col < DISPLAY_COLS; col++)
        {
            oled_sendByte((*bitmap)[col][page]);
            SERIAL_PRINT((*bitmap)[col][page], HEX);
            SERIAL_PRINT(' ');
        }
        SERIAL_PRINTLN();
    }
    SERIAL_PRINTLN();
}

void oled_addPixel(uint8_t x, uint8_t y) { graphics_add_pixel(x, y); }

void oled_addInlineSymbol(uint8_t cursor_position, uint8_t page, char c)
{
    if (c == 'a')
    {
        uint8_t symbol[8] = {0x05, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x07};
        graphics_add_inline_symbol(cursor_position, page, symbol);
    }
}
