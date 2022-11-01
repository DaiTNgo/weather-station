#include "DHT.h"
#include <SoftwareSerial.h>

#define TIMER 600000
#define UV_OUT A0
#define REF_3V3 A1
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial mySerial(10, 11); // RX, TX

const float mmPerPulse = 0.0058823529;

float mmTotal = 0;

int preStatus = 0;

unsigned long tempMillis = 0;

unsigned long startTime = 0;

unsigned long endTime = 0;

int first = 0;

int count = 1;

void displayTime(unsigned long);

void setup() {
  pinMode(9, INPUT);
  Serial.begin(9600);
  dht.begin();

  while (!Serial) {
  }
  Serial.println("Goodnight moon!");
  mySerial.begin(115200);
}

void loop() {
  // Start ML8511
  int uv_Level = analogRead_average(UV_OUT);
  int ref_Level = analogRead_average(REF_3V3);

  float output_Voltage = 3.3 / ref_Level * uv_Level;
  float uvIntensity = mapfloat(output_Voltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level -numbers from datasheet-

  Serial.print("ML8511 output: ");
  Serial.print(uv_Level);

  Serial.print(" / ML8511 voltage: ");
  Serial.print(output_Voltage);

  Serial.print(" / UV Intensity (mW/cm^2): ");
  Serial.print(uvIntensity);

  Serial.println();
  // End ML8511

  /* Start DHT22*/
  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  /* End DHT22*/
  uart(String(uvIntensity), String(t), String(h), String(hic), String(hif));
  // Start Rain
  int sensor = digitalRead(9);
  String jsonRain;
  if (first == 0) {
    preStatus = sensor;
    first += 1;
  }
  if (sensor != preStatus) {
    // send message to esp32, starting to rain.
    tempMillis = millis();
    mmTotal = mmTotal + mmPerPulse;
    uartRain(String(mmTotal), String(count));
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
  } else {
    delay(500);

    if (millis() > tempMillis + TIMER && startTime != 0) {
      // if after 10m mmTotal don't change, send message to esp32, rain end
      displayTime(tempMillis - startTime + 1000);

      Serial.println("Send Message to esp32 end");

      startTime = 0;

      mmTotal = 0;
      
      uartRain(String(mmTotal), String(count));
      tempMillis = millis();
      count += 1;
    }
  }
}
void uartRain(String rain, String rainId) {
  String  jsonRain = "{\"rain\":" + rain
                     + ", \"rainId\" : " + rainId
                     + "}";
  mySerial.println(jsonRain);
  mySerial.flush();
}

void uart(String uv, String temperature, String humidity, String hic, String hif)
{
  String json =  "{\"temperature\" : " + temperature
                 + ", \"humidity\" : " + humidity
                 + ",\"hic\":" + hic
                 + ",\"hif\":" + hif
                 + ",\"uv\":" + uv
                 + "}";
  mySerial.println(json);
  mySerial.flush();
}

void displayTime(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.println("s.");
}

int analogRead_average(int pinToRead)
{
  int NumberOfSamples = 8;
  int runningValue = 0;

  for (int x = 0; x < NumberOfSamples; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= NumberOfSamples;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
