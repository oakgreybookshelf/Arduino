// install Accelerometer ADXL335 library in Arduino
/*
troubleshooting: 
- heck sensor config (range, power modE)
- check assigned pins
*/ 

#include <Accelerometer_ADXL335.h>
#include <SD.h>

float x_data, y_data, z_data;

ADXL335 adxl335;
const int chipSelectPin = 10; // change to SD CS pin

// prototypes
void readAccelerometer();
void writeToFile();
void writeToConsole();

void setup() {
    Serial.begin(115200);
    delay(500); //500 milisecs

    if (!SD.begin(chipSelectPin)) {
        Serial.println("SD initialization failed!");
        while (1); // halt
    }
    Serial.println("SD card initialized.");
}

void loop () {
    readAccelerometer(); 

    // outputs data to console
    writeToConsole();
    // saves data on txt file
    writeToFile();

    delay(1000);
}

void readAccelerometer() {

    x_data = adxl335.getAccelerationX();
    y_data = adxl335.getAccelerationY();
    z_data = adxl335.getAccelerationZ();

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

}

void writeToFile() {
    File dataFile;
    dataFile = SD.open("adxl335_data.txt", FILE_WRITE);

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