#include <ESP8266WiFi.h>
char * MakerIFTTT_Event = "Accident";
char * MakerIFTTT_Key = "ZRp8Dg3RtdlV3NcBM5ncVYTUX7M7vtJIJCO2ZgH_Nx";
char * hostDomain = "maker.ifttt.com";
const int hostPort = 80;

 const int sleepTimeS = 30;

int distanceThreshold = 0;
int cm = 0;
int inches = 0;
int tiltRead;

char * ssid = "My-Wifi";
char * password = "WiFi@123$";

WiFiClient client;

 
void setup() 
{
  Serial.begin(115200);
  delay(50);
  Serial.println();
  Serial.println("ESP8266 in normal mode");
  Serial.println();
  pinMode(A5, INPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);

  connectToWiFi(ssid, password);

}

void loop() 
{

  distanceThreshold = 150;
  cm = 0.01723 * readUltrasonicDistance(7, 6);
  inches = (cm / 2.54);
  Serial.print("\nTilt sensor read: ");
  Serial.println(tiltRead);
  Serial.print(cm);
  Serial.print("cm, ");
  Serial.print(inches);
  Serial.println("in\n");
  tiltRead = digitalRead(A5);
  
  if(cm <= distanceThreshold)
  {
    digitalWrite(13, HIGH);
    delay(1000);
    Serial.print("Vehicle very near detected.");
  }
  
  if(tiltRead < 1)
  {
    digitalWrite(9, HIGH);
    delay(1000);
    Serial.print("Helmet tilted.");
  }

  if(tiltRead < 1 && cm <= distanceThreshold )
  {
    ifttt_trigger(MakerIFTTT_Key, MakerIFTTT_Event);
  }

}


void connectToWiFi( char * ssid, char * pwd)
{
  int ledState = 0;
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


String ifttt_trigger(String KEY, String EVENT) 
{
  String name = "";
  client.stop();

  if (client.connect(hostDomain, hostPort)) 
  {

    Serial.println("Connected");
    String toSend = "GET /trigger/";
    toSend += MakerIFTTT_Event;
    toSend += "/with/key/";
    toSend += MakerIFTTT_Key;
    toSend += "Estimated distance: ";
    toSend += distance;
    toSend += "tilt";
    toSend += tilt;
    toSend += hostDomain;
    toSend += "\r\n";
    toSend += "connection: close\r\n\r\n";
    client.print(toSend);

  }
  else 
  {
    Serial.println("Connection failed");
    return "FAIL";
  }
}