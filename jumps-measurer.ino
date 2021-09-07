#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

int trigger1 = 25;
int eco1 = 32;
int eco2 = 33;
int eco3 = 34;
int eco4 = 35;

const char* ssid = "AGUARIBAY4";
const char* password = "33417812";

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.println("initializing setup...");
  Serial.begin(9600);
  initWiFi();
 
  pinMode(eco1, INPUT);
  pinMode(eco2, INPUT);
  pinMode(eco3, INPUT);
  pinMode(eco4, INPUT);
  pinMode(trigger1, OUTPUT);
  
  digitalWrite(trigger1, LOW); //Primero lo apago para tener el ciclo de una onda limpia

  Serial.println("set up initialized");

}

void loop() {
  //Serial.println("initializing loop...");

  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient http;
    
    http.begin("https://inline-skating-technology.rj.r.appspot.com/api/jumper");  
    http.addHeader("Content-Type", "application/json");
    
  String measures = "";
  float minDistance = getMinDistance(true);

  Serial.print("minDistance ");
  Serial.println(minDistance);
    while(minDistance < 180 && minDistance > 0) {
      minDistance = getMinDistance(true);
      Serial.print("minDistance ");
      Serial.println(minDistance);
      measures += String(minDistance) + ",";
      delay(50);
    }
  
  Serial.print("Distancias= ");
  Serial.print(measures);

  delay(500);
    while(minDistance > 180 || minDistance <= 0) {
      minDistance = getMinDistance(false);
      Serial.print(".");
      delay(20);
    }

   String httpRequestData = getRequestData(measures);
    Serial.print("httpRequestData ▼ ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);   

    if(httpResponseCode>0){
      Serial.println("Código HTTP ► " + String(httpResponseCode));   //Print return code

      if(httpResponseCode == 200){
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió ▼ ");
        Serial.println(cuerpo_respuesta);
  
      }

    }else{

     Serial.print("Error enviando POST, código: ");
     Serial.println(httpResponseCode);

    }

    http.end();  //libero recursos
    
  }
  else{
    
     Serial.println("Error en la conexión WIFI");
  }
}

int getDistance(int pinEcho){
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  
  long t = pulseIn(pinEcho, HIGH); //obtenemos el ancho del pulso
  return t/59;  
}

float getMinDistance (bool logs){
  long d1; //distancia en centimetros
  long d2;
  long d3;
  long d4;
 
  d1 = getDistance(eco1);
  delay(50);
  d2 = getDistance(eco2);
  delay(50);
  d3 = getDistance(eco3);
  delay(50);
  d4 = getDistance(eco4);
  
  long minDistance = d1;
  
  if (logs) {
    Serial.print("D1: " + String(d1) + "cm - D2: " + String(d2) + "cm - D3: " + String(d3) + "cm - D4: " + String(d4) + "cm - ");
  }
  
  if (d1 <= 0 || (d2 > 0 && d2 < minDistance)) {
    minDistance = d2;
  }
  if (minDistance <= 0 || (d3 > 0 && d3 < minDistance)) {
    minDistance = d3;
  }
  if (minDistance <= 0 || (d3 > 0 && d3 < minDistance)) {
    minDistance = d3;
  }
  if (minDistance <= 0 || (d4 > 0 && d4 < minDistance)) {
    minDistance = d4;
  }
  
  return minDistance;

}

String getRequestData(String measures) {
      return "{\"measures\":\"" + measures + "\"}";
}
    
