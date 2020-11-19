/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */


#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  35, 0,                  // Button Count, Hat Switch Count
  false, false, false,     // X , Y, Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering



  
// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
//#include <Encoder.h>
//Encoder myEnc1(2, 3);
//Encoder myEnc2(4, 5);
//Encoder myEnc3(6, 7);
//#Encoder myEnc4(8, 9);
//   avoid using pins with LEDs attached

int lastbtn[50];
int BUTTONSHIFT=0;


volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder

//#define DEBUGSERIAL
void setup() {

  #ifdef DEBUGSERIAL
    Serial.begin (9600);
  #else
    // Initialize Joystick Library
    Joystick.begin();
  #endif
  //Serial.begin(9600);  Serial.println("Basic Encoder Test:");

  for(int i= 0 ; i<50;i++){
    lastbtn[i] = 0;
  }

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  
  pinMode(0, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(4, INPUT);
  pinMode(9, INPUT);

  //0, 1, 2, 3, 7
  attachInterrupt(digitalPinToInterrupt(1), ai1, FALLING);
  attachInterrupt(digitalPinToInterrupt(2), ai2, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), ai3, FALLING);
  attachInterrupt(digitalPinToInterrupt(7), ai4, FALLING);
}

volatile byte latchinput = 0;
byte safelatchinput = 0;
byte rotmoving = 0;
byte safeRotMoving = 0;
void ai1() {
  rotmoving=0;
  if (latchinput==0){
    if(digitalRead(5)==LOW ) {
      temp=1;
      rotmoving+=1;
    }else{
      temp=-1;
    }
  }
}
void ai2() {
  // ai0 is activated if DigitalPin nr 2 is going from HIGH to LOW 
  // Check pin 3 to determine the direction
  rotmoving=2;
  if (latchinput==0){
    if(digitalRead(6)==LOW ) {
      temp=1;
      rotmoving+=1;
    }else{
      temp=-1;
    }
  }
}



void ai3() {
  rotmoving=4;
  if (latchinput==0){
    if(digitalRead(4)==LOW ) {
      temp=1;
      rotmoving+=1;
    }else{
      temp=-1;
    }
  }
}

void ai4() {
  rotmoving=6;
  if (latchinput==0){
    if(digitalRead(9)==LOW ) {
      temp=1;
      rotmoving+=1;
    }else{
      temp=-1;
    }
  }
}

void setButton(int bID,int val){
  if(lastbtn[bID] != val){
    lastbtn[bID] = val;
    #ifdef DEBUGSERIAL
      Serial.print("bID:" );
      Serial.print(bID);
      Serial.print(" -> " );
      Serial.print(val);
      Serial.println("" );
    #else
      Joystick.setButton(bID, val);
    #endif
  }
}
void buttCheck(int pin,int bID){
  int b1 = digitalRead(pin);

  if(b1==1){
    setButton(bID+BUTTONSHIFT, 0);
  }else{
    setButton(bID+BUTTONSHIFT, 1);
    
  }
}

int modeCheck(){
  int mod = digitalRead(0);
  setButton(17, mod);
  if(mod==0) {
    return 0;
  }else{
    return 18;
  }
  
}


void loop() {
  // Send the value of counter
  noInterrupts();
    // critical, time-sensitive code here
    counter=temp;
    
    if(counter>0 ){

      latchinput = 1;
      safelatchinput=1;
      safeRotMoving = rotmoving;
      #ifdef DEBUGSERIAL

        Serial.print(millis());

        Serial.print(" pot: " );
        Serial.print(rotmoving+BUTTONSHIFT);
        Serial.print("->>>" );


        
      #else
        setButton(rotmoving+BUTTONSHIFT, 1);
      #endif
    }else if(counter<0){
      latchinput = 1;
      safelatchinput=1;
      safeRotMoving = rotmoving;
      #ifdef DEBUGSERIAL
        Serial.print(millis());

        Serial.print(" pot: " );
        Serial.print(rotmoving+BUTTONSHIFT);
        Serial.print("<<<---  " );

        
      #else
        setButton(rotmoving+BUTTONSHIFT, 1);
      #endif
    }

    // in case buttons does latch, prevent pot to switch
    latchinput = safelatchinput;
    temp=0;
    
  interrupts();

  if(safelatchinput==1){
    delay(50); // stop for a moment debounce
    
    noInterrupts();

    latchinput = 0;
    safelatchinput = 0;
    interrupts();
    
    #ifdef DEBUGSERIAL
      Serial.print("unlatch" );
      Serial.println("" );
    #else
      
      setButton(safeRotMoving,0 );  
      /*
      for(byte i=18;i<=25;i++){
        setButton(i, 0);  
        delay(2);
      }
      for(byte i=0;i<=7;i++){
        setButton(i, 0);  
        delay(2);
      }*/
      
    #endif
  }
  
  BUTTONSHIFT = modeCheck();

  buttCheck(8,12);
  buttCheck(A0,13);
  buttCheck(A1,14);
  buttCheck(A2,15);
  buttCheck(A3,16);

  // rotary buttons
  buttCheck(10,8);
  buttCheck(16,9);
  buttCheck(14,10);
  buttCheck(15,11);

  
  
}
