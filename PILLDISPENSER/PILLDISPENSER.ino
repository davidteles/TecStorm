

#include <Stepper.h>

int actualcompartment=0;
const int stepsbetweencompartments=25;
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor


// initialize the stepper library on pins 8 through 11:
Stepper rotationStepper(stepsPerRevolution, 8, 9, 10, 11);
Stepper selectionStepper(stepsPerRevolution, 4, 5, 6, 7);


int stepCount = 0;  // number of steps the motor has taken

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  while(Serial.readString()!="START"){
    ;
  }
  
  
  Serial.println("Hello There");
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2,INPUT);
  pinMode(A0,INPUT);
  initialization();
  Serial.println("Ready!");
}

void loop() {

  
  if (Serial.available() > 0) {
      // read the incoming byte:
      String data = Serial.readString();
      //Serial.println("I received");
      //Serial.println(data);
      
      if(data.startsWith("BLINK")){
        String intfromdata=data.substring(5);
        //Serial.println(intfromdata);
        int value = intfromdata.toInt();
        
        if(blinkled(3,500,value)==1){
          Serial.println("OK");
        }
      }
      else if (data.startsWith("BEEP")){
        String intfromdata=data.substring(4);
        //Serial.println(intfromdata);
        int value = intfromdata.toInt();
        
        if(beep(12,500,value)==1){
          Serial.println("OK");
        }
      }
      else if (data.startsWith("GOTO")){
        String destinationdata=data.substring(4);
        //Serial.println(destinationdata);
        int destination = destinationdata.toInt();
        actualcompartment=gotocompartment(actualcompartment,destination);
        Serial.println("OK");
      }
      else if (data.startsWith("PILL")){
        String intfromdata=data.substring(4);
        //Serial.println(intfromdata);
        int value = intfromdata.toInt();
        if(extractPill(value)==1){
          Serial.println("OK");
        }
      }
      

  }
}


int initialization(){
  beep(12,500,3);
  digitalWrite(3,HIGH);
  findHome();
  digitalWrite(3,LOW);
  beep(12,1000,1);
  return 1;
}

int findHome(){
  while (digitalRead(2) != 0) {
    rotationStepper.setSpeed(100);
    rotationStepper.step(1);
  }
  actualcompartment = 0;
  return 1;
}



int beep(int pin,int timedelay,int count){

  for(int i=0; i<count;i++){
    
    digitalWrite(pin,HIGH);
    delay(timedelay);
    digitalWrite(pin,LOW);
    delay(timedelay);
  }

  return 1;
}



int blinkled(int pin,int timedelay,int count){

  for(int i=0; i<count;i++){
    digitalWrite(pin,HIGH);
    delay(timedelay);
    digitalWrite(pin,LOW);
    delay(timedelay);
  }

  return 1;
}



int gotocompartment(int initial, int finale){
  rotationStepper.setSpeed(100);
  rotationStepper.step((finale-initial)*stepsbetweencompartments);
  return finale;
}



int extractPill(int numberofpills){
  digitalWrite(13,HIGH);
  delay(100);
  int noise=100;
  int meanvalue=0;
  int meancounts=5;
  for(int i=0; i<meancounts; i++){
    meanvalue=meanvalue+analogRead(0);
  }
  meanvalue=meanvalue/meancounts;

  for(int i=0; i<numberofpills; i++){
    int counter=0;
    selectionStepper.setSpeed(50);
    digitalWrite(3,HIGH);
    while(abs(analogRead(0)-meanvalue)<noise){
      selectionStepper.step(1);
      counter=counter+1;
    }
    digitalWrite(3,LOW);
    beep(12,500,1);
    selectionStepper.step((-1)*counter);
    delay(500);
  }
  delay(100);
  digitalWrite(13,LOW);
  return 1;
}



