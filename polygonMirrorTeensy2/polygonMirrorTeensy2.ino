
int in1 = 20;
int in2 = 21;
int in3 = 22;

int onSwitch = 2;

int mirror1 = 11;
int mirror2 = 12;

int laserPin = 6;

unsigned long delayVal = 5;

// the setup routine runs once when you press reset:
void setup() {                
  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(onSwitch, INPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(mirror1, OUTPUT);
    pinMode(mirror2, OUTPUT);

}

int steps = 0;
int curTime = 0;


#define CHANGEVALUE 1
#define PITCH 11250
#define STARTVALUE 4910
int curDelay = STARTVALUE;

// the loop routine runs over and over again forever:
void loop() {

 //tone(mirror,40);
 // Serial.println(digitalRead(onSwitch));
  if(digitalRead(2)==1)
  {
      tone(laserPin,PITCH);
   //  digitalWrite(laserPin,HIGH);
    digitalWrite(13, HIGH);

    switch(steps)
    {
      case 0: setInputs(1,0,1);
              break;
      case 1: setInputs(1,0,0);
              break;
      case 2: setInputs(1,1,0);
              break;
      case 3: setInputs(0,1,0);
              break;
      case 4: setInputs(0,1,1);
              break;
      case 5: setInputs(0,0,1);
              break;
      
    }
  }
  else{
        digitalWrite(13, LOW);
        noTone(6);

  }
//  if (micros()%analogRead(0)>analogRead(0)*.8){
//    digitalWrite(laserPin, HIGH);
//       
//  }
//  else{
//    digitalWrite(laserPin, LOW);
//  }
}



void setInputs(int in1Val, int in2Val, int in3Val)
{

//delayVal = analogRead(0); 
  
  
  if(Serial.available())
  {
    char readVal = Serial.read();
    
    if(readVal == 'u')
      curDelay += CHANGEVALUE;
    else if(readVal == 'd')
      curDelay -= CHANGEVALUE;
      else if(readVal == 's')
      curDelay = STARTVALUE;

    Serial.println(curDelay);
  }
 
 if(micros() - curTime > curDelay)
 {
    digitalWrite(in1, in1Val);
    digitalWrite(in2, in2Val);
    digitalWrite(in3, in3Val);
    curTime = micros();

    steps++;
    if(in1Val == 0 && in2Val == 0 && in3Val == 1)
      steps = 0;
 }
  
}
