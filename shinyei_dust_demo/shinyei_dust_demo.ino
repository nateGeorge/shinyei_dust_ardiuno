/* 
Grove - Dust Sensor Demo v2.0
Interface to Shinyei Model PPD42NS Particle Sensor
Program originally by Christopher Nafis 
Written April 2012
Updated by Nate George
March 2015

http://www.seeedstudio.com/depot/grove-dust-sensor-p-1050.html
http://www.sca-shinyei.com/pdf/PPD42NS.pdf
http://www.howmuchsnow.com/arduino/airquality/grovedust/


JST Pin 1 (Black Wire)  => Arduino GND
JST Pin 3 (Red wire)    => Arduino 5VDC
JST Pin 4 (Yellow wire) => Arduino Digital Pin 8
 */


int dustSignal = 8;
unsigned long lowDuration;
unsigned long highDuration;
unsigned long startTime_ms;
unsigned long sampleTime_ms = 60000;
// you can use any sample time, but seems like 30s gets a lot of 0s for lowPulseOccupancy...
unsigned long lowPulseOccupancy = 0;
float lowRatio = 0;
float concentration = 0;

void setup() {
  Serial.begin(9600);
  pinMode(dustSignal,INPUT);
  startTime_ms = millis();// get the current time
}

void loop() {
  // dust sensor outputs low voltage when dust is sensed
  lowDuration = pulseIn(dustSignal, LOW);
  lowPulseOccupancy += lowDuration;

  if ((millis()-startTime_ms) > sampleTime_ms)
  // if the sample time has been surpassed, update dust concentration result
  {
    lowRatio = lowPulseOccupancy/(sampleTime_ms*10.0);  // pulseIn (duration, lowplusInteger) is in microseconds, 
    // so divide by 1000, multiply by 100 for percentage of time dust was sensed (0=>100),
    // and that's where the 10.0 in the denominator comes from.
    
    // calculate concentration of particles > 1 micron in diameter per 0.01 ft3 using spec sheet curve
    concentration = 1.1*pow(lowRatio,3)-3.8*pow(lowRatio,2)+520*lowRatio;
    // removed the '+ 0.62' from the equation, because at 0 loPulseOccupancy, dust concentration should be 0.
    
    Serial.print("lowPulseOccupancy : ");
    Serial.println(lowPulseOccupancy);
    Serial.print("lowRatio : ");
    Serial.println(lowRatio);
    Serial.print("concentration : ");
    Serial.println(concentration);
    Serial.print("\n"); // this extra line break is for readability in the serial monitor
    
    // reset timing variables
    lowPulseOccupancy = 0.0;
    startTime_ms = millis();
  }
  
   
  
}
