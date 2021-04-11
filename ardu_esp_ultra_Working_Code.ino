#include<SoftwareSerial.h>

#define RX 10
#define TX 11

String AP   = "AsusZenFone";//"Chandan1998";//"AsusZenFone";//"christion";       // CHANGE ME
String PASS = "shubham8388";//"Chandan1998";//"2399723997"; // CHANGE ME

String API = "612S7C199L2YBDFD";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80"; 
String field = "field7";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;//1

SoftwareSerial esp8266(RX,TX); 

/////ultrasonic code /////

int trigPin = 8;    // Trigger
int echoPin = 9;    // Echo
long duration, cm;

////////////
 
void setup() {

/////// ultrasonic sensor ///////
//Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

////////////////////////////////

  Serial.begin(9600);
  esp8266.begin(115200);//(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() 
{
 //////// ultrasonic sensor////////

// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  //delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);


  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  //inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
 // Serial.print(inches);
 // Serial.print("in, ");

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  //delay(1000);

//////////////////////////////////

   //return (cm);
  //return random(cm); // Replace with
  delay(500); 
 valSensor = cm;//getSensorData();
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
 
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 
 esp8266.println(getData);
 
 
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

int getSensorData()
{
 
}

void sendCommand(String command, int maxTime, char readReplay[])
{
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
