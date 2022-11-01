#define TIMER 6000000

const float mmPerPulse = 0.173;

float mmTotal = 0;

int preStatus = 0;

unsigned long millis = 0;

unsigned long startTime = 0;

unsigned long endTime = 0;

void displayTime(unsigned long);

void setup() {
  pinMode(9, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensor = digitalRead(9);

  if (sensor != preStatus) {
    startTime = millis(); // send message to esp32, starting to rain.
    
    mmTotal = mmTotal + mmPerPulse;
    
    if(mmTotal - mmPerPulse == 0){
        // send message to esp32, starting to rain
    }else{
        // send current mmTotal 
    }
    
    
    preStatus = sensor;

    
  }

  delay(500);

    if(millis() > millis + TIMER && startTime != 0){
    // if after 10m mmTotal don't change, send message to esp32, rain end
        displayTime(millis);
    
        startTime = 0;

        mmTotal = 0;
    }   
  
}

void displayTime(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.print("s.");
}
