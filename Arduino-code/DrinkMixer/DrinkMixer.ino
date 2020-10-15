//Program written by Derek and Brian 
//Last Updated 9/17/2020
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
const int i2c_addr = 0xA7;


//LiquidCrystal_I2C lcd(i2c_addr,en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);
LiquidCrystal_I2C lcd(0x27,16,2);

//Current set drinks
String drinks[] = {"Vodka Lemonade","Vodka Cran","Rum n Coke"};
int numOfDrinks = sizeof drinks / sizeof drinks[0];


 // Rotary Encoder Inputs
 #define sw 10
 #define inputCLK 8
 #define inputDT 9


 //Assigning pin 0 for all unaccounted for pumps
 //Purple
 int pumpPin1 = 2;
 //Green
 int pumpPin2 = 3;
 //Orange
 int pumpPin3 = 4;
 //Yellow
 int pumpPin4 = 5;
 //Brown
 int pumpPin5 = 6;
 //Gray
 int pumpPin6 = 7;
 

 String previous = "";

 int yn_counter = 0;
 int counter = 0; 
 int currentStateCLK;
 int previousStateCLK; 
 boolean selected = false;
 boolean switchable = true;
 String encdir ="";
  
  //Gives all the pumps the option to be controlled by the name of the liquid they contain currently
  //Orange
  int vodka = pumpPin3;
  //purp
  int lem = pumpPin1;
  //Gray
  int cran = pumpPin6;
  //Yellow
  int vodka2 = pumpPin4;
  //Green
  int rum = pumpPin2;
  //Brown
  int coke = pumpPin5;

  //Gives the time to pour for hardliquor and mixer
  //Estimating the flow rate for small pump at 30ml/min
  long hardliquor = 120000;
  long mixer = 44000;


 void setup() { 

   //Start LCD
   lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
    lcd.backlight();
   lcd.begin(16,2);

   //Set encoder pins as inputs  
  pinMode (inputCLK,INPUT);
  pinMode (inputDT,INPUT);
  pinMode(sw, INPUT);
   
   //Set Pump pins as outputs
  pinMode (pumpPin1, OUTPUT);
  pinMode (pumpPin2, OUTPUT);
  pinMode (pumpPin3, OUTPUT);
  pinMode (pumpPin4, OUTPUT);
  pinMode (pumpPin5, OUTPUT);
  pinMode (pumpPin6, OUTPUT);
  

  //Insure all pumps are off
  digitalWrite(pumpPin1, LOW);
  digitalWrite(pumpPin2, LOW);
  digitalWrite(pumpPin3, LOW);
  digitalWrite(pumpPin4, LOW);
  digitalWrite(pumpPin5, LOW);
  digitalWrite(pumpPin6, LOW);


  lcd.print("Select Drink");
   
   // Setup Serial Monitor
   Serial.begin (9600);
   delay(1000);
   int counter = 0;
   
 } 

//Runs the pumps in order to make the specified drink
//@param takes the name of the drink to be made & the size of the drink
//Function writen by Derek
void make_drink(String drink, String drinkSize){
  Serial.print(drink);

  lcd.clear();
  lcd.print("Please wait ...");

  //Checks to see if drink should be a double
  if (drinkSize == "Double"){
    hardliquor = hardliquor*2;
  }

  
  
  if (drink == "Vodka Lemonade"){
    digitalWrite(vodka, HIGH);
    digitalWrite(vodka2, HIGH);
    digitalWrite(lem, HIGH);
    delay(mixer);
    digitalWrite(lem, LOW);
    //Only delaying by half the amount of time for hard liquor assuming that there are two pumps for vodka
    delay((hardliquor/2)-mixer);
    digitalWrite(vodka, LOW);
    digitalWrite(vodka2, LOW);   
  } else if (drink == "Vodka Cran"){
    digitalWrite(vodka, HIGH);
    digitalWrite(vodka2, HIGH);
    digitalWrite(cran, HIGH);
    delay(mixer);
    digitalWrite(cran, LOW);
    //Only delaying by half the amount of time for hard liquor assuming that there are two pumps for vodka
    delay((hardliquor/2)-mixer);
    digitalWrite(vodka, LOW);
    digitalWrite(vodka2, LOW);   
  } else if (drink == "Rum n Coke"){
    digitalWrite(rum, HIGH);
    digitalWrite(coke, HIGH);
    delay(mixer);
    digitalWrite(coke, LOW);
    delay(hardliquor-mixer);
    digitalWrite(rum, LOW);
    
  } 
  //Test drink
  else if (drink == "Vod Coke") {
    digitalWrite(rum, HIGH);
    digitalWrite(coke, HIGH);
    delay(mixer);
    digitalWrite(coke, LOW);
    delay(hardliquor-mixer);
    digitalWrite(rum, LOW);
  } else{
    Serial.print("Failed");
  }

  lcd.clear();
  lcd.print("Drink Mixed");
  delay(2000);
  switchable = true;
  
}


 void loop(){
  // Start screen
  start();
 }


