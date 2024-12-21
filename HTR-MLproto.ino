
int LeftBumperStatus = 0;
int RightBumperStatus = 0;
int BumperBumped = 0;
int LastBumperBumped = 0;

int LastActionPerformed = 0;

int MEMORY[11][2][2] = {0};  //LastActionPerformed, LeftBumperStatus, RightBumperStatus
int PreviousMemoryAction = 0;
int PreviousLeftBumperStatus = 0;
int PreviousRightBumperStatus = 0;

bool BadRobot = false;
bool ResumeState = true;

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
      ResumeState = true;
      break;

    case 1:
      SpinLeft(100);
      break;

    case 2:
      SpinRight(100);
      break;

    case 3:
      BackLeft(100);
      break;

    case 4:
      BackRight(100);
      break;

    case 5:
      SpinLeft(500);
      break;

    case 6:
      SpinRight(500);
      break;

    case 7:
      BackLeft(500);
      break;

    case 8:
      BackRight(500);
      break;

    case 9:
      Backup(100);
      break;
      
    case 10:
      Backup(500);
      break;
  }
}
//MOVEMENT Functions
void SpinLeft(int Duration)
{
  digitalWrite(3,1);    //Right Motor Forward Pin
  digitalWrite(9,0);    //Left Motor Forward Pin
  digitalWrite(5,0);    //Right Motor Reverse Pin
  digitalWrite(6,1);    //Left Motor Reverse Pin

  delay(Duration);
}

void SpinRight(int Duration)
{
  digitalWrite(3,0);    //Right Motor Forward Pin
  digitalWrite(9,1);    //Left Motor Forward Pin
  digitalWrite(5,1);    //Right Motor Reverse Pin
  digitalWrite(6,0);    //Left Motor Reverse Pin

  delay(Duration);
}

void BackRight(int Duration)
{
  digitalWrite(3,0);    //Right Motor Forward Pin
  digitalWrite(9,0);    //Left Motor Forward Pin
  digitalWrite(5,1);    //Right Motor Reverse Pin
  digitalWrite(6,0);    //Left Motor Reverse Pin

  delay(Duration);
}

void BackLeft(int Duration)
{
  digitalWrite(3,0);    //Right Motor Forward Pin
  digitalWrite(9,0);    //Left Motor Forward Pin
  digitalWrite(5,0);    //Right Motor Reverse Pin
  digitalWrite(6,1);    //Left Motor Reverse Pin

  delay(Duration);
}

void Backup(int Duration)
{
  digitalWrite(3,0);    //Right Motor Forward Pin
  digitalWrite(9,0);    //Left Motor Forward Pin
  digitalWrite(5,1);    //Right Motor Reverse Pin
  digitalWrite(6,1);    //Left Motor Reverse Pin

  delay(Duration);
}

void Resume()
{
  digitalWrite(3,1);    //Right Motor Forward Pin
  digitalWrite(9,1);    //Left Motor Forward Pin
  digitalWrite(5,0);    //Right Motor Reverse Pin
  digitalWrite(6,0);    //Left Motor Reverse Pin

  NotGoingForwardTimer = millis();
}

void setup() 
{
  Serial.begin(9600);
  pinMode (3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(10, INPUT);
  pinMode(11, INPUT);

}

void loop() 
{
  LeftBumperStatus = digitalRead(10);
  RightBumperStatus = digitalRead(11);

  if (LeftBumperStatus == 1)
    BumperBumped = 0;

  if (RightBumperStatus == 1)
    BumperBumped = 1;

  //Bad Robot Conditions

  if (millis() - NotGoingForwardTimer > 1250)
    BadRobot = true;
  
  if (LeftBumperStatus == 1 || RightBumperStatus == 1)
  {
    if (millis() - BumperHitTimer < 1500)
      if (BumperBumped == LastBumperBumped)
        BadRobot = true;

    BumperHitTimer = millis();

    LastBumperBumped = BumperBumped;
  }

  if (PreviousMemoryAction == 0 && LeftBumperStatus == 0 && RightBumperStatus == 0 && MEMORY[LastActionPerformed][LeftBumperStatus][RightBumperStatus] != 0)
    BadRobot = true;

  Serial.print(LeftBumperStatus);
  Serial.print(" X ");
  Serial.print(RightBumperStatus);
  Serial.print(" BR:");
  Serial.print(BadRobot);
  Serial.print(" MEM:");
  Serial.println(MEMORY[LastActionPerformed][LeftBumperStatus][RightBumperStatus]);
  

  //Learning:
  if (BadRobot)
    MEMORY[PreviousMemoryAction][PreviousLeftBumperStatus][PreviousRightBumperStatus] = random(0,10);

  //Select Next Action 
  PerformAction(MEMORY[LastActionPerformed][LeftBumperStatus][RightBumperStatus]);

  


  //Update Previous Actions
  PreviousMemoryAction = LastActionPerformed;
  PreviousLeftBumperStatus = LeftBumperStatus;
  PreviousRightBumperStatus = RightBumperStatus;
  

}
