#define TIMER 60000

const float mmPerPulse = 0.173;

float mmTotal = 0;

int preStatus = 0;

unsigned long tempMillis = 0;

unsigned long startTime = 0;

unsigned long endTime = 0;

int count = 0;

int first = 0;

void displayTime(unsigned long);

void setup() {
  pinMode(9, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensor = digitalRead(9);
  if (first == 0) {
    preStatus = sensor;
    first += 1;
  }
  if (sensor != preStatus) {
    // send message to esp32, starting to rain.
    tempMillis = millis();
    mmTotal = mmTotal + mmPerPulse;

    if (mmTotal - mmPerPulse == 0) {
      startTime = millis();
      // send message to esp32, starting to rain
      Serial.println("Send Message to ESP32, STARTING TO RAIN.....");
    } else {
      // send current mmTotal

      Serial.print("Send current total mm rain: ");
      Serial.println(mmTotal);
    }


    preStatus = sensor;

    count += 1;
  } else {
    delay(500);

    if (millis() > tempMillis + TIMER && startTime != 0) {
      // if after 10m mmTotal don't change, send message to esp32, rain end
      displayTime(tempMillis - startTime + 1000);
      Serial.print("Count: ");
      Serial.println(count);

      Serial.println("Send Message to esp32 end");

      startTime = 0;

      mmTotal = 0;

      count = 0;

      tempMillis = millis();
    }
  }
}

void displayTime(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.println("s.");
}
