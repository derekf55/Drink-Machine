
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
const int i2c_addr = 0xA7;


LiquidCrystal_I2C lcd(i2c_addr,en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);


 // Rotary Encoder Inputs
 #define sw 3
 #define inputCLK 4
 #define inputDT 5

 //Pump Inputs
 #define Pump2Ground 6
 #define pumpPin2 7
 #define pumpPin1 8
 #define Pump1Ground 9
 #define pumpPin3 10
 #define Pump3Ground 11

 int counter = 0; 
 int currentStateCLK;
 int previousStateCLK; 
 boolean selected = false;
 boolean switchable = true;
 String encdir ="";

 void setup() { 

   //Start LCD
   lcd.begin(16,2);
   
   // Set encoder pins as inputs  
   pinMode (inputCLK,INPUT);
   pinMode (inputDT,INPUT);
   pinMode(sw, INPUT_PULLUP);
   
  // Set Pump pins as outputs
  pinMode (pumpPin1, OUTPUT);
  pinMode (Pump1Ground, OUTPUT);
  pinMode (pumpPin2, OUTPUT);
  pinMode (Pump2Ground, OUTPUT);
  pinMode (pumpPin3, OUTPUT);
  pinMode (Pump3Ground, OUTPUT);
  
  digitalWrite(pumpPin1, LOW);
  digitalWrite(Pump1Ground, LOW);
  digitalWrite(pumpPin2, LOW);
  digitalWrite(Pump2Ground, LOW);
  digitalWrite(pumpPin3, LOW);
  digitalWrite(Pump3Ground, LOW);
  
   
   // Setup Serial Monitor
   Serial.begin (9600);
   
   // Read the initial state of inputCLK
   // Assign to previousStateCLK variable
   previousStateCLK = digitalRead(inputCLK);
   checkCounter();
 } 

 void loop() { 

  //Checks 
  if (digitalRead(sw) == LOW ) {
    checkClick();
  }
  
  // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);
    
   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the inputDT state is different than the inputCLK state then 
     // the encoder is rotating counterclockwise
     if (digitalRead(inputDT) != currentStateCLK ) { 
       counter --;
       encdir ="CCW";
       if (counter < -1){
        counter = 1;
       }
      checkCounter();
       
     } else {
       // Encoder is rotating clockwise
       counter ++;
       if (counter > 2){
        counter = 0;
       }
       encdir ="CW";
      checkCounter();
       
     }
     Serial.print("Direction: ");
     Serial.print(encdir);
     Serial.print(" -- Value: ");
     Serial.println(counter);
   } 
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 
 }


//Checks the current value of the rotory encoder and displays proper drink
void checkCounter(){
  if (counter == 0){
    lcd.clear();
    lcd.print("Vodka OJ");
  }
  if (counter == 1){
    lcd.clear();
    lcd.print("Vodka Cranberry");
  } 
}

 //If the rotory encoder is clicked check what was selected
void checkClick(){
  //Pump 1 Vodka
  //Pump 2 OJ
  //Pump 3 Cran

    //Vodka OJ
    if (counter == 0){
      switchable = false;
      lcd.clear();
      lcd.print("Please Wait ...");
      runPump(1);
      runPump(2);
      lcd.clear();
      lcd.print("Drink Mixed");
      delay(2000);
      switchable = true;
      checkCounter();
    }
    //Vodka Cran
    if (counter == 1){
      switchable = false;
      lcd.clear();
      lcd.print("Please Wait ...");
      runPump(1);
      runPump(3);
      lcd.clear();
      lcd.print("Drink Mixed");
      delay(2000);
      switchable = true;
      checkCounter(); 
    }
    
}

// Runs the pump 
//@param is the pump number to be activated
void runPump(int pump){
  int hardLiquor = 2000;
  int juice = 5000;
  
  //Vodka 
  if (pump == 1){
    digitalWrite(pumpPin1, HIGH);
    delay(hardLiquor);
    digitalWrite(pumpPin1, LOW);
  }
  //Orange Juice
  if (pump == 2){
    digitalWrite(pumpPin2, HIGH);
    delay(juice);
    digitalWrite(pumpPin2, LOW);
  }
  // Cran
  if (pump == 3){
    digitalWrite(pumpPin2, HIGH);
    delay(juice);
    digitalWrite(pumpPin2, LOW);
  }

  
}



 
