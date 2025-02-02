#include <VirtualWire.h>
#include <SoftwareSerial.h>
#define RX 2
#define TX 3
String AP = "Vamsi";       // AP NAME
String PASS = "012345679"; // AP PASSWORD
String API = "E7I5622LBI4ZKDEH";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false;
int valSensor;
SoftwareSerial esp8266(RX,TX); 
#define gas_threshold 80
#define siren 7
const int receive_pin = 12; 
int led = 10,gas_level;
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
  delay(1000);
  Serial.println("Receiving sensor DATA from all nodes....."); 
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

}
void loop() 
{
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
   int i;  
   if (vw_get_message(buf, &buflen))  // when msg is received
     {
        digitalWrite(led,HIGH);
        if(buf[1]=='2')     // check node number
        {  
          Serial.println("Node 1"); // print node number and
          Serial.print("gas level: ");  // all sensor values
          for (i = 2; i < 4; i++) Serial.print(buf[i]-48);             
          Serial.println(" %"); 
    gas_level = (buf[2]-48)*10 + (buf[3]-48)*1;
           if(gas_level>gas_threshold) digitalWrite(siren,HIGH);          
           else digitalWrite(siren,LOW);   
         } 
     if(buf[1]=='3')
        {  
          Serial.println("Node 2");
          Serial.print("Gas Level: "); 
          for (i = 2; i < 4; i++) Serial.print(buf[i]-48);             
          Serial.println(" %");
          gas_level = (buf[2]-48)*10 + (buf[3]-48)*1;
           if(gas_level>gas_threshold) digitalWrite(siren,HIGH); 
          else digitalWrite(siren,LOW);                  
       } 
     }
      valSensor = gas_level;
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
  }

  void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
