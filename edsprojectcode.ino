#include <SPI.h>    
#include <MFRC522.h>   //Library for mfrc522 RFID reader
 #include <LiquidCrystal.h>   ///Library for LCD(16*2)
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int servopin=6;   //servopin
int angle,pulsewidth;
LiquidCrystal lcd(7, 8, 5, 4, 3, 2); 
void servopulse(int servopin,int angle) //user defined function for servo motor according to  
{
    pulsewidth=angle*10+600;   //mapping of pulsewidth with the angle
    digitalWrite(servopin,HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servopin,LOW);
}
void setup() 
{

    Serial.begin(9600);   // Initiate a serial communication
    SPI.begin();      // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
    Serial.println("Approximate your card to the reader...");
    Serial.println();
    pinMode(servopin,OUTPUT);  //Declaring Servo pin as output
    lcd.begin(16, 2); 
}

void loop() 
{
     // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        return;
      }
    
    // Select one of the cards
    
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        return;
      }
    
    //Show UID on serial monitor
      Serial.print("UID tag :");
      
      String content= "";
      
      byte letter;
      
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
         Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
         Serial.print(mfrc522.uid.uidByte[i], HEX);
         content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
         content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      
      Serial.print("Message : ");
      
      content.toUpperCase();
      
      if(content.substring(1) == "06 45 2E 02") //UID of card
      /*In Real life if the card is rechared enough then the UID of the card will be regestered as valid that is why there is a condition 
       * on the UID of the card
      */ 
      {
              Serial.println("Authorized access");    //  check for the RFID wheteher it is working or not
              lcd.print("Welcome");   
                 delay(1000);
 
                // scroll 13 positions (string length) to the left
                    // to move it offscreen left:
              for (int positionCounter = 0; positionCounter < 7; positionCounter++) {
                // scroll one position left:
                lcd.scrollDisplayLeft();
                // wait a bit:
                delay(40);
              }
              lcd.clear();  //Clears the content of LCD
              lcd.print("Car no:");
              lcd.setCursor(0,1);
              lcd.print("GJ 04 HP 1390");
              delay(1000);
              // scroll 29 positions (string length + display length) to the right
              // to move it offscreen right
              for (int positionCounter = 0; positionCounter < 13; positionCounter++)
              {
                // scroll one position left:
                lcd.scrollDisplayLeft();
                // wait a bit:
                delay(40);
              }
              lcd.clear();
              lcd.print("Tax:50 INR");  //Prints the tax required
              delay(1500);
              for(int positionCounter=0;positionCounter<10;positionCounter++)
              {
                lcd.scrollDisplayLeft();
                delay(40);
              }
              //Open the gate
              for(angle=0;angle<=90;angle++)   //angle change from 0 to 90
              {                                  
                  servopulse(servopin,angle);      
                  delay(20);                         
              }
            lcd.clear();
            lcd.print("Bye");
            delay(1000);
            for(int positionCounter=0;positionCounter<3;positionCounter++)
            {

              lcd.scrollDisplayLeft();
              delay(40);
            }
             delay(1900);
             //Close the gate
             
             for(angle=90;angle>=0;angle--)      //angle change from 90 to 0
             {
                 servopulse(servopin,angle);
                 delay(20);
             }
            lcd.clear();
             
          
       }
     /*If the user has not the card(RFID tag) recharged then the process will work manually*/
     else  
     {
        Serial.println(" Access denied");//Access denied
        lcd.print("Pay Manually!");
        lcd.setCursor(0,1);
        lcd.print("Tax:50 INR");
        delay(2500);  //Some time for maunal cash collection
        for(int positionCounter=0;positionCounter<13;positionCounter++)
        {
          lcd.scrollDisplayLeft();
          delay(40);
        }
        lcd.clear();
        lcd.print("Payment Done!");
        lcd.setCursor(0,1);
        lcd.print("Thank you!");
        delay(1500);
        for(int positionCounter=0;positionCounter<13;positionCounter++)
        {
          lcd.scrollDisplayLeft();
          delay(40);
        }
        //Open the gate
             for(angle=0;angle<=90;angle++)   //angle cchange from 0 to 90
              {                                  
                  servopulse(servopin,angle);      
                  delay(20);                         
              }
              delay(999);
              //Close the gate
               for(angle=90;angle>=0;angle--)      //angle change from 0 to 90
             {
                 servopulse(servopin,angle);
                 delay(20);
             }
             lcd.clear();
              
        }
        
      
        
     }