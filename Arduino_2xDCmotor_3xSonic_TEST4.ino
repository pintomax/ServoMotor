/* 
 *  Code Written by Diogo Estima. I've explained all the code in the grey comments.
 *  I recommend you to go over the code, examine it, play with it, improve it and modify it according to your needs. 
 *  https://www.youtube.com/watch?v=AOL_vbarsEk
 *  
 *  https://www.dropbox.com/sh/c7xzxbrc2eriyf9/AAC7q2DLdfjapvh_VJZtTQtCa?dl=0
 *
 *  https://learn.adafruit.com/afmotor-library-reference/af-dcmotor# // Reference per gestione motori con shiels
 *  20160230 - Aggiunta gestione delle distanze con modifica rotazione motori MotorRight e MotorLeft
 *
 * BY MORONI detto Enrico
 *  
*/

#include <AFMotor.h> //import your motor shield library

int Direzione = 0;

// Led segnalzione arresto motori
int Led_FermoMotori = 12;

// variabili per calcolo tempo trascorso con Millis
unsigned long currentTime;
unsigned long loopTime;

// Sensore centrale
#define Central_trigPin A0 
#define Central_echoPin A1

// Sensore destro
#define Right_trigPin A2 
#define Right_echoPin A3

// Sensore sinistro
#define Left_trigPin A4 
#define Left_echoPin A5
long Central_distance, Right_distance, Left_distance;
long Central_duration, Right_duration, Left_duration;

// Setup motori
AF_DCMotor MotorRight(1, MOTOR12_64KHZ); // Motore di destra rispetto alla vista frontale
AF_DCMotor MotorLeft(3, MOTOR34_64KHZ); // Motore di sinistra rispetto alla vista frontale
 
void setup() {
  Serial.begin(9600); // begin serial communitication  
  pinMode(Led_FermoMotori, OUTPUT); // Led segnalazione fermo motori
  digitalWrite(Led_FermoMotori, LOW); // spengo led segnalazione fermo motori
  pinMode(Central_trigPin, OUTPUT);// set the trig pin to output (Send sound waves)
  pinMode(Central_echoPin, INPUT);// set the echo pin to input (recieve sound waves)
  pinMode(Right_trigPin, OUTPUT);// set the trig pin to output (Send sound waves)
  pinMode(Right_echoPin, INPUT);// set the echo pin to input (recieve sound waves)
  pinMode(Left_trigPin, OUTPUT);// set the trig pin to output (Send sound waves)
  pinMode(Left_echoPin, INPUT);// set the echo pin to input (recieve sound waves)
  Serial.println("Ambros - Inizializzazione sensori HC-SR04 ok");
  MotorRight.setSpeed(210); //set the speed of the motors, between 0-255
  MotorLeft.setSpeed (210);  
  Serial.println("Ambros - Inizializzazione motori a 250");
}
 
void loop() {

  Read_all_sensors(); // leggo i valori di tutti i sensori
  
// if (Central_distance > 100 && (Right_distance < 35 || Right_distance > 30) && (Left_distance > 35)) // proseguo dritto

if (Central_distance > 100) Direzione = 1; // proseguo dritto
if ((Central_distance > 100) && (Right_distance < 0) && (Left_distance > 35)) Direzione = 2;  // sterzo a sinistra
if ((Central_distance > 100) && (Right_distance < 0 || Right_distance > 30) && (Left_distance < 30)) Direzione = 3;  // sterzo a destra
if ((Central_distance < 100)) Direzione = 0;  // Fermo i motori se non ho spazio
// if ((Central_distance < 100) && (Right_distance < 0) && (Left_distance < 0)) Direzione = 0;  // Fermo i motori se non ho spazio

 Serial.print ( "Dove sto andando :");
 Serial.println (Direzione);
 Print_Distances();
 
switch (Direzione) 
    {
      case 1:
        Direzione_Dritto();
        break;
      case 2:
        Direzione_Sinistra();
        break;
      case 3:
        Direzione_Destra();
        break;
      case 0:
        Direzione_Fermo();
        break;
    }
}
void Direzione_Dritto()    
  {
    digitalWrite(Led_FermoMotori, LOW); // spengo led segnalazione fermo motori
    MotorRight.setSpeed(210);
    MotorLeft.setSpeed(210); 
    MotorRight.run(FORWARD);
    MotorLeft.run(FORWARD);
    Serial.println ( "Nessun ostacolo rilevato");
//    Print_Distances();
  }

