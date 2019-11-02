/* .Fish rises
    Fish waits a random time between 20 and 30 minutes
    Fish sinks
    Fish waits for a random time between 20 and 30 minutes

    Working
*/

// Enables
const int dirPin = 11;  // direction motor
const int pwmPin = 6;  // pwm pin for motor

unsigned long startSinkTime;
unsigned long startWaitBottomTime;
unsigned long startRiseTime;
unsigned long startWaitTopTime;

// motor speeds
int SINK_SPEED = 50;
int RISE_SPEED = 50;
// timing of states - you neeed to change these
const unsigned long SINK_TIME = 35000;
const unsigned long ASCEND_TIME = 40000;

const unsigned long WAIT_BOTTOM_TIME_MIN = 5000;
const unsigned long WAIT_BOTTOM_TIME_MAX = 10000;
const unsigned long WAIT_TOP_TIME_MIN = 5000;
const unsigned long WAIT_TOP_TIME_MAX = 10000;

// motor directions
const int SINK = 0;
const int RISE = 1;

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

  // Serial.print(RISE);

  if (!init) {
    startRiseTime = millis(); // time that ascend start - make sure this happens only once
    init = 1;
    Serial.print("sinking\t");
    Serial.println(SINK_TIME);
    digitalWrite(dirPin, SINK);
    analogWrite(pwmPin, SINK_SPEED);
  }

  if (millis() - startRiseTime > ASCEND_TIME) {
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
  unsigned long static topTime;


  if (!init) {
    startWaitTopTime = millis(); // time that ascend start - make sure this happens only once
    init = 1;
    topTime = random (WAIT_TOP_TIME_MIN, WAIT_TOP_TIME_MAX);
    Serial.print("wait on bottom\t");
    Serial.println(topTime);
    digitalWrite(dirPin, RISE);
    analogWrite(pwmPin, 0);
  }

  if (millis() - startWaitTopTime > topTime) {
    // time's up - all done
    init = 0;
    state = ASCENDING;
  }
}


//////////////////WATE ON SURFACE////////////////
void wait_on_surface() {
  int static init = 0;
  unsigned long static topTime;


  if (!init) {
    startWaitTopTime = millis(); // time that ascend start - make sure this happens only once
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
