#define INTERVAL1 5000

unsigned long millis_1 = 0;

void displayTime(unsigned long);
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (millis() > millis_1 + INTERVAL1)
  {
    millis_1 = millis();
    displayTime(millis_1);
    Serial.println("1");
  }
}
void displayTime(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.print("s - ");
}
