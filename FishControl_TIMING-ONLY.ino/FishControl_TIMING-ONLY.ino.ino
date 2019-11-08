/*********************** TIMING ONLY VERSION ************************
 ********************************************************************
 *  This version uses ONLY timeing to rise and sink the fish.       *
 *  If timming is not correct there is a possibility that too much, *
 *  or not enough slack will occure in the cable, rusulting in the  *
 *  fish not being pulled under watter, or never surfacing.         *
 ********************************************************************\
 
 /*   Fish rises
    Fish waits a random time between ? and ? minutes
    Fish sinks
    Fish waits for a random time between ? and ? minutes
*/

// Enables
const int dirPin = 11;  // direction motor
const int pwmPin = 6;  // pwm pin for motor



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

// motor directions
const int SINK = 0;
const int RISE = 1;

///////////////////// NOTHING TO SEE HERE //////////////////////
//////// No adjustments should be made behond this point /////// 
////////////////////////////////////////////////////////////////

int state;

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
  delay(2000);
}

void loop() {
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

  if (!init) {
    startSinkTime = millis(); // time that sink start - make sure this happens only once
    init = 1;
    Serial.print("sinking\t");
    Serial.println(SINK_TIME);
    digitalWrite(dirPin, SINK);
    analogWrite(pwmPin, SINK_SPEED);
  }

  if (millis() - startSinkTime > SINK_TIME) {
    // time's up - all done
    init = 0;
    state = WAIT_ON_BOTTOM;
    Serial.println("Sink time is done - Moving to WAIT_ON_BOTTOM");
    Serial.println("The fish should be resting at the bottom...");
  }
}
//////////////////ASCEND///////////////////
void raise_fish() {
  int static init = 0;

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
    Serial.println("Ascend time is done - Moving to WAIT_ON_TOP");
    Serial.println("The fish should be floating at the surface...");

  }
}


////////////////WAITE ON BOTTOM//////////////

void wait_on_bottom() {
  int static init = 0;
  unsigned long static bottomTime;


  if (!init) {
    startWaitBottomTime = millis(); // time that bottom waite start - make sure this happens only once
    init = 1;
    bottomTime = random (WAIT_BOTTOM_TIME_MIN, WAIT_BOTTOM_TIME_MAX);
    Serial.print("wait on bottom\t");
    Serial.println(bottomTime);
    digitalWrite(dirPin, SINK);
    analogWrite(pwmPin, 0);
  }

  if (millis() - startWaitBottomTime > bottomTime) {
    // time's up - all done
    init = 0;
    state = ASCENDING;
    Serial.println("Done waiting at the bottom - Moving to ASCENDING");
    Serial.println("The fish start be rising...");
  }
}


//////////////////WAIT ON SURFACE////////////////
void wait_on_surface() {
  int static init = 0;
  unsigned long static topTime;


  if (!init) {
    startWaitTopTime = millis(); // time that surface wait start - make sure this happens only once
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
    Serial.println("Done waiting at the bottom - Moving to SINKING");
    Serial.println("The fish start be sinking...");
  }
}
