#include "clsLight.h"

clsLight::clsLight(uint8_t LedPin,uint8_t SwitchPin,uint8_t *RemoteSwitch)
{
   _LedPin= LedPin;
_SwitchPin=SwitchPin;
_RemoteXYSwitch=RemoteSwitch;
pinMode(_LedPin,OUTPUT);
pinMode(_SwitchPin,INPUT_PULLUP);
bounce.attach(_SwitchPin);
bounce.interval(40); 
}


void clsLight::UpdateLight()
{
bounce.update();
  if (_SwitchState != (!bounce.read())) {
    _LedState = !_LedState;
    _SwitchState = !_SwitchState;
    *_RemoteXYSwitch= _LedState;
  }

  if (*_RemoteXYSwitch) _LedState = 1;
  else _LedState = 0;
  // --- تحديث الليد ---
  digitalWrite(_LedPin, _LedState ? LOW : HIGH);
}

