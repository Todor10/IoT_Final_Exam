#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

void initSerial(void);
void handleSerial(void);

void sendCommandToSerial(void);

void sendMail(void);
void sendStatus(void);

#endif // SERIAL_H
