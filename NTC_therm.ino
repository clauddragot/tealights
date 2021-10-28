#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>  

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,0); 





const double VCC = 3.3;             
const double R2 = 9400;            
const double adc_resolution = 1023; 

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741; 

double temperature;
String text = "";

//WiFi const  
ESP8266WebServer server(80);


const char* KNOWN_SSID[] = {"claudiu", "HUAWEI P9 lite 2017"};
const char* KNOWN_PASSWORD[] = {"19950723", "stais-ocaut"};

const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); 


//SPIFFS 
String getContentType(String filename); 
bool handleFileRead(String path);       


void setup() {
  Serial.begin(9600); 
  boolean wifiFound = false;
  int i, n;
WiFi.persistent(false);
 WiFi.mode(WIFI_OFF);

 mySerial.begin(9600); 

  //WiFi config
  
  Serial.println("scan start");
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println("scan done");
  if (nbVisibleNetworks == 0) {
    Serial.println("no networks found. Reset to try again");
    while (true);
  }


  Serial.print(nbVisibleNetworks);
  Serial.println(" network(s) found");



  for (i = 0; i < nbVisibleNetworks; ++i) {
    Serial.println(WiFi.SSID(i)); 
    for (n = 0; n < KNOWN_SSID_COUNT; n++) { 
      if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
        
      } else { // we got a match
        Serial.println("\tWe got a match! ");
        wifiFound = true;
        break; 
      }
    } 
    if (wifiFound) break; 
  } 

  if (!wifiFound) {
    Serial.println(F("no Known network identified. Reset to try again"));
    while (true); 
  }

  Serial.print("\nConnecting to ");
  Serial.println(KNOWN_SSID[n]);

 
  WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
     
     delay(500);
     Serial.print("*");
  }
  
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());        

  Serial.println("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());


  server.on("/data.txt", [](){
   text = (String)temperature;
   server.sendHeader("Access-Control-Allow-Origin","*");
   server.send(200, "text/html", text);
   
 });


  //SPIFFS 
  if (MDNS.begin("esp8266")) {         
      Serial.println("mDNS responder started");
    } else {
      Serial.println("Error setting up MDNS responder!");
    }

    server.onNotFound([]() {                              
      if (!handleFileRead(server.uri()))                  
        server.send(404, "text/plain", "404: Not Found"); 
    });
  

  SPIFFS.begin();                           
  server.begin();  //Start the server
  Serial.println("Server listening");
}

void loop() {
 
  
  double Vout, Rth, adc_value; 

  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

/*  Steinhart-Hart Thermistor Equation: */
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   

  temperature = temperature - 273.15;  
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" degree celsius");


 
  mySerial.write("Hello world");


  
  delay(1000);
  server.handleClient();



}


String getContentType(String filename) { 
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".txt")) return "text/html";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";        
  String contentType = getContentType(path);            
  if (SPIFFS.exists(path)) {                           
    File file = SPIFFS.open(path, "r");                
    size_t sent = server.streamFile(file, contentType); 
    file.close();                                      
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                       
}
