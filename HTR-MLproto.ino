
#define RIGHT_MOTOR_FORWARD_PIN 3
#define LEFT_MOTOR_FORWARD_PIN 9
#define RIGHT_MOTOR_REVERSE_PIN 5
#define LEFT_MOTOR_REVERSE_PIN 6

#define LEFT_BUMPER_PIN 10
#define RIGHT_BUMPER_PIN 11

#define SHORT_DURATION 100
#define LONG_DURATION 500

#define NOT_GOING_FORWARD_THRESHOLD 1250
#define BUMPER_HIT_THRESHOLD 1500

bool LeftBumperStatus = 0;
bool RightBumperStatus = 0;
byte BumperBumped = 0;
byte LastBumperBumped = 0;

byte LastActionPerformed = 0;

byte MEMORY[11][2][2] = {0};  //LastActionPerformed, LeftBumperStatus, RightBumperStatus
byte PreviousMemoryAction = 0;
bool PreviousLeftBumperStatus = 0;
bool PreviousRightBumperStatus = 0;

bool BadRobot = false;

unsigned long NotGoingForwardTimer = 0;
unsigned long BumperHitTimer = 0;

void PerformAction(int action)
{
  BadRobot = false;
  LastActionPerformed = action;

  switch (action) 
  {
    case 0:
      Resume();
      break;

    case 1:
      SpinLeft(SHORT_DURATION);
      break;

    case 2:
      SpinRight(SHORT_DURATION);
      break;

    case 3:
      BackLeft(SHORT_DURATION);
      break;

    case 4:
      BackRight(SHORT_DURATION);
      break;

    case 5:
      SpinLeft(LONG_DURATION);
      break;

    case 6:
      SpinRight(LONG_DURATION);
      break;

    case 7:
      BackLeft(LONG_DURATION);
      break;

    case 8:
      BackRight(LONG_DURATION);
      break;

    case 9:
      Backup(SHORT_DURATION);
      break;
      
    case 10:
      Backup(LONG_DURATION);
      break;
  }
}

//MOVEMENT Functions
void MoveMotors(int Duration, bool right_forward, bool left_forward, bool right_reverse, bool left_reverse)
{
  //Don't try to spin both directions at the same time
  if ((right_forward == right_reverse) && (right_forward == true))
    return;
  if ((left_forward == left_reverse) && (left_forward == true))
    return;
  
  digitalWrite(RIGHT_MOTOR_FORWARD_PIN, right_forward);    //Right Motor Forward Pin
  digitalWrite(LEFT_MOTOR_FORWARD_PIN, left_forward);    //Left Motor Forward Pin
  digitalWrite(RIGHT_MOTOR_REVERSE_PIN, right_reverse);    //Right Motor Reverse Pin
  digitalWrite(LEFT_MOTOR_REVERSE_PIN, left_reverse);    //Left Motor Reverse Pin

  delay(Duration);
}

void SpinLeft(int Duration)
{
  MoveMotors(Duration, 1, 0, 0, 1);
}

void SpinRight(int Duration)
{
  MoveMotors(Duration, 0, 1, 1, 0);
}

void BackRight(int Duration)
{
  MoveMotors(Duration, 0, 1, 0, 1);
}

void BackLeft(int Duration)
{
  MoveMotors(Duration, 0, 0, 0, 1);
}

void Backup(int Duration)
{
  MoveMotors(Duration, 0, 0, 1, 1);
}

void Resume()
{
  MoveMotors(0, 1, 1, 0, 0);
  NotGoingForwardTimer = millis();
}

void UpdateBumperStatus()
{
  LeftBumperStatus = digitalRead(LEFT_BUMPER_PIN);
  RightBumperStatus = digitalRead(RIGHT_BUMPER_PIN);

  if ((LeftBumperStatus == 1) && (RightBumperStatus == 1))
    //Are we crashed into a wall head on?
    BumperBumped = 3;
  else if (LeftBumperStatus == 1)
    BumperBumped = 0;
  else if (RightBumperStatus == 1)
    BumperBumped = 1;
}

void IsBadRobot()
{
  //Bad Robot Conditions
  if (millis() - NotGoingForwardTimer > NOT_GOING_FORWARD_THRESHOLD)
    BadRobot = true;
  
  if (LeftBumperStatus == 1 || RightBumperStatus == 1)
  {
    if (millis() - BumperHitTimer < BUMPER_HIT_THRESHOLD)
      if (BumperBumped == LastBumperBumped)
        BadRobot = true;

    BumperHitTimer = millis();

    LastBumperBumped = BumperBumped;
  }

  if (PreviousMemoryAction == 0 && LeftBumperStatus == 0 && RightBumperStatus == 0 && MEMORY[LastActionPerformed][LeftBumperStatus][RightBumperStatus] != 0)
    BadRobot = true;
}

void Learn()
{
  //Learning
  if (BadRobot)
    MEMORY[PreviousMemoryAction][PreviousLeftBumperStatus][PreviousRightBumperStatus] = random(0, 10);

  //Select Next Action 
  PerformAction(MEMORY[LastActionPerformed][LeftBumperStatus][RightBumperStatus]);

  //Update Previous Actions
  PreviousMemoryAction = LastActionPerformed;
  PreviousLeftBumperStatus = LeftBumperStatus;
  PreviousRightBumperStatus = RightBumperStatus;
}

void OutputStats()
{
  Serial.print(LeftBumperStatus);
  Serial.print(" X ");
  Serial.print(RightBumperStatus);
  Serial.print(" BR:");
  Serial.print(BadRobot);
  Serial.print(" MEM:");
  Serial.println(MEMORY[LastActionPerformed][LeftBumperStatus][RightBumperStatus]);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(RIGHT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_REVERSE_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_REVERSE_PIN, OUTPUT);

  pinMode(LEFT_BUMPER_PIN, INPUT);
  pinMode(RIGHT_BUMPER_PIN, INPUT);
}

void loop() 
{
  UpdateBumperStatus();
  IsBadRobot();
  OutputStats();
  Learn();
}
