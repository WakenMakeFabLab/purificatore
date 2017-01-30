
// LED pins
int value1, value2, value3, value4, value5 ;    // VALEU FOR LATER CALCULATION

//ARDUINO CONNECTION
int ledpin1 = 11;                           
int ledpin2 = 10;                           
int ledpin3 = 9;
int ledpin4 = 6;
int ledpin5 = 3;
long time=0;

// LED Fade timers
int periodeSlow = 5000;     // TIME OF THE SLOW WAVE IN MS
int periodeFast = 1500;     // TIME OF THE FAST WAVE IN MS

//DISPLACEMENT OF THE SLOW WAVE IN MS
int displace2Slow = 500;  
int displace3Slow = 1000;
int displace4Slow = 1500;
int displace5Slow = 2000;

//DISPLACEMENT OF THE FAST WAVE IN MS
int displace2Fast = 150;
int displace3Fast = 300;
int displace4Fast = 450;
int displace5Fast = 600;

// MQ135 gas sensor
float RoA = 10000.0;    // this has to be tuned 10K Ohm WILL INCRESE SENSOR SENSETIVITY
int MQ135 = 7;          // select the input pin for the sensor

int valA = 0;            // variable to store the value coming from the sensor
float VrlA = 0.0;       // variable FOR LATER CALCULATION
float RsA = 0.0;        // variable FOR LATER CALCULATION
float ratioA = 0.0;     // variable FOR LATER CALCULATION
float ppm = 0.0;        // variable FOR LATER CALCULATION

double result;          // variable FOR LATER CALCULATION
double rzero;           // variable FOR LATER CALCULATION

//RELAIS FAN SPEED
 int FanSlow = 4;       // RELAIS 1 pin
 int FanFast = 2;       // RELAIS 2 pin


//Air Qualiti index

int AQI = 0;            // variable FOR LATER CALCULATION
int AQIlast = 0;        // variable FOR LATER CALCULATION
  
  // SENSOR CALIBRATION
float CLEAN = 0.5;      // SENSOR TUNING CLEAN VALUE
float DIRTY = 1.0;      // SENSOR TUNING FAN SLOW VALUE
float GROSS = 2.0;      // SENSOR TUNING FAN FAST VALUE

// TIMERS 
int GROSStimer = 0;
int CLEANtimer = 0;
int x = 1;
int y = 0;
int i = 0;


void setup() {
  
  pinMode(FanSlow, OUTPUT);
  pinMode(FanFast, OUTPUT);


    
Serial.begin(9600);

}

// FINE TUNING SENSOR CALIBREATION 
// original CCalA = 250.6020682  * pow (ratioA, -2.769034857);   

float get_CalA (float ratioA){
  float CalA = 0.0;
  CalA = 250.6020682  * pow (ratioA, -2.769034857);
return CalA;
}

