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


//volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder

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
  
  pinMode(0, INPUT);
  pinMode(1, INPUT_PULLUP);


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
  Joystick.setButton(17, mod);
  if(mod==0) {
    return 0;
  }else{
    return 18;
  }
  
}
void loop() {

  BUTTONSHIFT = modeCheck();

  buttCheck(1,12);
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