void start(){ 

counter = 0;
lcd.clear();
lcd.print(drinks[getDrinkIndex(counter)]);

while (switchable == true){
  // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);
    
   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the inputDT state is different than the inputCLK state then 
     // The user is turning left
     if (digitalRead(inputDT) != currentStateCLK ) { 
       counter --;
       encdir ="Left";
       if (counter < 0){
        counter = (numOfDrinks*2)-1;
        //Serial.print(numOfDrinks);
       }
      lcd.clear();
      lcd.print(drinks[getDrinkIndex(counter)]);
      //Serial.print(counter);
      //Serial.print(drinks[getDrinkIndex(counter)]+"\n");
       
     } else {
       // User is turning right
       counter ++;
       if (counter > (numOfDrinks*2)-1){
        counter = 0;
       }
       encdir ="Right";
      lcd.clear();
      lcd.print(drinks[getDrinkIndex(counter)]);
      //Serial.print(counter);
      //Serial.print(drinks[getDrinkIndex(counter)]+"\n");
       
     }
   } 
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 

    //When the encoder is clicked
    if (digitalRead(sw) == LOW ) {
      switchable = false;
      doubles_menu(counter);
      
  }
 }
 }

void doubles_menu(int drinkCounter){

  delay(500);
  lcd.clear();
  lcd.print("Single");
  counter = 0;
  delay(1000);
  while (true){
    // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);
    
   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the inputDT state is different than the inputCLK state then 
     // The user is turning left
     if (digitalRead(inputDT) != currentStateCLK ) { 
       counter --;
       encdir ="Left";
       if (counter < 0){
        counter = 5;
       }
      doubles_menu_checkCounter(counter);
       
     } else {
       // User is turning right
       Serial.print("Turning right\n");
       counter ++;
       if (counter > 5){
        counter = 0;
       }
       encdir ="Right";
      doubles_menu_checkCounter(counter);
       
     }
   } 
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 

    //When the encoder is clicked
    if (digitalRead(sw) == LOW ) {

      
      if (counter == 0 or counter == 1){
        Serial.print("Make single\n");
        make_drink(drinks[getDrinkIndex(drinkCounter)],"Single");
        start(); 
        
      }
      
      if (counter == 2 or counter == 3){
        Serial.print("Make double\n");
        make_drink(drinks[getDrinkIndex(drinkCounter)],"Dobule");
        start();
      }
      
      if (counter == 4 or counter == 5){
        Serial.println("Go back");
        switchable = true;
        delay(500);
        start();
      }
  }
  }
  
  
}

int getDrinkIndex(int counter){
  int drinkIndex;
  
    if (counter%2 == 0){
      drinkIndex = counter/2;
      //Serial.print("Even");
    } else{
      drinkIndex = (counter-1)/2;
      //Serial.print("ODD");
    }
    Serial.print(drinkIndex);
    return drinkIndex;
}

void doubles_menu_checkCounter(int counter){
  //Single
  if (counter == 4 or counter == 5){
    Serial.print("Should be single\n");
    lcd.clear();
    lcd.print("Single");
  }
  if (counter == 2 or counter == 3){
    Serial.print("Should be double\n");
    lcd.clear();
    lcd.print("Double");
  } 
  if (counter == 0 or counter == 1){
    Serial.print("Should be back\n");
    lcd.clear();
    lcd.print("Back");
  }
}



//@param takes the value of the counter and finds the correct index for the drink and prints it to the screen
void checkCounter(int counter){
    lcd.clear();
    int drinkIndex = getDrinkIndex(counter);
    lcd.print(drinks[drinkIndex]);
   
}
