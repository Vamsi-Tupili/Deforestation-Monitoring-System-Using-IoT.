#include "UbidotsESPMQTT.h"
#include<PubSubClient.h>
/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBUS-jpha0xcVRxL3zk2PwFlWc2LfR****" // Your Ubidots TOKEN
#define WIFINAME "Vamsi" //Your SSID
#define WIFIPASS "0123456789" // Your Wifi Pass

Ubidots client(TOKEN);

String gas, fire;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
}

void loop() {
  if(Serial.available())
  {
    String data=Serial.readStringUntil('\n');
    Serial.println(data);
    //int space=data.indexOf(2)
     gas = data.substring(0,3);
     Serial.println(gas);
      fire = data.substring(3);
      Serial.println(fire);
      float Gas_level =gas.toFloat();
      float fire_level =fire.toFloat();
    if(!client.connected()){
      client.reconnect();
      client.add("Gas",Gas_level);
      client.add("Fire",fire_level);
      client.ubidotsPublish("sensors");
  }
  client.loop();
  }

}




/*void loop()
{
  int a;
  a=digitalRead(D1);
  if(a == LOW)
  {
    digitalWrite(D6,LOW);
  }
  else
  {
    digitalWrite(D6,HIGH);
  }
  if(!client.connected()){
      client.reconnect();
      client.add("Fire",a);
      client.ubidotsPublish("Flame_sensor");
  }
  client.loop();
delay(500);
}
*/
