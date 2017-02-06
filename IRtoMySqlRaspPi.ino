/* Arduino AquaCell reader.

 A .php file is requested (with measured numbers in the GET request) every minute (interval set at line #24)
 
 Changes 1 febr 2017:
 regel 79 & 80			Aangepast zodat berekende afstanden in eerst twee kolommen komen.
 regel 91, 93, 95 & 97	Variabele naar string geconverteerd ivm afwijkende waarden in db.

*/

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0x30, 0x32, 0x32};
IPAddress ip(192, 168, 2, 175);
IPAddress server(192, 168, 2, 29);
EthernetClient client;

const int IRPinA = 0;
const int IRPinB = 1;

long mcont1A = 0; //IR meter 1 raw value
long mcont1B = 0; //IR meter 1 milimeter value
long mcont2A = 0; //IR meter 2 raw value
long mcont2B = 0; //IR meter 2 milimeter value

long lastTime = 0;        // will store last time
long interval = 60000;    // interval (milliseconds)
unsigned int AvgNumber = 15;

void setup() {

  Serial.begin(115200);
  delay(1000);

  Ethernet.begin(mac, ip);
  delay(1000);

  pinMode(4, OUTPUT);                  // SD select pin
  digitalWrite(4, HIGH);               // Explicitly disable SD

}

void loop() {
  
  readIRsensors();

  if (millis() - lastTime > interval) {
    lastTime = millis();
   
    Serial.print(String(mcont1B) + " mm. (" + String(mcont1A) + ")\t")
    Serial.println (String(mcont2B) + " mm. (" + String(mcont2A) + ")"); 

    //send data to PHP/MySQL
    httpRequest();

    //Reset variables to zero for next run
    mcont1A = 0;
    mcont1B = 0;
    mcont2A = 0;
    mcont2B = 0;

    //Stop Ethernet
    client.stop();
  }
} //Einde loop

void readIRsensors() {
 
  unsigned int x=0;
  float ResultsA = 0.0;
  float ResultsB = 0.0;
  float AvgResultsA = 0.0;
  float AvgResultsB = 0.0;
  
  long volts1 = analogRead(A0); 
  mcont1A = volts1;
  
  long volts2 = analogRead(A1); 
  mcont2A = volts2;

  for (int x = 0; x < AvgNumber; x++){
   
   ResultsA = ResultsA + mcont1A;
   ResultsB = ResultsB + mcont2A;
   
   delay(1000);
   
  }
 
  AvgResultsA = ResultsA / AvgNumber;
  AvgResultsB = ResultsB / AvgNumber;
 
  float distance1 = pow(3027.4 / AvgResultsA, 1.2134);
  float distance2 = pow(3027.4 / AvgResultsB, 1.2134);
 
  mcont1B = long(distance1*10);
  mcont2B = long(distance2*10);
  
  // Debug waarden in Serial Monitor
  // Serial.print(String(mcont1B) + " mm.\t" + String(mcont2B) + " mm.\t\t"); 
  // Serial.println(String(mcont1A) + "\t" + String(mcont2A) + " ");
  
  delay(1000);

}

void httpRequest() {
  // if there's a successful connection:

  if (client.connect(server, 80)) {
    client.print("GET /salt.php?mcont1A=");
    client.print(String(mcont1A));
    client.print("&mcont1B=");
    client.print(String(mcont1B));
    client.print("&mcont2A=");
    client.print(String(mcont2A));
    client.print("&mcont2B=");
    client.print(String(mcont2B));
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.2.29");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
   
    // Debug waarden in Serial Monitor
    Serial.print("GET /salt.php?mcont1A=");
    Serial.print(String(mcont1A));
    Serial.print("&mcont1B=");
    Serial.print(String(mcont1B));
    Serial.print("&mcont2A=");
    Serial.print(String(mcont2A));
    Serial.print("&mcont2B=");
    Serial.print(String(mcont2B));
    Serial.println(" HTTP/1.1");   
   
    //Request complete; empty recieve buffer
    while (client.available()) { //data available
      char c = client.read(); //gets byte from ethernet buffer
    }
    client.println();
  }
  else {
    client.stop();
  }
}


