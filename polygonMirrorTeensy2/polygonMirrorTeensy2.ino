int laserPin = 6;
int redLaserPin = 5;
unsigned long curTime;
unsigned long lastTime;
unsigned long elapsedTime = 10000;

int hBridgeA = 11;
int hBridgeB = 12;


int in1 = 20;
int in2 = 21;
int in3 = 22;

int onSwitch = 2;
int steps = 0;
int cTime = 0;

#define STARTVALUE 500      // delay between mirror pulses
#define TARGET 32000
int curDelay = 20;

void setup(){
  // Wait for 1 second to allow everything to stabilize after being turned on. 
  // The current circuit has some capacitance issues.
  delay(1000);
  Serial.begin(9600);
  
  // Set all IO directions on pins
  pinMode(10, INPUT);         // input pin for red laser interrupt
  pinMode(laserPin, OUTPUT);
  pinMode(redLaserPin,OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(onSwitch, INPUT);
  pinMode(hBridgeA, OUTPUT);
  pinMode(hBridgeB, OUTPUT);
  
  tone(3, 40 );
  
  // Turn on opto-interruptor pin
  digitalWrite(redLaserPin,1);

  // Turn on Interrupt in Code
  attachInterrupt(10, faceSpin, FALLING);
}

unsigned long lastTimeChange = 0;
unsigned long changeInterval = 10;

boolean flip = true;

unsigned long lastCount = 0;
unsigned long count = 0;
long faceSpeed = 0;
long lastMicros = 0;
long lastGalvoMicros = 0;
boolean curGalvoState = false;

long beginLineAnimationMicros = 0;
long animationCount = 0;

long animationLength = 1000;
long lastAnimationTime = 0;

long animationFaceCount = 80;
long curAnimationFaceCount = 0;
boolean galvoAnimationState = 0;

void loop(){
  
  while(true)
  {
      spinMirror();
      moveGalvo();

      if(count > lastCount)
      {
        beginLineAnimationMicros = micros();
        lastCount = count;
        curAnimationFaceCount++;
        if(curAnimationFaceCount > animationFaceCount)
        { 
          curAnimationFaceCount = 0;
        }
      }
		
	  // Run the animation Algorithms
      fourPixelAnimation();
      //twoPixelAnimation();
  }

}

// Attempt to do a raster animation of 4 quadrants blinking (Aliases right now. Galvo is not perfect yet)
void fourPixelAnimation()
{
      if(curAnimationFaceCount < animationFaceCount/4)
      {
          if(micros() - beginLineAnimationMicros > faceSpeed/2)
          {
            digitalWrite(laserPin, 1);
          }
          else
          { 
            digitalWrite(laserPin, 0);
          }
      }
      else if(curAnimationFaceCount < animationLength/2)
      {
        if(micros() - beginLineAnimationMicros < faceSpeed/2)
          {
            digitalWrite(laserPin, 1);
          }
          else
          { 
            digitalWrite(laserPin, 0);
          }
      }
      else
      {
        digitalWrite(laserPin, 0);
      }

      if(millis() - lastAnimationTime > animationLength)
      {

        lastAnimationTime = millis();
      }
}

// Create a raster animation of two halves of our "screen blinking" (Works)
void twoPixelAnimation()
{
      if(millis() - lastAnimationTime < animationLength/2)
      {
        if(micros() - beginLineAnimationMicros > faceSpeed/2)
        {
          digitalWrite(laserPin, 1);
        }
        else
        {

          digitalWrite(laserPin, 0);
        }
      }
      else
      {
        if(micros() - beginLineAnimationMicros < faceSpeed/2)
        {
          digitalWrite(laserPin, 1);
        }
        else
        {

          digitalWrite(laserPin, 0);
        }
      }

      if(millis() - lastAnimationTime > animationLength)
      {

        lastAnimationTime = millis();
      }
}

// Control Galvo with an H-Bridge circuit
void moveGalvo()
{
  // Galvo moves based off of face speed of polygon mirror array
  if(micros() > lastGalvoMicros + ( (animationFaceCount/2)+ (analogRead(0)/100-5) )*faceSpeed)
  {
    if(curGalvoState)
    {
      digitalWrite(hBridgeB, 0);
      digitalWrite(hBridgeA, 1);
    }
    else
    {
      digitalWrite(hBridgeB, 1);
      digitalWrite(hBridgeA, 0);
    }
    curGalvoState = !curGalvoState;
    lastGalvoMicros = micros();
  }

}

// Spin the BLDC under the Polygon Mirror Array
void spinMirror()
{
  if(digitalRead(2)==1)
  {   
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
  }
}
  
// Count everytime a face moves through the opto-interruptor
void faceSpin(){
  
  count++;

  faceSpeed = micros() - lastMicros;
  lastMicros = micros();

}

// Write data to the BLDC controller
void setInputs(int in1Val, int in2Val, int in3Val){
 if(micros() - cTime > curDelay)
 {
    digitalWrite(in1, in1Val);
    digitalWrite(in2, in2Val);
    digitalWrite(in3, in3Val);
    cTime = micros();

    steps++;
    if(in1Val == 0 && in2Val == 0 && in3Val == 1)
      steps = 0;
 }
}

