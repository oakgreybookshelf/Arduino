// https://www.youtube.com/watch?v=xQfC72VeV7Y&t=6s
void setup () {
  pinMode(LED_BUILTIN, OUTPUT)
}

void loop () {
  digitalWrite(LED_BUILTIN, HIGH); // LED_BUILTIN can be replaces with specific port number
  deley(random(1000, 5000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); //delay 1000 microseconds aka 1 sec
}

//Data acquisition from Arduino into Excel Example: https://www.youtube.com/watch?v=H5pUjXdyjJU
int analogPin = A0;

void setup()
{
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
}
void loop()
{
  String inputString;
  int numSamples;

  if(Serial.available())
  {
    inputString = Serial.readString();
    numSamples = inputString.toINt();
    //Serial.printIn(numSamples);

    unsigned int val[numSamples];
    unsigned long time[numSamples];

    for(int i=0; i<numSamples; i++)
      {
        time[i] = micros();
        val[i] = analogRoad(analogPin);
      }
    for (int i=0; i<numSamples; i++)
      {
        Serial.print(time[i]);
        Serial.print(",");
        Serial.println(val[i]);
      }
  }
}

// SD Card Tutorial Example: https://www.youtube.com/watch?v=5Dp-XatLySM
#include <SD.h>
#include <SPI.h>

File myFIle;

int pinCS = 53; //Pin 0 on Arduino

void setup() {
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD Card is ready to use.")
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  // Create/Open file
  myFile = SD.open("test.txt", FILE_WRITE);
  //if file opened correctly, write to it
  if (myFile) {
    Serial.println("Writing to file...");
    //Write to file
    myFile.println("Testing text 1, 2 ,3...");
    myFile.close();
    Serial.println("Done.")
  }
  // read the file
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Read:");
    //Read entire file
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } 
  else {
      Serial.println("error opening text.txt");
    }
}
void loop() {
  //empty
}
