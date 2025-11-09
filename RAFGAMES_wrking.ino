//Motor 2(Right)
int m1_in1 = 3;
int m1_in2 = 2;
int m1_en = 6;

//Motor 2(Right)
int m2_in1 = 4;
int m2_in2 = 7;
int m2_en = 5;

//Sensor pins
int sensor1 = 12;  //leftIR
int sensor2 = 11;  //right IR
//Ultrasonic Sensor
int trigPin = 10;
int echoPin = 8;
// Global variable outside function
enum State { LINE_FOLLOW, OBSTACLE_AVOID };
State currentState = LINE_FOLLOW;

void setup() {
  //Input Sensors
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  //Motor pins
  pinMode(m1_in1, OUTPUT);
  pinMode(m1_in2, OUTPUT);
  pinMode(m1_en, OUTPUT);
  pinMode(m2_in1, OUTPUT);
  pinMode(m2_in2, OUTPUT);
  pinMode(m2_en, OUTPUT);
  //Ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

//Motor functions
void moveMotor1(int speed) {
  if (speed > 0) {
    digitalWrite(m1_in1, HIGH);
    digitalWrite(m1_in2, LOW);
    analogWrite(m1_en, speed);
  } else if (speed < 0) {
    digitalWrite(m1_in1, LOW);
    digitalWrite(m1_in2, HIGH);
    analogWrite(m1_en, -speed);
  } else {
    analogWrite(m1_en, 0);
  }
}
void moveMotor2(int speed) {
  if (speed > 0) {
    digitalWrite(m2_in1, HIGH);
    digitalWrite(m2_in2, LOW);
    analogWrite(m2_en, speed);
  } else if (speed < 0) {
    digitalWrite(m2_in1, LOW);
    digitalWrite(m2_in2, HIGH);
    analogWrite(m2_en, -speed);
  } else {
    analogWrite(m2_en, 0);
  }
}
void stopMotors() {
  moveMotor1(0);
  moveMotor2(0);
}

long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // ADD TIMEOUT - this is the critical fix
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout  
  long distance = duration * 0.034 / 2;
  return distance; 
}
//Obstacle Avoidance
void avoidObstacles() {
  long distance = getDistanceCM();
  if (distance > 0 && distance < 15) {
    //if obs is closer than 15cm
    stopMotors();
    delay(200);

    //back up
    moveMotor1(-150);
    moveMotor2(-150);
    delay(400);

    //Turn right
    moveMotor1(150);
    moveMotor2(-150);
    delay(400);

    stopMotors();
  }
}
void lineFollow() {

  if (digitalRead(sensor1) && digitalRead(sensor2)) {
        // Both sensors detect line - stop or special case
        stopMotors();
        Serial.println("Both on line - STOP");
      }
      else if ((digitalRead(sensor1) == 0) && (digitalRead(sensor2) == 0)) {
        // Both sensors off line - go straight
        moveMotor1(90);
        moveMotor2(90);
        Serial.println("Both off line - FORWARD");
      }
      else if ((digitalRead(sensor1) == 0) && (digitalRead(sensor2) == 1)) {
        // Left off line, right on line - turn right
        moveMotor1(-100);
        moveMotor2(100);
        Serial.println("Left off, right on - TURN RIGHT");
      }
      else if ((digitalRead(sensor1) == 1) && (digitalRead(sensor2) == 0)) {
        // Left on line, right off line - turn left
        moveMotor1(100);
        moveMotor2(-100);
      }
}
void allBlack() {
  if (digitalRead(sensor1) && digitalRead(sensor2)) {
    stopMotors();
  }
}
void allWhite() {
  if ((digitalRead(sensor1) == 0) && (digitalRead(sensor2) == 0)) {
    moveMotor2(140);
    moveMotor1(140);
  }
}
void rightBlack() {
  if ((digitalRead(sensor1) == 0) && (digitalRead(sensor2) == 1)) {
    moveMotor1(206);
    moveMotor2(-204);
  }
}
void leftBlack() {
  if ((digitalRead(sensor1) == 1) && (digitalRead(sensor2) == 0)) {
    moveMotor1(-204);
    moveMotor2(204);
  }
}
void turnRight(int cdelay) {
  moveMotor1(100);
  moveMotor2(-164);
  delay(cdelay);
}
void turnLeft(int cdelay) {
  moveMotor1(-100);
  moveMotor2(164);
  delay(cdelay);
}
void setMotors(int lspeed, int rspeed) {
  moveMotor1(lspeed);
  moveMotor2(rspeed);
}
void turnSetMotors(int lspeed, int rspeed, int ddelay) {
  moveMotor1(lspeed);
  moveMotor2(rspeed);
  delay(ddelay);
}
void forward();

