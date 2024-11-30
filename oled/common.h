#ifndef COMMON_H
#define COMMON_H

//#define DEBUG

#ifdef DEBUG
#define SERIAL_PRINT(...) Serial.print(__VA_ARGS__)
#define SERIAL_PRINTLN(...) Serial.println(__VA_ARGS__)
#define SERIAL_WRITE(s) Serial.write(s)
#else /* DEBUG */
#define SERIAL_PRINT(...)
#define SERIAL_PRINTLN(...)
#define SERIAL_WRITE(s)
#endif /* DEBUG */

#endif
