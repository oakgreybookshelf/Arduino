// ADXL355 BBoard - v2
// http://adafru.it/163 
// W14545-A
// https://outlook.office365.com/mail/inbox/id/AAQkAGRiMmQ2YWZjLWU3ODktNDQzZi05YWFmLTQyMTk2MjExMjlhMQAQAPHoywUd95hKpryqqBe%2BfTc%3D

void setup() {
  Serial.begin(9600);
  SPI.begin();

  // Initalize the  data ready and chip select pins:
  pinMode(CHIP_SELECT_PIN, OUTPUT);

  //Configure ADXL355:
  writeRegister(RANGE, RANGE_2G); // 2G
  writeRegister(POWER_CTL, MEASURE_MODE); // Enable measure mode

  // Give the sensor time to set up:
  delay(100);
}

void loop() {
  int axisAddresses[] = {XDATA1, XDATA2, XDATA3, YDATA1, YDATA2, YDATA3, ZDATA1, ZDATA2, ZDATA3};
  int axisMeasures[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int dataSize = 9;

  // Read accelerometer data
  readMultipleData(axisAddresses, dataSize, axisMeasures);

  // Split data
  int xdata = (axisMeasures[0] >> 4) + (axisMeasures[1] << 4) + (axisMeasures[2] << 12);
  int ydata = (axisMeasures[3] >> 4) + (axisMeasures[4] << 4) + (axisMeasures[5] << 12);
  int zdata = (axisMeasures[6] >> 4) + (axisMeasures[7] << 4) + (axisMeasures[8] << 12);

    // Print axis
  Serial.print("X=");
  Serial.print(xdata);
  Serial.print("\t");
  
  Serial.print("Y=");
  Serial.print(ydata);
  Serial.print("\t");

  Serial.print("Z=");
  Serial.print(zdata);
  Serial.print("\n");

    // Next data in 100 milliseconds
  delay(100);
}

