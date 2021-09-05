int trigger = 2;
int eco1 = 32;
int eco2 = 33;
int eco3 = 34;
int eco4 = 35;

unsigned long contador = 0;

float getMinDistance (){
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(trigger, LOW);
  
  t = pulseIn(eco1, HIGH); //obtenemos el ancho del pulso
  d = t/59;

  long minDistance = d;
  /*t = pulseIn(eco2, HIGH); //obtenemos el ancho del pulso
  d = t/59;
  if (d < minDistance) {
    minDistance = d;
  }
  
  t = pulseIn(eco3, HIGH); //obtenemos el ancho del pulso
  d = t/59;
  if (d < minDistance) {
    minDistance = d;
  }
  
  t = pulseIn(eco4, HIGH); //obtenemos el ancho del pulso
  d = t/59;
  if (d < minDistance) {
    minDistance = d;
  }*/
  
  return minDistance;
}

void setup() {
  Serial.begin(9600);

  pinMode(eco1, INPUT);
  pinMode(eco2, INPUT);
  pinMode(eco3, INPUT);
  pinMode(eco4, INPUT);
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);

}

void loop() {
  
  String measures = "";
  float minDistance = getMinDistance();
  while(minDistance < 200 ) {
    measures += String(minDistance) + ",";
    delay(5);
    minDistance = getMinDistance();
  }
  
  Serial.print("Distancias= ");
  Serial.println(measures);
  Serial.println("");

  delay(500);
}
