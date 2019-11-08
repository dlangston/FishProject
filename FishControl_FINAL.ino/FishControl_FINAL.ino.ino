/*  Fish rises
    Fish waits a random time between ? and ? minutes
    Fish sinks
    Fish waits for a random time between ? and ? minutes
    Working

  Disaster switch is wired with ground and a digitalPin


*/

// Enables
const int dirPin = 11;  // direction motor
const int pwmPin = 6;  // pwm pin for motor
const int currentSensPin = A0;  // current sensor signal pin
const int disasterPin = 2; // runaway fish pin


unsigned long startSinkTime;
unsigned long startWaitBottomTime;
unsigned long startRiseTime;
unsigned long startWaitTopTime;

// Motor speeds - Adjust this for how fast the fish rises and sinks(PWM)
int SINK_SPEED = 50;
int RISE_SPEED = 50;

// Timing of states - adjust these for the how long it takes for the fish to rise and sink
// *NOTE: ASCEND_TIME is a backup to the current sensor
const unsigned long SINK_TIME = 120000;
const unsigned long ASCEND_TIME = 70000;

// MIN and MAX times for random waits
// EXAMPLE:   
// A minimume of 5000 = 5 seconds and a maximue of 10000 = 10 seconds
// This means the time will be anywere between 5 secons and 10 secons
const unsigned long WAIT_BOTTOM_TIME_MIN = 60000; 
const unsigned long WAIT_BOTTOM_TIME_MAX = 61000;
const unsigned long WAIT_TOP_TIME_MIN = 180000;
const unsigned long WAIT_TOP_TIME_MAX = 181000;

// Empirical curent value
const int lockedFishADC = 550;  // Adjust this value to stop the sink_fish function

// Motor directions - Swap these if it goes the wrong way at startup - or you can reverse the motor wires
const int SINK = 0;
const int RISE = 1;

///////////////////// NOTHING TO SEE HERE //////////////////////
//////// No adjustments should be made behond this point /////// 
////////////////////////////////////////////////////////////////
int state;
int currentADC; // current sensor reading

#define SINKING 0
#define WAIT_ON_BOTTOM 1
#define ASCENDING 2
#define WAIT_ON_SURFACE 3

// state definitions
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  state = SINKING;
  pinMode(disasterPin, INPUT_PULLUP);
  delay(2000);
}

void loop() {

  readCurrentSensor();
  // Disaster switch to stop program 
//  checkDisasterSwitch();  // uncomment to add disaster sensor

  switch (state)
  {
    case SINKING:
      sink_fish();
      break;
    case WAIT_ON_BOTTOM:
      wait_on_bottom();
      break;
    case ASCENDING: // code to be executed if n = 1;
      raise_fish();
      break;
    case WAIT_ON_SURFACE: // code to be executed if n = 1;
      wait_on_surface();
      break;
  }
}


//////////////SINK/////////////////

void sink_fish() {
  int static init = 0;

  // Serial.print(RISE);

  if (!init) {
    startRiseTime = millis(); // time that ascend start - make sure this happens only once
    init = 1;
    Serial.print("sinking\t");
    Serial.println(SINK_TIME);
    digitalWrite(dirPin, SINK);
    analogWrite(pwmPin, SINK_SPEED);
  }

  if (currentADC > lockedFishADC) { // cable stop against the bottom weight
    // cut off drive and go to next state
    init = 0;
    state = WAIT_ON_BOTTOM;
    Serial.println("bail out - fish is locked on bottom");

  }

  if (millis() - startRiseTime > SINK_TIME) {
    // time's up - all done
    init = 0;
    state = WAIT_ON_BOTTOM;
  }
}

//////////////////ASCEND///////////////////
void raise_fish() {
  int static init = 0;

  //    Serial.print(RISE);

  if (!init) {
    startRiseTime = millis(); // time that ascend start - make sure this happens only once
    init = 1;
    Serial.print("ascending\t");
    Serial.println(ASCEND_TIME);
    digitalWrite(dirPin, RISE);
    analogWrite(pwmPin, RISE_SPEED);
  }

  if (millis() - startRiseTime > ASCEND_TIME) {
    // time's up - all done
    init = 0;
    state = WAIT_ON_SURFACE;
  }
}


////////////////WAITE ON BOTTOM//////////////

void wait_on_bottom() {
  int static init = 0;
  unsigned long static bottomTime;

  if (!init) {
    startWaitBottomTime = millis(); // time that ascend start - make sure this happens only once
    init = 1;
    bottomTime = random ( WAIT_BOTTOM_TIME_MIN,  WAIT_BOTTOM_TIME_MAX);
    Serial.print("wait on bottom\t");
    Serial.println(bottomTime);
    digitalWrite(dirPin, RISE);
    analogWrite(pwmPin, 0);
  }

  if (millis() - startWaitBottomTime > bottomTime) {
    // time's up - all done
    init = 0;
    state = ASCENDING;
  }
}

//////////////////WAIT ON SURFACE////////////////
void wait_on_surface() {
  int static init = 0;
  unsigned long static topTime;

  if (!init) {
    startWaitTopTime = millis(); // time that ascend start
    init = 1;
    topTime = random (WAIT_TOP_TIME_MIN, WAIT_TOP_TIME_MAX);
    Serial.print("wait on surface\t");
    Serial.println(topTime);
    digitalWrite(dirPin, RISE);
    analogWrite(pwmPin, 0);
  }

  if (millis() - startWaitTopTime > topTime) {
    // time's up - all done
    init = 0;
    state = SINKING;
  }
}

void readCurrentSensor() {
  static unsigned long lastPrintMillis;
  int rawADC = analogRead(currentSensPin);
  static float smoothedVal;
  static float filterVal = 0.97;

  smoothedVal = smooth((float)rawADC, filterVal, smoothedVal);
  currentADC  = smoothedVal;

  if (millis() - lastPrintMillis > 1000) { // controls Serial debugging of current value
    Serial.print(rawADC);
    Serial.print("\t");
    Serial.println(smoothedVal);
    lastPrintMillis = millis();
  }
}

void checkDisasterSwitch() {
  if (!digitalRead(disasterPin)) {  // wire for normally open switch
    analogWrite(pwmPin, 0);
    while (1) {
      Serial.println("Disaster sensor is triggered!");
      delay(1000);
    }
  }
}


float smooth(float data, float filterVal, float smoothedVal) {
  if (filterVal > 1.0) {     // check to make sure param's are within range
    filterVal = .999;
  }
  else if (filterVal <= 0) {
    filterVal = 0;
  }
  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return smoothedVal;
}
