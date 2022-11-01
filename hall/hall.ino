const float mmPerPulse = 0.173;  //put here the value of rain in mm for each movement of the bucket

float mmTotali = 0;
int sensore = 0;
int statoPrecedente = 0;

void setup() {
  pinMode(9, INPUT);
  Serial.begin(9600);
}

void loop() {
  sensore = digitalRead(9);

  if (sensore != statoPrecedente) {
    mmTotali = mmTotali + mmPerPulse;
  }

  delay(500);

  statoPrecedente = sensore;
  Serial.println(mmTotali);
}
