#include <VirtualWire.h>
#define gas_threshold 80
#define siren 7
const int receive_pin = 12; 
int led = 13,gas_level,ir_level;
void setup() 
{
  Serial.begin(9600);                   // setup serial communication
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true);          
  vw_setup(2000);                       // wireless data rate
  vw_rx_start();                    // Start the receiver PLL running
  pinMode(led,OUTPUT); 
  pinMode(siren,OUTPUT);   
  digitalWrite(led,LOW);
  digitalWrite(siren,LOW);
  delay(100);
  Serial.println("Receiving sensor DATA from all nodes....."); 
}
void loop() 
{
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
   int i;  
   if (vw_get_message(buf, &buflen))  // when msg is received
     {
        digitalWrite(led,HIGH);
        if(buf[1]=='1')     // check node number
        {  
          //Serial.println("Node 1"); // print node number and
          //Serial.print("gas level: ");  // all sensor values
          for (i = 2; i < 4; i++) //Serial.print(buf[i]-48);             
          //Serial.println(" %"); 
          gas_level = (buf[2]-48)*10 + (buf[3]-48)*1;
           if(gas_level>gas_threshold) digitalWrite(siren,HIGH);          
           else digitalWrite(siren,LOW);   
         } 
     if(buf[1]=='2')
        {  
          //Serial.println("Node 2");
          //Serial.print("Fire Level: "); 
          for (i = 2; i < 4; i++) //Serial.print(buf[i]-48);             
          //Serial.println(" %");
          ir_level = (buf[2]-48)*10 + (buf[3]-48)*1;
           if(gas_level>gas_threshold) digitalWrite(siren,HIGH); 
          else digitalWrite(siren,LOW);                  
       } 
     }
     delay(500);
     //Serial.print("gas_level: ");
     Serial.print(gas_level );
     //Serial.print("fire_level: ");
     Serial.println( fire_level);
  }

