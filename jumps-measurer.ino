int trigger1 = 25;
int trigger2 = 26;
int eco1 = 32;
int eco2 = 33;
int eco3 = 34;
int eco4 = 35;

void setup() {
  Serial.println("initializing setup...");
  Serial.begin(9600);
 
  pinMode(eco1, INPUT);
  pinMode(eco2, INPUT);
  //pinMode(eco3, INPUT);
  //pinMode(eco4, INPUT);
  pinMode(trigger1, OUTPUT);
  pinMode(trigger2, OUTPUT);
  
  digitalWrite(trigger1, LOW); //Primero lo apago para tener el ciclo de una onda limpia
  digitalWrite(trigger2, LOW);

  Serial.println("set up initialized");
}

void loop() {
  //Serial.println("initializing loop...");
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

  delay(2000);
  while(minDistance > 180 || minDistance <= 0) {
    minDistance = getMinDistance(false);
    Serial.print(".");
    delay(20);
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
  delay(10);
  d2 = getDistance(eco2);
  delay(10);
  /*d3 = getDistance(eco3);
  delay(10);
  d4 = getDistance(eco4);*/
  long minDistance = d1;
  if (logs) {
    Serial.print("D1: " + String(d1) + "cm - D2: " + String(d2)/* + "cm - D3: " + String(d3) + "cm - D4: " + String(d4)*/ + "cm - ");
  }
  
  if (d1 <= 0 || (d2 > 0 && d2 < minDistance)) {
    minDistance = d2;
  }
  if (minDistance <= 0 || (d3 > 0 && d3 < minDistance)) {
    minDistance = d3;
  }
  /*if (minDistance <= 0 || (d3 > 0 && d3 < minDistance)) {
    minDistance = d3;
  }
  if (minDistance <= 0 || (d4 > 0 && d4 < minDistance)) {
    minDistance = d4;
  }*/
  
  return minDistance;
}
