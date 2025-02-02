#include <VirtualWire.h>
#define fire_pin 5

const int transmit_pin = 11;
char str1[3];
char node[3] = "02"; // node number. change this node number for
char trnsmit_str[4];// different node
void setup()
{
    vw_set_tx_pin(transmit_pin);    
    vw_set_ptt_inverted(true);     
    vw_setup(2000);             // Bits per sec
    Serial.begin(9600);    
//    pinMode(led_pin, OUTPUT);
}
void loop()
{
  int fire,fire_level;
  fire=analogRead(fire_pin);    
  fire_level=map(fire,0,1023,0,99);   // map them in the range between 0 to 99% 
  Serial.println("Node 2");   // serially print all values
  Serial.print("FIRE level: ");    // for debugging
  Serial.print(fire_level);
  Serial.println(" %");     // Flash a light to show transmitting
  itoa(fire_level,str1,10);      // convert all 2 digit values into 
  strcpy(trnsmit_str,node);   // one string
  strcat(trnsmit_str,str1);
  Serial.print("transmitted string: ");
  Serial.println(trnsmit_str);
  vw_send((uint8_t *)trnsmit_str, 4);
  vw_wait_tx();         // Wait until the whole message is gone
  delay(500);  
}
