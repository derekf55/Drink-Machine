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
 //#define Pump2Ground 6
 #define pumpPin2 10
 #define pumpPin1 9
 //#define Pump1Ground 9
 #define pumpPin3 7
 //#define Pump3Ground 11

 String previous = "";

 int yn_counter = 0;
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
   pinMode(sw, INPUT);
   
  // Set Pump pins as outputs
  pinMode (pumpPin1, OUTPUT);
  //pinMode (Pump1Ground, OUTPUT);
  pinMode (pumpPin2, OUTPUT);
  //pinMode (Pump2Ground, OUTPUT);
  pinMode (pumpPin3, OUTPUT);
  //pinMode (Pump3Ground, OUTPUT);
  
  digitalWrite(pumpPin1, LOW);
  //digitalWrite(Pump1Ground, LOW);
  digitalWrite(pumpPin2, LOW);
  //digitalWrite(Pump2Ground, LOW);
  digitalWrite(pumpPin3, LOW);
  //digitalWrite(Pump3Ground, LOW);

  lcd.print("Select Drink");
  

  
   
   // Setup Serial Monitor
   Serial.begin (9600);
   
   // Read the initial state of inputCLK
   // Assign to previousStateCLK variable
   //previousStateCLK = digitalRead(inputCLK);
   //checkCounter();
 } 

 void loop(){
  // Start screen
  initial_screen();
 }

 void initial_screen(){
  while (true){
  if (digitalRead(sw) == LOW ) {
    delay(500);
    start();
  }
  }
 }

void start(){ 

while (true){
  //lcd.cursor();
  //Checks 
  
  
  
  // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);
    
   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the inputDT state is different than the inputCLK state then 
     // the encoder is rotating counterclockwise
     if (digitalRead(inputDT) != currentStateCLK ) { 
       counter --;
       encdir ="CCW";
       if (counter < 0){
        counter = 5;
       }
      checkCounter();
       
     } else {
       // Encoder is rotating clockwise
       counter ++;
       if (counter > 5){
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

    if (digitalRead(sw) == LOW ) {
    delay(500);
    lcd.clear();
    lcd.noCursor();
    lcd.print(previous);
    lcd.print("?");
    delay(1200);
    lcd.clear();
    lcd.print("*No");
    lcd.setCursor(0,1);
    lcd.print("Yes");
    lcd.setCursor(0,0);
    counter = 0;
    yes_no_counter();
    checkClick();
  }
 }
 }


//Checks the current value of the rotory encoder and displays proper drink
void checkCounter(){
  if (counter == 0 or counter == 1){
    lcd.clear();
    lcd.print("Vodka OJ");
    previous = "Vodka OJ";
  }
  if (counter == 2 or counter == 3){
    lcd.clear();
    lcd.print("Vodka Cranberry");
    previous = "Vodka Cranberry";
  } 
  if (counter == 4 or counter == 5){
    lcd.clear();
    lcd.print("Rum n Coke");
    previous = "Rum n Coke";
  } 
}

 //If the rotory encoder is clicked check what was selected
void checkClick(){
  //Pump 1 Vodka
  //Pump 2 OJ
  //Pump 3 Cran

  //turn cursor off

  Serial.print("counter at click:    ");
  Serial.println(counter);
  
    //Vodka OJ
    if (counter == 0 or counter == 1){
      switchable = false;
      lcd.clear();
      lcd.print("Please Wait ...");
      runPump(1);               
      runPump(2);
      lcd.clear();
      lcd.print("Drink Mixed");
      delay(2000);
      switchable = true;
    }
    //Vodka Cran
    if (counter == 2 or counter == 3){
      switchable = false;
      lcd.clear();
      lcd.print("Please Wait ...");
      runPump(1);
      runPump(3);
      lcd.clear();
      lcd.print("Drink Mixed");
      delay(2000);
      switchable = true; 
    }

    //Coke n Rum
    if (counter == 4 or counter == 5){
      switchable = false;
      lcd.clear();
      lcd.print("Please Wait ...");
      runPump(1);
      runPump(3);
      lcd.clear();
      lcd.print("Drink Mixed");
      delay(2000);
      switchable = true; 
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
    Serial.println("OJ");
    digitalWrite(pumpPin2, HIGH);
    delay(juice);
    digitalWrite(pumpPin2, LOW);
  }
  // Cran
  if (pump == 3){
    digitalWrite(pumpPin3, HIGH);
    delay(juice);
    digitalWrite(pumpPin3, LOW);
  }

  
}

void yes_no_counter(){
  while(true){

    if (digitalRead(sw) == LOW ) {
      delay(500);
      lcd.clear();
      if (yn_counter >1){
        checkClick();
        lcd.clear();
        lcd.print("Select Drink");
        initial_screen();
      }
      else{
        lcd.print("Select Drink");
        start();
      }
    }
           
           currentStateCLK = digitalRead(inputCLK);
            
           // If the previous and the current state of the inputCLK are different then a pulse has occured
           if (currentStateCLK != previousStateCLK){ 
               
             // If the inputDT state is different than the inputCLK state then 
             // the encoder is rotating counterclockwise
             if (digitalRead(inputDT) != currentStateCLK ) { 
               yn_counter --;
               encdir ="CCW";
               if (yn_counter < 0){
                yn_counter = 3;
               }
              
               
             } else {
               // Encoder is rotating clockwise
               yn_counter ++;
               if (yn_counter > 3){
                yn_counter = 0;
               }
               encdir ="CW";
              
               
             }
             Serial.print("Direction: ");
             Serial.print(encdir);
             Serial.print(" -- Value: ");
             Serial.println(yn_counter);
             previousStateCLK = currentStateCLK; 
             yes_no_edit();
           } 
           // Update previousStateCLK with the current state
           
        
    }
}

void yes_no_edit(){
  if (yn_counter > 1){
    lcd.setCursor(0,0);
    lcd.print("      ");
    lcd.setCursor(0,0);
    lcd.print("No");
    lcd.setCursor(0,1);
    lcd.print("      ");
    lcd.setCursor(0,1);
    lcd.print("*Yes");
    Serial.println("yes");
  }
  else{
    lcd.setCursor(0,0);
    lcd.print("      ");
    lcd.setCursor(0,0);
    lcd.print("*No");
    lcd.setCursor(0,1);
    lcd.print("      ");
    lcd.setCursor(0,1);
    lcd.print("Yes");
    Serial.println("yes");
  }
  yes_no_counter();
}
