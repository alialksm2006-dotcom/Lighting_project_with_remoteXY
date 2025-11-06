// fix code 

#define REMOTEXY_MODE__WIFI
#include "secret.h"
#include "clsLight.h"
#include <WiFi.h>
#include <Bounce2.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 87 bytes
  { 255,4,0,1,0,147,0,19,0,0,0,216,186,216,177,217,129,216,169,32,
  216,167,217,132,217,134,217,136,217,133,32,0,163,1,106,200,1,1,5,0,
  10,60,19,31,31,48,249,26,31,32,78,69,79,78,95,79,78,0,31,78,
  69,79,78,95,79,70,70,0,10,62,82,31,31,48,249,26,31,76,69,68,
  95,79,78,0,31,76,69,68,95,79,70,70,0,10,8,18,31,31,48,249,
  26,31,78,69,79,78,95,79,78,0,31,78,69,79,78,95,79,70,70,0,
  10,10,81,31,31,48,249,26,31,76,69,68,95,79,78,0,31,76,69,68,
  95,79,70,70,0,70,44,149,18,18,17,26,37,0  };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t pushSwitch_01; // =1 if state is ON, else =0, from 0 to 1
  uint8_t pushSwitch_02; // =1 if state is ON, else =0, from 0 to 1
  uint8_t pushSwitch_03; // =1 if state is ON, else =0, from 0 to 1
  uint8_t pushSwitch_04; // =1 if state is ON, else =0, from 0 to 1

    uint8_t led_01; // from 0 to 1
    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define LedPin_01 15
#define LedPin_02 4
#define LedPin_03 2 
#define LedPin_04 16

#define Switch1 17
#define Switch2 18
#define Switch3 5
#define Switch4 19


#define Buzzer 21
#define Indicate_Pin 22

clsLight Light_1 (LedPin_01,Switch1,&RemoteXY.pushSwitch_01);
clsLight Light_2 (LedPin_02,Switch2,&RemoteXY.pushSwitch_02);
clsLight Light_3 (LedPin_03,Switch3,&RemoteXY.pushSwitch_03);
clsLight Light_4 (LedPin_04,Switch4,&RemoteXY.pushSwitch_04);


WiFiClientSecure client ;
uint8_t Last_Elec_State = 0;

void onWiFiEvent(WiFiEvent_t event)
{
  switch(event) 
  {
    case SYSTEM_EVENT_STA_GOT_IP:
    ArduinoOTA.begin();
     TelnetStream.begin();
      break;
    default:
      break;
  }
}
void setup() 
{
  WiFi.onEvent(onWiFiEvent);
  Serial.begin(115200);
  RemoteXY_Init ();
  pinMode(Buzzer,OUTPUT);
  if(WiFi.status()==WL_CONNECTED )
  {ArduinoOTA.begin();
    TelnetStream.begin();
  }
  // WiFi.softAP(ssid_AP,password_AP);
  // delay(10000);
  // TelnetStream.println("IP AP is :");
  // TelnetStream.println(WiFi.softAPIP());
  pinMode(Indicate_Pin,INPUT_PULLUP);
  Last_Elec_State = !digitalRead(Indicate_Pin);
  client.setInsecure();
}



void PrintTemp()
{
  
  float temperature = temperatureRead(); 
  TelnetStream.print("ESP32 Internal Temperature: ");
  TelnetStream.print(temperature);
  TelnetStream.println(" °C");
}


const char* botToken = BOT_TOKEN;
String chatID = CHAT_ID;
UniversalTelegramBot bot (botToken,client);
void Send_Elec_Message (uint8_t CurrentState , uint8_t &LastState)
{
  if(CurrentState==1&&LastState==0)
  { 
     LastState=CurrentState;
     bot.sendMessage(chatID,"Electricity is on ");
     
  }

  if(CurrentState==0&&LastState==1)
  { 
     LastState=CurrentState;
     bot.sendMessage(chatID,"Electricity is off ");
     
  }
  
}
void loop() 
{ 
  RemoteXY_Handler ();
  ArduinoOTA.handle();
  Light_1.UpdateLight();
  Light_2.UpdateLight();
  Light_3.UpdateLight();
  Light_4.UpdateLight();
  
  RemoteXY.led_01=!digitalRead(Indicate_Pin);
// RemoteXY.led_01=1;


Send_Elec_Message(RemoteXY.led_01,Last_Elec_State);

if(temperatureRead()>63)
digitalWrite(Buzzer,HIGH);
else digitalWrite(Buzzer,LOW);

// PrintTemp();
delay(20);


}

