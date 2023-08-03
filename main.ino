// IMPORT LIBRARIES
#include <Keypad.h>
#include <LiquidCrystal.h>


// MAXIMUM LENGTH OF THE PASSWORD
#define MAX 4

// pin d9 for the buzzer
#define BUZZER 9

// pin d13 for sensor output
#define SENSOROUT 13

// pin d12 for sensor power
#define SENSORPOWER 12


// SET UP KEYPAD
const byte ROWS = 4; 
const byte COLS = 3; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {19, 18, 17, 16}; 
byte colPins[COLS] = {15, 14, 8}; 
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// SET UP LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// state 1 - DISARMED
// state 2 - ARMED
// state 3 - ALERT
int state;        // state variable 



int password(void) {

lcd.clear();

int length =0;
char PIN[4] = {'1','2','3','4'};
char entry[4];
int flag;

lcd.print("Enter PIN:");
lcd.blink();

while (1){  
  
flag = 1;

if (length < MAX)   // probe for key touches
{         
          //lcd.print("X: ");
          char key = keypad.getKey();
          if (key != NO_KEY)
            {
              lcd.print(key);
              entry[length] = key;
              length++;
            }
}
else             
{        
          
          //SHORT PAUSE
          delay(1000);
          lcd.clear();

          
          // COMPARE PINS
          for (size_t i = 0; i < MAX; i++) 
          {
                  if (PIN[i] == entry[i]) 
                    {
                      continue;
                    }
                  else 
                    {
                    flag = 0;
                    break;               
                    } 
          }

          //CHECK SUCCESS OR FAILURE
          if (flag ==1 )
            {           
                        lcd.noBlink();
                        lcd.print("Success.");
                        
                        // in case buzzer was already ON 
                        digitalWrite(BUZZER, LOW);
                        delay(100);

                        digitalWrite(BUZZER, HIGH);
                        delay(100);
                        digitalWrite(BUZZER, LOW);
                        delay(100);
                        digitalWrite(BUZZER, HIGH);
                        delay(100);
                        digitalWrite(BUZZER, LOW);

                        delay(3000);
                        lcd.clear();
                        
                        return 1;        
                        //break;
            }
            else 
            {    
                        lcd.noBlink();
                        lcd.print("Try Again.");
                        // digitalWrite(BUZZER, HIGH);
                        // delay(100);
                        // digitalWrite(BUZZER, LOW);


                        delay(3000);

                        length = 0; //restart
                        lcd.clear();
                        lcd.print("Enter PIN:");
                        lcd.blink();
            }     
}
}
}


//this function implements a countdown using millis() 
void countdown(int interval) {

  unsigned long current;
  int time1;
  int time2;
  int i = 0; // counts up to 60 
  lcd.print("Arming System");
  lcd.noBlink();
  
  unsigned long start = millis();
  while(1) {
    current = millis();
    int time = (current - start)/1000;
    if (time > interval) {
      break;
    }
    lcd.setCursor(0, 1);

    // we are counting DOWN, not counting UP
    time2 = interval - time;
    if (time2 < 10) 
         lcd.print('0'); 
    lcd.print(time2);
    
  }
  lcd.clear();
  delay(1500);
}


// this function monitors for intruders
int eternalcheck(void){

int val;

while(1){
  val = digitalRead(SENSOROUT);   // read sensor value

  
  if (val == HIGH) {           // check if the sensor is HIGH
    //digitalWrite(led, HIGH);   // turn LED ON
    digitalWrite(BUZZER, HIGH);
    return 1;
  } 

  }
}
  
 


void setup () {


    //set up some PINS
      pinMode(BUZZER, OUTPUT);
      pinMode(SENSORPOWER, OUTPUT);
      pinMode(SENSOROUT, INPUT);

      lcd.begin(16, 2);
      lcd.setCursor(3, 0);
      lcd.print("Welcome.");

      // check buzzer
      digitalWrite(BUZZER, HIGH);
      delay(500);
      digitalWrite(BUZZER, LOW);

      //lcd.print("testing");
      delay(3000);
      lcd.clear();
      // we start at state 1 - DISARMED.
      state = 1;     
}


void loop() 
{

// state 1 - DISARMED
// state 2 - ARMED
// state 3 - ALERT

// password() is infinite loop until it returns 1
if (password())
{ 
  state = 2;  // upon successful PIN, we enter state 2 - ARMED

  //TURN ON SENSOR HERE
  digitalWrite(SENSORPOWER, HIGH);

  countdown(60);           // count to some number for sensor to stabilize
  lcd.print("System Armed.");
  // check buzzer
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
  
  delay(2000);
  lcd.clear();


  if (eternalcheck()) {
    // we are now in alarm state
    state = 3; 
    lcd.print("ALARM!");
    delay(3000);

    if (password()) {
      // we are in disarmed state
      state = 1; 
      lcd.clear();
      lcd.print("Disarmed");  

      //turn off sensor and buzzer
      digitalWrite(BUZZER, LOW);
      digitalWrite(SENSORPOWER, LOW);
            
      delay(5000);
    }

    
  }      
  

}

}
 
