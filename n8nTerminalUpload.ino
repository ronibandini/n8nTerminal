// n8n terminal 1.0
// Roni Bandini, August 2025, MIT License
// Hardware: Unihiker K10 + 2 gravity buttons

#include "unihiker_k10.h"
#include "img.h"
#include <WiFi.h>
#include <HTTPClient.h> 
#include <ArduinoJson.h>

void onButtonAPressed();
void onButtonBPressed();
void callWebhookAndDisplay();

// Settings
const char* ssid = "";
const char* password = ""; 
const char* webhook_url = "";

UNIHIKER_K10 k10;
uint8_t screen_dir = 2;
Music music;

int line;
int startingLine=50;
String qrIdEnvio="";
String qrName="";
String qrAddress="";

String fieldName1="Shipping #";
String fieldName2="Name";
String fieldName3="Address";
String fieldName4="Notes";    


void setup() {
    k10.begin();

    pinMode(P0, INPUT);
    pinMode(P1, INPUT);

    Serial.begin(115200);
    
    lightRed();
    Serial.println("N8n Screen Started");
    Serial.println("Roni Bandini, 8.25");

    k10.buttonA->setPressedCallback(onButtonAPressed);
    k10.buttonB->setPressedCallback(onButtonBPressed);

    k10.initSDFile();
    k10.initScreen(screen_dir);
    k10.creatCanvas();  
        
    cleanScreen();    
    updateScreenTitle("Boot");
    music.playTFCardAudio("S:/sound2.wav");
    updateScreen("n8n Terminal started");
    updateScreen("Roni Bandini - 08.25");

    // WiFi connection
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi OK");
        updateScreen("Connected to WiFi...");
    } else {
        Serial.println("\nWiFi FAIL");
        updateScreen("Wifi failed...");
    }

      // Get local IP
      IPAddress localIP = WiFi.localIP();
      
      // Get current time via NTP
      configTime(0, 0, "pool.ntp.org", "time.nist.gov");  // UTC
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
          Serial.println("Failed to obtain time");
      } else {
          Serial.printf("Current time: %02d:%02d:%02d\n",
                        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      }
    updateScreen("Local IP: "+localIP.toString());
    updateScreen("NTP: "+String(timeinfo.tm_hour) +" "+String(timeinfo.tm_min));
    updateScreen("Ready to start some automations :)");
    updateScreen("Waiting for buttons...");

    // blink
    lightRed();
    delay(500);  
    lightOff();
    lightRed();
    delay(500);
    lightOff();
    lightBlue();
    
}

void cleanScreen(){
 k10.canvas->canvasDrawBitmap(0, 0, 240, 320, back);
 k10.canvas->updateCanvas(); 
 line=startingLine;
 delay(800);
}

void updateScreen(String myMessage){

   k10.canvas->canvasText(myMessage, 7, line, 0x000000, k10.canvas->eCNAndENFont16, 35, true);
   k10.canvas->updateCanvas();
   line=line+15;
   Serial.println("\nLine");
   Serial.println(line); 
   
   if (line>315){
    cleanScreen();
    k10.canvas->canvasText(myMessage, 7, line, 0x000000, k10.canvas->eCNAndENFont16, 35, true);
    k10.canvas->updateCanvas();
    line=line+15;
   }

}

void updateScreenTitle(String myMessage){
    
   k10.canvas->canvasText(myMessage, 5, line, 0xFFA500, k10.canvas->eCNAndENFont24, 35, true);
   k10.canvas->updateCanvas();

   line=line+30;
   Serial.println("\nLine");
   Serial.println(line); 
   
   if (line>315){
    cleanScreen();
    k10.canvas->canvasText(myMessage, 5, line, 0xFFA500, k10.canvas->eCNAndENFont24, 35, true);
    k10.canvas->updateCanvas();
    line=line+30;
    }
}

void lightBlue(){
    k10.rgb->brightness(7);
    k10.rgb->write(0, 0x0000FF); 
  }

void lightRed(){
    k10.rgb->brightness(7);
    k10.rgb->write(0, 0xFF0000); 
  }

void lightGreen(){
    k10.rgb->brightness(7);
    k10.rgb->write(0, 0x00FF00); 
}

void lightOff(){
    k10.rgb->brightness(0);
    k10.rgb->write(0, 0x000000); 
}

void beeps(){
  music.playTone(131,  1000);
  music.playTone(196,  1000);
  music.playTone(220,  500);
}

void callWebhookAndDisplay(String pressed) {
    cleanScreen();
    lightRed();   
    updateScreen("Calling workflow...");         
    beeps();

    if (WiFi.status() != WL_CONNECTED) {        
        updateScreen("WiFi is down...");
        Serial.println("Webhook call aborted: No WiFi connection.");
        return;
    }
    
    Serial.println("Calling webhook...");
    HTTPClient http;

    http.begin(String(webhook_url) + "?button=" + pressed);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.GET();
    Serial.printf("HTTP Response Code: %d\n", httpCode);

    if (httpCode <= 0) {
        updateScreen("HTTP error...");
        http.end();
        Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
        return;
    }

    String response = http.getString();
    http.end();
    Serial.println("---- RAW RESPONSE ----");
    Serial.println(response);

    // Parseo el raw JSON 
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (error) {
        updateScreen("Json parsing error...");
        Serial.printf("JSON deserialization failed with error: %s\n", error.c_str());
        return;
    }

    // Check if the response is an array and access the first element
    if (!doc.is<JsonArray>()) {
        updateScreen("Not a Json array...");
        Serial.println("Response is not a JSON array.");
        return;
    }

    // Access the first object in the array
    JsonObject root = doc[0];
    
    Serial.println("Extracting values");
    
    // Step 2: Extract values from the "output" object within the array
    const char* idEnvio = root["output"]["idEnvio"];  
    const char* nombre = root["output"]["nombre"];
    const char* direccion = root["output"]["direccion"];  
    const char* detalle = root["output"]["detalle"];  

    Serial.println(nombre);
    Serial.println(direccion);
    
    // store for QR
    qrIdEnvio=idEnvio;
    qrName=nombre;
    qrAddress=direccion;

    lightGreen();
    
    // Step 3: Display values
    updateScreenTitle(fieldName1);
    updateScreen(idEnvio);
    updateScreenTitle(fieldName2);
    updateScreen(nombre);
    updateScreenTitle(fieldName3);
    updateScreen(direccion);      
    updateScreenTitle(fieldName4);
    updateScreen(detalle);   
    delay(3000);
    
    lightOff();
}

void onButtonAPressed() {        
    callWebhookAndDisplay("1");
}

void onButtonBPressed() {  

  if (strlen(qrIdEnvio.c_str()) > 0) {
    cleanScreen();
    beeps();  
    lightGreen(); 
    delay(3000);
    k10.canvasDrawCode(qrIdEnvio+"-"+qrName+"-"+qrAddress);
    k10.canvas->updateCanvas();
    delay(5000);
    k10.canvas->canvasClear();  
    delay(3000);
    lightOff(); 
   }
   else
   {
    updateScreen("No data for QR label...");
    delay(1000);
    }
}

void loop() {
  int connectedButton0=digitalRead(P0);
  int connectedButton1=digitalRead(P1);

  if (connectedButton0==HIGH){    
    Serial.println("External button 0");
    callWebhookAndDisplay("1");
  }

  if (connectedButton1==HIGH){    
    Serial.println("External button 1");
    onButtonBPressed();
  }
  
  delay(50);
  
}