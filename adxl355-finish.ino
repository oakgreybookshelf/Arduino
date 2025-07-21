// install PL ADXl355 library in Arduino
/*
troubleshooting: 
- heck sensor config (range, power modE)
- check SPI wriing and assigned pins (lines 24-25)
*/ 

#include <SPI.h>
#include <PL_ADXL355.h>
#include <SD.h>

int analogPin = A0;

// value range
const int RANGE_2G = 0x01;
const int RANGE_4G = 0x02;
const int RANGE_8G = 0x03;
const int MODE = 0x06; // accelerometer

float x_data, y_data, z_data, temp;

PL::ADXL355 adxl355;
uint8_t spiCsPin = 2;
const int chipSelectPin = 10; // change to SD CS pin

// prototypes
void readAccelerometer();
void writeToFile();
void writeToConsole();

void setup() {
    Serial.begin(115200);
    while(!Serial);
    delay(500); //500 milisecs

    adxl355.beginSPI(spiCsPin);

    if (!SD.begin(chipSelectPin)) {
        Serial.println("SD initialization failed!");
        while (1); // halt
    }
    Serial.println("SD card initialized.");
}

void loop () {
    readAccelerometer(); 

    // outputs data to console
    void writeToConsole();
    // saves data on txt file
    writeToFile();

    delay(1000);
}

void readAccelerometer() {

    auto results = adxl355.getAccelerations();

    if (!results.empty()) {
        x_data = results[0].x;
        y_data = results[0].y;
        z_data = results[0].z;
    }

    temp = adxl355.getTemperature();
}

void writeToConsole() {
    Serial.print("X: ");
    Serial.print(x_data, 4); // shows 4 decimals
    Serial.print("\t");

    Serial.print("Y: ");
    Serial.print(y_data, 4);
    Serial.print("\t");

    Serial.print("Z: ");
    Serial.print(z_data, 4);
    Serial.print("\n");

    Serial.print("Temp: ");
    Serial.print(temp, 4);
}

void writeToFile() {
    File dataFile;
    dataFile = SD.open("adxl355_data.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.print("X: ");
      dataFile.print(x_data, 4);
      dataFile.print(", Y: ");
      dataFile.print(y_data, 4);
      dataFile.print(", Z: ");
      dataFile.println(z_data, 4);
      dataFile.close();
      Serial.println("Data written to file.");
    } else {
      Serial.println("Failed to open file.");
    }
}

//controls pins high/low state, power
//digitalWrite(pin#, high/low)

//writes data to specific location on sensor/device
//writeRegister(address, data)

// void clearFifo() discards 3-axis samples in the FIFO
