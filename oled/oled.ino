#include "oled.h"

void setup()
{
    oled_init();
#ifdef DEBUG
    Serial.begin(115200);
    delay(2000);
#endif

    oled_disable();
}

void loop()
{
    for (byte row = 0; row < 3; row++)
    {
        oled_addInlineSymbol(0, row, 'A');
    }
    oled_addInlineSymbol(1, 0, 'R');
    oled_displayBitmap();
    delay(1000);
    oled_clearDisplay();
    oled_displayBitmap();
    delay(1000);
}
