#ifndef SERIAL_H
#define SERIAL_H

#include "lighting.h"
#include "hvac.h"
#include "util.h"

#include <Arduino.h>
/**
 * @brief code that will be inside loop()
 * 
 */
void handleSerial(void);

/**
 * @brief sends comand to Serial for controlling (LIGHT ON, LIGHT OFF, LIGHT AUTO, LIGHT SECURE, HVAC ON, HVAC OFF, COOLING, HEATING)
 * 
 */
void sendCommandToSerial(void);

/**
 * @brief prints 'M' into Serial, so that script knows it should send email for motion detection
 * 
 */
void sendMail(void);

/**
 * @brief prints 'D' followed by all data that are measured in Serial
 * 
 */
void sendStatus(void);

#endif // SERIAL_H
