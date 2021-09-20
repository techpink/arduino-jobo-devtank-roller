#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int button = btnNONE;
int lastButton = btnNONE;

// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  button = btnNONE;
  if (adc_key_in < 50)
  {
    button = btnRIGHT;
  }
  else if (adc_key_in < 250)
  {
    button = btnUP;
  }
  else if (adc_key_in < 450)
  {
    button = btnDOWN;
  }
  else if (adc_key_in < 650)
  {
    button = btnLEFT;
  }
  else if (adc_key_in < 850)
  {
    button = btnSELECT;
  }
  
  // If the button changed, due to noise or pressing:
  if (button != lastButton)
  {
    lastButton = button;
    return button;
  }

  return btnNONE;
}

const int relayPin =  2;
const int buzzerPin = 3;

// timer
bool running = false;
bool finished = false;
unsigned long devTime = 300; // default development time
unsigned long startTime;
String strTime;

void startRunning() {
  if (!running)
  {
    running = true;
     
    // activate relay
    digitalWrite(relayPin, HIGH);
    
    startTime = millis();
    lcd.setCursor(0,1);
    lcd.print("Running    ");
  }
}

void stopRunning() {
  if (running)
  {
    running = false;
    finished = true;
     
    // deactivate relay
    digitalWrite(relayPin, LOW);

    lcd.setCursor(0,1);
    lcd.print("Finished   ");
  }
}

String getTimeStr(int t)
{
  String strMins = String(t / 60);
  String strSecs = String(t % 60);
  if (t % 60 < 10)
  {
    return strMins + ":0" + strSecs;
  }
  else
  {
    return strMins + ":" + strSecs;
  }
}

void setup()
{
  // Start the LCD library
  lcd.begin(16, 2);
  
  lcd.setCursor(0,1);
  lcd.print("Not started");
  
  // Set the relay pin to output:
  pinMode(relayPin, OUTPUT);
  
  // Set the buzzer pin to output:
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  // display dev time
  lcd.setCursor(0,0);
  lcd.print("Dev. time: ");
  strTime = getTimeStr(devTime);
  lcd.setCursor(16 - strTime.length(),0);
  lcd.print(strTime);


  if (running)
  {
    // display running time
    strTime = getTimeStr((millis() - startTime) / 1000);
    lcd.setCursor(16 - strTime.length(), 1);
    lcd.print(strTime);
    
    if (millis() > startTime + (devTime * 1000))
    {
      stopRunning();
    }
  }
  else if (finished)
  {
    // sound the buzzer
    digitalWrite(buzzerPin, HIGH);  
    delay(50);
    digitalWrite(buzzerPin, LOW);  
    delay(50);
  }

  lcd_key = read_LCD_buttons();
  switch (lcd_key)
  {
   case btnRIGHT:
     break;
   case btnLEFT:
     break;
   case btnUP:
     {
       devTime += 15;
       break;
     }
   case btnDOWN:
     {
       if (devTime >= 15)
       {
         devTime -= 15;
       }
       break;
     }
   case btnSELECT:
     {
       startRunning();
       break;
     }
   case btnNONE:
     break;
  }
}
