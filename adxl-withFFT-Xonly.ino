


/* ****************************************************************************
 * Created By: Tauseef Ahmad
 * Create On: 8th January, 2022
 
 * How To Interface ADXL335 Analog Accelerometer Sensor With Arduino
 * Tutorial: https://youtu.be/HrIBGJep-1o
 * 
 *****************************************************************************/
// * YouTube Video: https://www.youtube.com/watch?v=C9efrWYrVVw
// this code is for live reporting, it doesn't log/save data

// add arduino fast fourier transform to measure frequency
#include <arduinoFFT.h>

//--------------------------------------------------------------
#define xPin A1
#define yPin A2
#define zPin A3
//--------------------------------------------------------------
//calibrated minimum and maximum Raw Ranges for each axis
//use calibration.ino file to get these values
// REPLACE LINES 23-30 WITH ACTUAL VALUES NOT ZERO****
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int xMin = 0;
int xMax = 0;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int yMin = 0;
int yMax = 0;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int zMin = 0;
int zMax = 0;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//--------------------------------------------------------------
// Take multiple samples to reduce noise
const int samples = 10;
//--------------------------------------------------------------
// FFT parameters
#define SAMPLES 64  // Must be power of 2
#define sampling_frequency 10000  // 10kHz sampling rate
#define sampling_period_us 100  // 100 microseconds for 10kHz
ArduinoFFT<double> FFT = ArduinoFFT<double>();
double vReal[SAMPLES];
double vImag[SAMPLES];
int vRealADC[SAMPLES];  // Array to store raw ADC values
//--------------------------------------------------------------

void setup() 
{
  //analogReference(EXTERNAL);
  Serial.begin(115200);
}

unsigned long previousMillis = 0;
const long sampleInterval = 100; // 100 ms (10 Hz)

void loop() 
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= sampleInterval) {
    previousMillis = currentMillis;
    
    //--------------------------------------------------------------
    //Read raw values
  int xRaw=0,yRaw=0,zRaw=0;
  for(int i=0;i<samples;i++)
  {
    xRaw+=analogRead(xPin);
    yRaw+=analogRead(yPin);
    zRaw+=analogRead(zPin);
  }
  xRaw/=samples;
  yRaw/=samples;
  zRaw/=samples;
  //--------------------------------------------------------------
  //Convert raw values to 'milli-Gs"
  //Convert value of RawMin to -1000
  //Convert value of RawMax to 1000
  long xMilliG = map(xRaw, xMin, xMax, -1000, 1000);
  long yMilliG = map(yRaw, yMin, yMax, -1000, 1000);
  long zMilliG = map(zRaw, zMin, zMax, -1000, 1000);
  //--------------------------------------------------------------
  // re-scale to fractional Gs
  float x_g_value = xMilliG / 1000.0;
  float y_g_value = yMilliG / 1000.0;
  float z_g_value = zMilliG / 1000.0;
  //--------------------------------------------------------------
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.print(x_g_value,3);
  //Serial.print(x_g_value,2);
  Serial.print("G");
  Serial.print("\t");
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.print(y_g_value,3);
  //Serial.print(y_g_value,2);
  Serial.print("G");
  Serial.print("\t");
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.print(z_g_value,3);
  //Serial.print(z_g_value,2);
  Serial.print("G");
  Serial.println();
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //--------------------------------------------------------------
  
  // Run FFT on all three axes
  runFFT();
  
  //--------------------------------------------------------------
  } // Close the if statement
}

//--------------------------------------------------------------
// FFT function for all three axes
void runFFT() {
  int rawX = 0;
  unsigned long microseconds;
  
  for (int i = 0; i < SAMPLES; i++)
    {
      microseconds = micros();
      rawX = analogRead(xPin);  // Use xPin instead of ADXL_XPin
      vRealADC[i] = rawX;
      vReal[i] = rawX;
      vImag[i] = 0;
      //wait for next sampling time according to freq
      while (micros() < (microseconds + sampling_period_us))  { }
    }

 
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);

  Serial.println("+++ Freq / Amplitude +++++");
  for(int i=0; i<(SAMPLES/2); i++)  {
      Serial.print((i * 1.0 * sampling_frequency) / SAMPLES, 1);
      Serial.print("\t");
      Serial.println(vReal[i]);    
  }

  Serial.println("*** ADC val ***");

  for(int i=0; i<(SAMPLES); i++)  {
      Serial.println(vRealADC[i]);    
  }
}
//--------------------------------------------------------------