void Direzione_Sinistra()  
  {
    digitalWrite(Led_FermoMotori, LOW); // spengo led segnalazione fermo motori
    MotorRight.setSpeed(250);
    MotorLeft.setSpeed(180); 
    MotorRight.run(FORWARD);
    MotorLeft.run(FORWARD);
    Serial.println ( "Sterzo verso sinistra");
//    Print_Distances();
  }

void Direzione_Destra()  
  {
    digitalWrite(Led_FermoMotori, LOW); // spengo led segnalazione fermo motori
    MotorRight.setSpeed(180);
    MotorLeft.setSpeed(250); 
    MotorRight.run(FORWARD);
    MotorLeft.run(FORWARD);
    Serial.println ( "Sterzo verso destra");
//    Print_Distances();
  }
  
void Direzione_Fermo()  
  {
    MotorRight.run(RELEASE);
    MotorLeft.run(RELEASE);
    loopTime = millis();
    digitalWrite(Led_FermoMotori, HIGH);
    Serial.println ( "Fermo tutto - Fermo tutto - Fermo tutto");
    delayMicroseconds(30);
//    Print_Distances();
}

// ------------------------------------------------------------------------------------------------ //

void Read_all_sensors()
{
  Read_Central();
  delay(2);
  Read_Right();
  delay(2);
  Read_Left();
  delay(2);
}

void Read_Central()//get distance from center PING
{
  digitalWrite(Central_trigPin, LOW);  
  delayMicroseconds(2); // delays are required for a succesful sensor operation.
  digitalWrite(Central_trigPin, HIGH);
  delayMicroseconds(10); //this delay is required as well!
  digitalWrite(Central_trigPin, LOW);
  Central_duration = pulseIn(Central_echoPin, HIGH);
  Central_distance = (Central_duration/2) / 29.1;// convert the distance to centimeters.
}

void Read_Right()//get distance from Right PING
{
  digitalWrite(Right_trigPin, LOW);  
  delayMicroseconds(2); // delays are required for a succesful sensor operation.
  digitalWrite(Right_trigPin, HIGH);
  delayMicroseconds(10); //this delay is required as well!
  digitalWrite(Right_trigPin, LOW);
  Right_duration = pulseIn(Central_echoPin, HIGH);
  Right_distance = (Right_duration/2) / 29.1;// convert the distance to centimeters.
}

void Read_Left()//get distance from Left PING
{
  digitalWrite(Left_trigPin, LOW);  
  delayMicroseconds(2); // delays are required for a succesful sensor operation.
  digitalWrite(Left_trigPin, HIGH);
  delayMicroseconds(10); //this delay is required as well!
  digitalWrite(Left_trigPin, LOW);
  Left_duration = pulseIn(Left_echoPin, HIGH);
  Left_distance = (Left_duration/2) / 29.1;// convert the distance to centimeters.
}

void Print_Distances()
{
  Serial.print ("Central Distance From Robot is " );
  Serial.print ( Central_distance);
  Serial.println ( " CM!");// print out the distance in centimeters.
  Serial.print ("Right Distance From Robot is " );
  Serial.print ( Right_distance);
  Serial.println ( " CM!");// print out the distance in centimeters.
  Serial.print ("Left Distance From Robot is " );
  Serial.print ( Left_distance);
  Serial.println ( " CM!");// print out the distance in centimeters.
  Serial.println ( " ");
  delay (300);
}



