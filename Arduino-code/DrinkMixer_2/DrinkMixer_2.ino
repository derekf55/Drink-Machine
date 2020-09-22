//Program written by Derek and Brian 
//Last Updated 9/17/2020
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
const int i2c_addr = 0xA7;


LiquidCrystal_I2C lcd(i2c_addr,20,4);

//Current set drinks
String drinks[] = {"Vodka Lemonade","Vodka Cran","Run n Coke"};
int numOfDrinks = sizeof drinks / sizeof drinks[0];


 // Rotary Encoder Inputs
 #define sw 3
 #define inputCLK 4
 #define inputDT 5

 //Pump Inputs
 #define pumpPin2 10
 #define pumpPin1 9
 #define pumpPin3 7

 //Assigning pin 0 for all unaccounted for pumps
 int pumpPin4 = 0;
 int pumpPin5 = 0;
 int pumpPin6 = 0;
 

 String previous = "";

 int yn_counter = 0;
 int counter = 0; 
 int currentStateCLK;
 int previousStateCLK; 
 boolean selected = false;
 boolean switchable = true;
 String encdir ="";
  
  //Gives all the pumps the option to be controlled by the name of the liquid they contain currently
  int vodka = pumpPin1;
  int lem = pumpPin2;
  int cran = pumpPin3;
  int vodka2 = pumpPin4;
  int rum = pumpPin5;
  int coke = pumpPin6;

  //Gives the time to pour for hardliquor and mixer
  //Estimating the flow rate for small pump at 30ml/min
  int hardliquor = 120;
  int mixer = 46;


 void setup() { 

   //Start LCD
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
   
 } 

//Runs the pumps in order to make the specified drink
//@param takes the name of the drink to be made & the size of the drink
//Function writen by Derek
void make_drink(String drink, String drinkSize){
  Serial.print(drink);

  switchable = false;

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
  }
  if (drink == "Vodka Cran"){
    digitalWrite(vodka, HIGH);
    digitalWrite(vodka2, HIGH);
    digitalWrite(cran, HIGH);
    delay(mixer);
    digitalWrite(cran, LOW);
    //Only delaying by half the amount of time for hard liquor assuming that there are two pumps for vodka
    delay((hardliquor/2)-mixer);
    digitalWrite(vodka, LOW);
    digitalWrite(vodka2, LOW);   
  }
  if (drink == "Run n Coke"){
    digitalWrite(rum, HIGH);
    digitalWrite(coke, HIGH);
    delay(mixer);
    digitalWrite(coke, LOW);
    delay(hardliquor-mixer);
    digitalWrite(rum, LOW);
    
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
        counter = (numOfDrinks/2)-1;
       }
      lcd.clear();
      lcd.print(drinks[getDrinkIndex(counter)]);
       
     } else {
       // User is turning right
       counter ++;
       if (counter > (numOfDrinks*2)-1){
        counter = 0;
       }
       encdir ="Right";
      lcd.clear();
      lcd.print(drinks[getDrinkIndex(counter)]);
       
     }
   } 
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 

    //When the encoder is clicked
    if (digitalRead(sw) == LOW ) {
      doubles_menu(counter);
  }
 }
 }

void doubles_menu(int drinkCounter){
  
  lcd.clear();
  lcd.print("Back");
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

      //Back button
      if (counter == 0 or counter == 1){
        start();
      }
      //Make a single
      if (counter == 2 or counter == 3){
        make_drink(drinks[getDrinkIndex(drinkCounter)],"Single");
      }
      //Make a double
      if (counter == 4 or counter == 5){
        make_drink(drinks[getDrinkIndex(drinkCounter)],"Dobule");
      }
  }
  
}

int getDrinkIndex(int counter){
  int drinkIndex;

    if (counter%2 == 0){
      int drinkIndex = counter/2;
    } else{
      int drinkIndex = (counter-1)/2;
    }
    return drinkIndex;
}

void doubles_menu_checkCounter(int counter){
  //Back
  if (counter == 0 or counter == 1){
    lcd.clear();
    lcd.print("Back");
  }
  if (counter == 2 or counter == 3){
    lcd.clear();
    lcd.print("Single");
  } 
  if (counter == 4 or counter == 5){
    lcd.clear();
    lcd.print("Double");
  }
}



//@param takes the value of the counter and finds the correct index for the drink and prints it to the screen
void checkCounter(int counter){
    lcd.clear();
    int drinkIndex = getDrinkIndex(counter);
    lcd.print(drinks[drinkIndex]);
   
}
/*
//If the rotory encoder is clicked check what was selected
//@param takes the value of the counter and finds the correct index for the drink and prints it to the screen
void checkClick(int counter){
  //turn cursor off

  Serial.print("counter at click:    ");
  Serial.println(counter);

     switchable = false;
     lcd.clear();
     lcd.print("Please Wait ...");

    int drinkIndex;

    if (counter%2 == 0){
      int drinkIndex = counter/2;
    } else{
      int drinkIndex = (counter-1)/2;
    }

    
    lcd.clear();
    lcd.print("Drink Mixed");
    delay(2000);
    switchable = true;
        
}
*/