void loop() {

 time = millis();


//MQ135 Sensor valeu read
// get CO ppm

valA = analogRead(MQ135);     // read the value from the analog sensor
VrlA = valA * ( 5.00 / 1024.0  );      // V
RsA = 10000 * ( 5.00 - VrlA) / VrlA ;   // Ohm
ratioA =  RsA/RoA;  
ppm = (get_CalA(ratioA));

//AQI calculation

if ( ppm <= CLEAN){
  AQI = 1;
  CLEANtimer ++;
}

  if ( ppm > CLEAN && ppm < DIRTY ){
  AQI = AQIlast;
}

if ( ppm >= DIRTY && ppm <= (GROSS-0.5)){
  AQI = 2;
}

  if ( ppm > (GROSS-0.5) && ppm < GROSS ){
  AQI = AQIlast;
}

if ( ppm >= GROSS){
  AQI = 3;
  GROSStimer ++;
}




  if  (AQI == 1 && GROSStimer == 0){
    
    digitalWrite(FanSlow, LOW);
    digitalWrite(FanFast, LOW);
    
    value1 = 128+110*cos(2*PI/(periodeSlow)*-time);
    value2 = 128+115*cos(2*PI/periodeSlow*(displace2Slow-time));
    value3 = 128+125*cos(2*PI/periodeSlow*(displace3Slow-time));
    value4 = 128+125*cos(2*PI/periodeSlow*(displace4Slow-time));
    value5 = 128+125*cos(2*PI/periodeSlow*(displace5Slow-time));

    analogWrite(ledpin1, LOW);         
    analogWrite(ledpin2, LOW);          
    analogWrite(ledpin3, LOW);          
    analogWrite(ledpin4, LOW);
    analogWrite(ledpin5, LOW);
      
  }


  else if  (AQI == 2 && GROSStimer == 0){
    
    digitalWrite(FanSlow, HIGH);
    digitalWrite(FanFast, LOW);
  
    value1 = 128+110*cos(2*PI/(periodeSlow)*-time);
    value2 = 128+115*cos(2*PI/periodeSlow*(displace2Slow-time));
    value3 = 128+125*cos(2*PI/periodeSlow*(displace3Slow-time));
    value4 = 128+125*cos(2*PI/periodeSlow*(displace4Slow-time));
    value5 = 128+125*cos(2*PI/periodeSlow*(displace5Slow-time));
    
    analogWrite(ledpin1, value1);           // sets the value (range from 0 to 255)
    analogWrite(ledpin2, value2);           // sets the value (range from 0 to 255)
    analogWrite(ledpin3, value3);           // sets the value (range from 0 to 255)
    analogWrite(ledpin4, value4);           // sets the value (range from 0 to 255)
    analogWrite(ledpin5, value5);           // sets the value (range from 0 to 255)
    
  }

  
  else if  (AQI == 3 && GROSStimer == 0){

    GROSStimer ++;
    digitalWrite(FanSlow, LOW);
    digitalWrite(FanFast, HIGH);
    
    value1 = 128+110*cos(2*PI/(periodeFast)*-time);
    value2 = 128+115*cos(2*PI/periodeFast*(displace2Fast-time));
    value3 = 128+125*cos(2*PI/periodeFast*(displace3Fast-time));
    value4 = 128+125*cos(2*PI/periodeFast*(displace4Fast-time));
    value5 = 128+125*cos(2*PI/periodeFast*(displace5Fast-time));
    
    analogWrite(ledpin1, value1);           // sets the value (range from 0 to 255)
    analogWrite(ledpin2, value2);           // sets the value (range from 0 to 255)
    analogWrite(ledpin3, value3);           // sets the value (range from 0 to 255)
    analogWrite(ledpin4, value4);           // sets the value (range from 0 to 255)
    analogWrite(ledpin5, value5);           // sets the value (range from 0 to 255) 
  
  }

  else if  (GROSStimer > 0){

    GROSStimer ++;
    digitalWrite(FanSlow, LOW);
    digitalWrite(FanFast, HIGH);
    
    value1 = 128+110*cos(2*PI/(periodeFast)*-time);
    value2 = 128+115*cos(2*PI/periodeFast*(displace2Fast-time));
    value3 = 128+125*cos(2*PI/periodeFast*(displace3Fast-time));
    value4 = 128+125*cos(2*PI/periodeFast*(displace4Fast-time));
    value5 = 128+125*cos(2*PI/periodeFast*(displace5Fast-time));
    
    analogWrite(ledpin1, value1);           // sets the value (range from 0 to 255)
    analogWrite(ledpin2, value2);           // sets the value (range from 0 to 255)
    analogWrite(ledpin3, value3);           // sets the value (range from 0 to 255)
    analogWrite(ledpin4, value4);           // sets the value (range from 0 to 255)
    analogWrite(ledpin5, value5);           // sets the value (range from 0 to 255) 
  
  }

  else if ( AQI == 1 && GROSStimer == 0 && CLEANtimer <=5 && ppm <= 1){

  
   for (int y = 0; y > -1; y = y + x)
          analogWrite(ledpin1, y);           // sets the value (range from 0 to 255)
    analogWrite(ledpin2, y);           // sets the value (range from 0 to 255)
    analogWrite(ledpin3, y);           // sets the value (range from 0 to 255)
    analogWrite(ledpin4, y);           // sets the value (range from 0 to 255)
    analogWrite(ledpin5, y);           // sets the value (range from 0 to 255)
    
      if (i == 255) x = -1;             // switch direction at peak
      delay(10);
   

  }

  // TIMER FOR GROSS AIR 
 if ( GROSStimer >= 1000) {   
      GROSStimer = 0;
 }
  if ( CLEANtimer >= 1000) {
      CLEANtimer = 0;
 }


  valA = analogRead(0);       // read analog input pin 0

    Serial.print("| MW135 RAW\t"); 
    Serial.print(valA); 
    Serial.print("\t CO2 concentration\t"); 
    //Serial.print(ppm);
    Serial.print(get_CalA(ratioA));
    Serial.print("\tppm\t");
    Serial.print(AQI); 
    Serial.print("\t");
        Serial.print(GROSStimer); 
    Serial.print("\t");
            Serial.print(CLEANtimer); 
    Serial.print("\t");
    Serial.println();

 AQIlast = AQI;

 
 
 delay (10);
}
