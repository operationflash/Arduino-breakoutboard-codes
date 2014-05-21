/*
   Zender code om 4 integers te zenden
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Dit zijn constantes
RF24 radio(9,10); // Juiste connecties voor de tranceiver

const uint64_t pipe = 0xF0F0F0F0D2LL; // Stel communicatie adress in

const int buttonPin1 = 3;     // Dip 1 is op deze pin aangesloten
const int buttonPin2 = 4;     // Dip 2 is op deze pin aangesloten
const int buttonPin3 = 5;     // Dip 3 is op deze pin aangesloten
const int buttonPin4 = 6;     // Dip 4 is op deze pin aangesloten

// Dit zijn variabele
int Schakelaar1 = 0; // Int declaratie
int Schakelaar2 = 0; // Int declaratie
int Schakelaar3 = 0; // Int declaratie
int Schakelaar4 = 0; // Int declaratie

void setup(void)
{
  pinMode(buttonPin1, INPUT);  // Zorg dat dip 1 een input is
  pinMode(buttonPin2, INPUT);  // Zorg dat dip 2 een input is
  pinMode(buttonPin3, INPUT);  // Zorg dat dip 3 een input is
  pinMode(buttonPin4, INPUT);  // Zorg dat dip 4 een input is
  Serial.begin(57600);
  printf_begin(); // Start printf code
  radio.begin(); // Start tranceiver code
  radio.setRetries(15,15); // Tijd tussen herzendingen, # herzendingen
  radio.setDataRate( RF24_2MBPS ); // Stel de communicatiesnelheid in (RF24_250KBPS of RF24_1MBPS of RF24_2MBPS)
  radio.setChannel(76); // Stel communicatiekanaal in (0-127)
  radio.openWritingPipe(pipe); // Open communicatie lijn om te kunnen zenden
  radio.printDetails(); // Print hoe de tranceiver is ingesteld
  printf("Led zender ingesteld en klaar voor verzenden\n\r"); // Print
}

void loop(void)
{
  Schakelaar1 = digitalRead(buttonPin1); // Lees dip uit
  Schakelaar2 = digitalRead(buttonPin2); // Lees dip uit
  Schakelaar3 = digitalRead(buttonPin3); // Lees dip uit
  Schakelaar4 = digitalRead(buttonPin4); // Lees dip uit

  int Schakelaars[4] = {
    Schakelaar1,  Schakelaar2,  Schakelaar3,  Schakelaar4                                       }; // Zet ints in een array om alles in 1 keer te versturen

  Serial.print("Schakelaar1 =  "); // Voor debuggen
  Serial.print(Schakelaars[0]); // Voor debuggen
  Serial.print(",  Schakelaar2 = "); // Voor debuggen
  Serial.print(Schakelaars[1]); // Voor debuggen
  Serial.print(",  Schakelaar3 = "); // Voor debuggen
  Serial.print(Schakelaars[2]); // Voor debuggen
  Serial.print(",  Schakelaar4 = "); // Voor debuggen
  Serial.println(Schakelaars[3]); // Voor debuggen


  printf("Verstuur schakelaar posities...",Schakelaars); // Print
  bool ok = radio.write( &Schakelaars, sizeof(Schakelaars) ); // Kijk of het versturen gelukt is
  if (ok) // Als het versturen gelukt is:
    printf("Ok...\n\r"); // Print
  else // Als het versturen niet gelukt is:
  printf("Verzenden mislukt.\n\r"); // Print

  delay(25); // Vertraging om de stabiliteit te verhogen
}




