void D_torDash(){ 
  long distance = getDistanceCM();
  // Manual declaration for avoidance direction (as requested)
  static bool avoidedRight = false;
  static bool avoidedLeft = false;
  // Debug output
  Serial.print("State: ");
  Serial.print(currentState == LINE_FOLLOW ? "LINE_FOLLOW" : "OBSTACLE_AVOID");
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.print("cm | Sensors: ");
  Serial.print(digitalRead(sensor1));
  Serial.print(",");
  Serial.println(digitalRead(sensor2));

  switch (currentState) {
    case LINE_FOLLOW:
      // Check for obstacle FIRST - this overrides line following
      if (distance > 0 && distance < 7) {
        Serial.println("🛑 OBSTACLE DETECTED - Overriding line follow!");
        stopMotors();
        delay(500);
        currentState = OBSTACLE_AVOID;
        break; // Exit switch case immediately
      }
      
      // Only execute line following if no obstacle detected
      if (digitalRead(sensor1) && digitalRead(sensor2)) {
        // Both sensors detect line - stop or special case
        stopMotors();
        Serial.println("Both on line - STOP");
      }
      else if ((digitalRead(sensor1) == 0) && (digitalRead(sensor2) == 0)) {
        // Both sensors off line - go straight
        moveMotor1(90);
        moveMotor2(90);
        Serial.println("Both off line - FORWARD");
      }
      else if ((digitalRead(sensor1) == 0) && (digitalRead(sensor2) == 1)) {
        // Left off line, right on line - turn right
        moveMotor1(-100);
        moveMotor2(100);
        Serial.println("Left off, right on - TURN RIGHT");
      }
      else if ((digitalRead(sensor1) == 1) && (digitalRead(sensor2) == 0)) {
        // Left on line, right off line - turn left
        moveMotor1(100);
        moveMotor2(-100);
        Serial.println("Left on, right off - TURN LEFT");
      }
      break;

    case OBSTACLE_AVOID:
      Serial.println("🚧 OBSTACLE AVOIDANCE ACTIVE");
      
      // Step 1: Stop and check both sides
      stopMotors();
      delay(1000);
      
      // Step 2: First check right side
      Serial.println("Checking RIGHT side...");
      turnRight(300); // Turn jabout 40 degrees right
      stopMotors();
      delay(500);
      
      distance = getDistanceCM();
      Serial.print("Right side distance: ");
      Serial.println(distance);
      
      if (distance > 0 && distance < 7) {
        // Right side blocked, check left side
        Serial.println("❌ Right side blocked, checking LEFT side...");
        stopMotors();
        delay(1000);
        turnLeft(500); // Turn back to center then left
        stopMotors();
        delay(500);
        
        distance = getDistanceCM();
        Serial.print("Left side distance: ");
        Serial.println(distance);
        
        if (distance > 0 && distance < 7) {
         avoidedRight = false;
         avoidedLeft = false;
         stopMotors();
        } else {
          // Left side clear - go left
          Serial.println("✅ Left side clear! Avoiding via LEFT...");
          avoidedRight = false;
          avoidedLeft = true;
          forward(90);   // Move forward past obstacle
          delay(900);
          turnRight(100); // Turn to pass obstacle by moving forward
          forward(90);
          delay(900);
          turnRight(120);
          forward(70);
//          delay(900);
        }
      } else {
        // Right side clear - go right
        Serial.println("✅ Right side clear! Avoiding via RIGHT...");
        avoidedRight = true;
        avoidedLeft = false;
        forward(90);    // Move forward past obstacle
        delay(900);
        turnLeft(100);  // Turn to pass obstacle by moving forward
        forward(90);
        delay(900);
        turnLeft(120);
        forward(70);
//        delay(900);
      }
      
      // Step 3: Search for line to resume following
      Serial.println("🔍 Searching for line...");
      unsigned long searchStartTime = millis();
      bool lineFound = false;
      
      while (millis() - searchStartTime < 6000) { // 6 second timeout
        bool s1 = digitalRead(sensor1);
        bool s2 = digitalRead(sensor2);
        
        // NEW: Move forward until line detected instead of fixed delay
    Serial.println("🔍 Moving toward line until detection...");
    unsigned long searchTime = millis();
    while (digitalRead(sensor1) == 0 && digitalRead(sensor2) == 0 && millis() - searchTime < 5000) {
      forward(60);
    }
    stopMotors();
    delay(900);
  }
    
      // Step 4: PRECISE LINE ALIGNMENT LOGIC
      Serial.println("🎯 Starting precise line alignment...");
      preciseLineAlignment(avoidedRight, avoidedLeft);
      
      // Return to line following\
      Serial.println("🔄 Returning to LINE FOLLOWING");
      currentState = LINE_FOLLOW;
      break;
  }
}
// PRECISE LINE ALIGNMENT — ONLY WHEN BOTH SENSORS ON LINE
void preciseLineAlignment(bool avoidedRight, bool avoidedLeft) {
  Serial.println("🔧 Checking for both sensors on line...");

  // Only act if BOTH sensors detect the line
  if (digitalRead(sensor1) == 1 && digitalRead(sensor2) == 1) {
    Serial.println("🎯 Both sensors on line. Performing small correction...");

    if (avoidedLeft) {
      // Bot avoided via right path → needs small LEFT turn
      Serial.println("🔄 Avoided left → small LEFT turn...");
      moveMotor1(-60);   // Left motor backward
      moveMotor2(60);    // Right motor forward
      delay(50);
    } 
    else if (avoidedRight) {
      // Bot avoided via left path → needs small RIGHT turn
      Serial.println("🔄 Avoided right → small RIGHT turn...");
      moveMotor1(60);    // Left motor forward
      moveMotor2(-60);   // Right motor backward
      delay(50);
    } 
    else {
      Serial.println("⚠ No avoidance flag set. Skipping correction.");
    }

    stopMotors();
    delay(100);
    Serial.println("✅ Correction done. Ready for line following.");
  } 
  else {
    // Any other sensor state → do nothing
    Serial.println("⚠ Not both sensors on line. Returning without action.");
    return;
  }
}
void reverse(int rdelay, int hspeed) {
  moveMotor1(hspeed);
  moveMotor2(hspeed);
  delay(rdelay);
}
void forward(int fspeed) {
  moveMotor2(fspeed);
  moveMotor1(fspeed);
}
void custom_obstacleD_tor() {
  getDistanceCM();
  long distance = getDistanceCM();
  if (distance > 0 && distance < 17) {
    //if obstacle is closer than 17cm
    turnRight(100);
    if (distance > 0 && distance < 17) {
      turnLeft(200);
      forward(150);
      turnRight(200);
      forward(100);
      delay(1500);
      turnRight(200);
      while (digitalRead(sensor1) == 0 && digitalRead(sensor2 == 0)) {
        forward(100);
      }
      if (digitalRead(sensor1) || digitalRead(sensor2)) {
        forward(100);
        delay(180);
        stopMotors();
        delay(100);
        turnLeft(100);
        stopMotors();
        delay(100);
      }
    }
    delay(200);
    forward(100);
    delay(1500);
    turnLeft(200);
    while (digitalRead(sensor1) == 0 && digitalRead(sensor2) == 0) {
      forward(100);
    }
    if (digitalRead(sensor1) || digitalRead(sensor2)) {
      forward(100);
      delay(180);
      stopMotors();
      delay(100);
      turnRight(100);
      stopMotors();
      delay(100);
    }
  }
}
void rotate360Slow(int fspeed, int rspeed) {  //rpeed mostly zero for right spin
  long distance = getDistanceCM();
  while (distance > 15) {
    moveMotor1(fspeed);
    moveMotor2(rspeed);
    delay(700);
    long distance = getDistanceCM();
    break;
  }
}
void MatadorRun() {
  //Robot States
  enum State { CALM,
               CHASE,
               SEARCH };      //defining all possible modes
  State currentState = CALM;  //start in CALM mode
  long distance = getDistanceCM();

  switch (currentState) {
    //Calm Mode
    case CALM:
      long distance = getDistanceCM();
      if (distance < 15) {  //Target spotted within 15cm range
        currentState = CHASE;
      } else {
        lineFollow();
      }
      break;

    //CHASE Mode
    case CHASE:
     distance = getDistanceCM();
      if (distance < 15 && distance > 4.5) {
        forward(91);
      } else if (distance < 4.5) {
        stopMotors();
        delay(1000);//just an approximated delay before going back to search
      } else {
        currentState = SEARCH;
      }
      break;
    case SEARCH:
      for (int i = 0; i < 20; i++) {
        //trying 20 small rotations by giving custom speed to rotateSlow Func, and delay()use
        rotate360Slow(81, -6);
       distance = getDistanceCM();
        if (distance < 15) {  //Target found again
          currentState = CHASE;
          break;
        }
      }
      //if still no target, return to line
      if (currentState == SEARCH) {
        while (true) {
          forward(100);
          //initial one to search for line but since tape for,s a regular figure,a normal forward will still end the bot up on the line
          //rotate360Slow(81,-6);//Small steps search for the line
          if (digitalRead(sensor1) and digitalRead(sensor2)) {
            currentState = CALM;
            //Found line again->Calm mode
            while (digitalRead(sensor1) || digitalRead(sensor2)) {
              stopMotors();
              forward(80);
              delay(180);
              stopMotors();
              delay(100);
              turnSetMotors(80,-6,100);
              stopMotors();
              delay(100);
            }
            break;
          }
        }
      }
      break;
  }
}
void sniffy() {
  long distance = getDistanceCM();
  enum State { FOLLOW,
               PAUSE,
               SEARCH,
               WAIT };
  State currentState = FOLLOW;
  switch (currentState) {
    //FOLLOW mode
    case FOLLOW:
      if ((distance > 4.5) && (distance < 15)) {
        forward(90);
      } else if (distance > 15) {
        stopMotors();
        currentState = PAUSE;
      }
      break;
      //PAUSE mode
    case PAUSE:
      stopMotors();
      delay(2000);
      if (distance < 15) {
        currentState = SEARCH;
      } else {
        currentState = FOLLOW;
      }
      break;
      //SEARCH mode
    case SEARCH:
      for (int i = 0; i < 20; i++) {
        //trying 20 small rotations by giving custom speed to rotateSlow Func, and delay()use
        rotate360Slow(81, -6);
        if (distance < 15) {  //Target found again
          currentState = FOLLOW;
          break;
        } else {
          stopMotors();
          currentState = WAIT;
        }
        break;
      }
    case WAIT:
      stopMotors();
      if (distance < 15) {
        currentState = FOLLOW;
      }
  }
}
void reverse2Junc() {
  while ((digitalRead(sensor1) != 1) && (digitalRead(sensor2) != 1)) {
    moveMotor1(-150);
    moveMotor2(-150);
  }
  stopMotors();
}
void crossRoad_Quest() {
  long distance = getDistanceCM();
  enum State { LINE_FOLLOW,
               AT_JUNCTION,
               FOLLOW_PATH,
               OBSTACLE_FOUND,
               FINISH,
               STOPPED };
  State currentState = LINE_FOLLOW;
  int counterUp = 0;
  int counterRev = 0;
  switch (currentState) {
    case LINE_FOLLOW:
      lineFollow();
      if ((digitalRead(sensor1) == 1) && (digitalRead(sensor2) == 1)) {
        counterUp++;
        currentState = AT_JUNCTION;
      }
      break;
    case AT_JUNCTION:
      if (counterRev == 0) {
        turnLeft(20);
        while (digitalRead(sensor2) == 0) {
          turnLeft(0);  //gotta confirm with delay
        }
        stopMotors;
      } else if (counterRev == 1) {
        turnRight(20);
        while (digitalRead(sensor1) == 0) {
          turnRight(0);  //gotta confirm with delay;
        }
        stopMotors();
      } else if (counterRev == 2) {
        turnRight(20);
        while (digitalRead(sensor1) == 0) {
          turnRight(0);  //gotta confrim with delay;
        }
        stopMotors;
      }
      currentState = FOLLOW_PATH;
      break;
    case FOLLOW_PATH:
      lineFollow();
      if ((distance > 1.5) && (distance < 6.8)) {
        stopMotors();
        reverse2Junc();
        counterRev++;
        currentState = AT_JUNCTION;
      } else if ((digitalRead(sensor1) == 1) && (digitalRead(sensor2) == 1)) {
        stopMotors();
        counterUp++;
        currentState = STOPPED;
      }
      break;
    case STOPPED:
      stopMotors();
      break;
  }
}
void boundRush() {
  long distance = getDistanceCM();
  enum State { FORWARD,
               AVOID_LINE,
               AVOID_OBS };
  State currentState = FORWARD;
  switch (currentState) {
    case FORWARD:
      forward(100);
      if ((digitalRead(sensor1) == 1) && (digitalRead(sensor2) == 1)) {
        currentState = AVOID_LINE;
      } else if (distance < 15) {
        currentState = AVOID_OBS;
      }
      break;
    case AVOID_LINE:
      reverse(500, -150);
      reverse_delay(-206, -50, 200);
      currentState = FORWARD;
      break;
    case AVOID_OBS:
      reverse(500, -150);
      reverse_delay(-206, -50, 200);
      currentState = FORWARD;
      break;
  }
}
void reverse_delay(int lspeed, int rspeed, int rdelay) {
  moveMoto