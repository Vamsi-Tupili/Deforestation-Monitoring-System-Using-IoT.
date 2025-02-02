#include <VirtualWire.h>
#define MQ2_pin A0

const int transmit_pin = 11;
char str1[3];
char node[3] = "01"; // node number. change this node number for
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
  int gas,gas_level;
  gas=analogRead(MQ2_pin);    
  gas_level=map(gas,0,1023,0,99);   // map them in the range between 0 to 99% 
  Serial.println("Node 1");   // serially print all values
  Serial.print("GAS level: ");    // for debugging
  Serial.print(gas_level);
  Serial.println(" %");     // Flash a light to show transmitting
  itoa(gas_level,str1,10);      // convert all 2 digit values into 
  strcpy(trnsmit_str,node);   // one string
  strcat(trnsmit_str,str1);
  Serial.print("transmitted string: ");
  Serial.println(trnsmit_str);
  vw_send((uint8_t *)trnsmit_str, 4);
  vw_wait_tx();         // Wait until the whole message is gone
  delay(500);  
}

