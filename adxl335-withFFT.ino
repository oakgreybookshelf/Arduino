/* ****************************************************************************
 * Calibration and Base Code GitHub: https://github.com/ahmadlogs/adxl335
 * Base Code YouTube Tutorial: https://www.youtube.com/watch?v=C9efrWYrVVw
 * this code is for live reporting, it doesn't log/save data
 *****************************************************************************/

#include <arduinoFFT.h>

//--------------------------------------------------------------
#define xPin A1
#define yPin A2
#define zPin A3
//--------------------------------------------------------------

//calibrated minimum and maximum Raw Ranges for each axis
//use calibration.ino file to get these values
// REPLACE LINES 24-31 WITH ACTUAL VALUES NOT ZERO**********************
int xMin = 0; 
int xMax = 0;

int yMin = 0;  
int yMax = 0;

int zMin = 0;  
int zMax = 0;
//--------------------------------------------------------------

// FFT parameters
#define SAMPLES 16  // Must be power of 2 - use same sample count for all data
#define sampling_frequency 10000  // 10kHz sampling rate
#define sampling_period_us 100  // 100 microseconds for 10kHz
ArduinoFFT<double> FFT = ArduinoFFT<double>();
double vReal[SAMPLES];
double vImag[SAMPLES];
//--------------------------------------------------------------

int runCount = 0;

void setup() 
{
  //analogReference(EXTERNAL);
  Serial.begin(115200);
  randomSeed(analogRead(A0));  // Initialize random seed
}

unsigned long previousMillis = 0;
const long sampleInterval = 100; // 100 ms (10 Hz)

void loop() 
{
  if (runCount < 1) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= sampleInterval) {
      previousMillis = currentMillis;
      
      int xRaw[SAMPLES], yRaw[SAMPLES], zRaw[SAMPLES];
      double xAmp[SAMPLES/2], yAmp[SAMPLES/2], zAmp[SAMPLES/2];
      
      unsigned long microseconds;
      for (int i = 0; i < SAMPLES; i++) {
        microseconds = micros();
        xRaw[i] = analogRead(xPin);
        yRaw[i] = analogRead(yPin);
        zRaw[i] = analogRead(zPin);
        while (micros() < (microseconds + sampling_period_us)) { }
      }

      // Print raw values
      Serial.println("xRaw,yRaw,zRaw"); // header for raw values
      for(int i=0; i<SAMPLES; i++) {
        Serial.print(xRaw[i]);
        Serial.print(",");
        Serial.print(yRaw[i]);
        Serial.print(",");
        Serial.println(zRaw[i]);
      }
      
      // Print converted values (G) for each sample
      Serial.println("xG,yG,zG"); // header for converted values
      for(int i=0; i<SAMPLES; i++) {
        float x_g = map(xRaw[i], xMin, xMax, -1000, 1000) / 1000.0;
        float y_g = map(yRaw[i], yMin, yMax, -1000, 1000) / 1000.0;
        float z_g = map(zRaw[i], zMin, zMax, -1000, 1000) / 1000.0;
        Serial.print(x_g, 3);
        Serial.print(",");
        Serial.print(y_g, 3);
        Serial.print(",");
        Serial.println(z_g, 3);
      }
      
      // Process FFT for all axes using the same samples
      processFFT(xRaw, yRaw, zRaw, xAmp, yAmp, zAmp);
      
      // Print FFT results
      Serial.println("Frequency,X_Amp,Y_Amp,Z_Amp");
      for (int i = 0; i < SAMPLES/2; i++) {
        float freq = (i * 1.0 * sampling_frequency) / SAMPLES;
        Serial.print(freq, 1);
        Serial.print(",");
        Serial.print(xAmp[i]);
        Serial.print(",");
        Serial.print(yAmp[i]);
        Serial.print(",");
        Serial.println(zAmp[i]);
      }
      
      runCount++;
    } 
  }
}

//--------------------------------------------------------------
// FFT processing function 
void processFFT(int* xRaw, int* yRaw, int* zRaw, double* xAmp, double* yAmp, double* zAmp) {
  // Process X-axis FFT
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = xRaw[i];
    vImag[i] = 0;
  }
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);
  for (int i = 0; i < SAMPLES/2; i++) xAmp[i] = vReal[i];
  
  // Process Y-axis FFT
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = yRaw[i];
    vImag[i] = 0;
  }
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);
  for (int i = 0; i < SAMPLES/2; i++) yAmp[i] = vReal[i];
  
  // Process Z-axis FFT
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = zRaw[i];
    vImag[i] = 0;
  }
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);
  for (int i = 0; i < SAMPLES/2; i++) zAmp[i] = vReal[i];
}
