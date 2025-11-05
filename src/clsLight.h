#include <Arduino.h>
#include <Bounce2.h>
#ifndef CLSLIGHT_H
#define CLSLIGHT_H
class clsLight
{
private : 
uint8_t _LedPin;
uint8_t _SwitchPin;
bool _SwitchState=0;
bool _LedState =0;
Bounce bounce ;
uint8_t *_RemoteXYSwitch;


public:  
clsLight(uint8_t LedPin,uint8_t SwitchPin,uint8_t *RemoteSwitch);

void UpdateLight();

};
#endif