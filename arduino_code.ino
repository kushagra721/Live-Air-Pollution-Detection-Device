#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

MDNSResponder mdns;
Adafruit_BMP280 bmp; // I2C

// Replace with your network credentials
const char* ssid = "admin";
const char* password = "12345678g";

ESP8266WebServer server(80);
String webSite="";
String tem="[";
String air="[";
String pre="[";
String alt="[";
int mq135 = A0;
int data = 0;


void display_data()
{
//  Serial.println(tem);
webSite="";
        webSite+="<script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js'></script><div style='width:40%; display:flex'><canvas id='myChart' style='padding:20px; margin-right: 30px; border:red 2px solid' width='20' height='10'></canvas><canvas id='myChart1' style='padding:20px;border:skyblue 2px solid' width='20' height='10'></canvas></div><div style='width:40%; display:flex; margin-top:20px'><canvas id='myChart2' style=' padding:20px; margin-right: 30px; border:yellow 2px solid' width='20' height='10'></canvas><canvas id='myChart3' style='padding:20px;border:orange 2px solid' width='20' height='10'></canvas></div><script>";
          webSite+="var airValues = "; 
          webSite+=air+";";
          webSite+="var tempValues = ";
          webSite+=tem+";";
          webSite+="var preValues = ";
          webSite+=pre+";";
          webSite+="var altValues = ";
          webSite+=alt+";";
webSite+="var xValues = [0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5];";
webSite+="new Chart('myChart', {type: 'line',data: {labels: xValues,datasets: [{backgroundColor: 'transparent',borderColor: 'red',data: airValues}]},options:{legend: {display: false},title: {display: true,text: 'Air Quealty Data'}}});new Chart('myChart1', {type: 'line',data: {labels: xValues,datasets: [{backgroundColor: 'transparent',borderColor: 'skyblue',data: tempValues}]},options:{legend: {display: false},title: {display: true,text: 'Temperature( Celsius)'}}});";
webSite+="new Chart('myChart2', {type: 'line',data: {labels: xValues,datasets: [{backgroundColor: 'transparent',borderColor: 'yellow',data: preValues}]},options:{legend: {display: false},title: {display: true,text: 'Pressure'}}});";
webSite+="new Chart('myChart3', {type: 'line',data: {labels: xValues,datasets: [{backgroundColor: 'transparent',borderColor: 'orange',data: altValues}]},options:{legend: {display: false},title: {display: true,text: 'Approx altitude'}}});</script>";
}
void setup(void){
Serial.begin(115200);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin(0x76);
  if (!status) {
  Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
                 while (1) delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  webSite +="<p>Welcome</p>";
  
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
 
 if (mdns.begin("esp8266", WiFi.localIP())) {
   Serial.println("MDNS responder started");
  }
    server.on("/", [](){
    server.send(200, "text/html", webSite);
    });
    
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
  tem="[";
  air="[";
  pre="[";
  alt="[";
  for(int i=0;i<11;i++)
  {
    String temp=String(bmp.readTemperature());
    String temp1=String(analogRead(mq135));
    String temp2=String(bmp.readPressure());
    String temp3=String(bmp.readAltitude(1013.25));
    tem+=temp;
    air+=temp1;
    pre+=temp2;
    alt+=temp3;
    tem+=", ";
    air+=", ";
    pre+=", ";
    alt+=", ";
    delay(500);
  }
  tem+="]";
    air+="]";
    pre+="]";
    alt+="]";
display_data();
}