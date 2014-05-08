/*
   Afstandsbedinieng code
 Ondersteund momenteel 5 pot's
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10); // Juiste connecties voor de tranceiver
const uint64_t pipe = 0xF0F0F0F0D2LL; // Stel communicatie adress in

void setup(void)
{

  Serial.begin(57600);
  printf_begin(); // Start printf code
  radio.begin(); // Start tranceiver code
  radio.setRetries(15,15); // tijd tussen herzendingen, # herzendingen
  radio.setDataRate( RF24_2MBPS ); // Stel de communicatiesnelheid in (RF24_250KBPS of RF24_1MBPS of RF24_2MBPS)
  radio.setChannel(76); // Stel communicatiekanaal in (0-127)
  radio.openWritingPipe(pipe); // Open communicatie lijn om te kunnen zenden
  radio.printDetails(); // Print hoe de tranceiver is ingesteld
  printf("De afstandsbediening is ingesteld\n\r"); // Print
}

void loop(void)
{
  int RX = analogRead(A5); // Lees adc uit en zet waarde om in een integer
  int RY = analogRead(A4); // Lees adc uit en zet waarde om in een integer
  int LY = analogRead(A3); // Lees adc uit en zet waarde om in een integer
  int RTrim = analogRead(A2); // Lees adc uit en zet waarde om in een integer
  int LTrim = analogRead(A1); // Lees adc uit en zet waarde om in een integer
  int waarde[5] = {
    LY, LTrim, RY, RX, RTrim                   }; // Zet alle integers in een array om alles in 1 keer te versturen
  Serial.print("Linker y-axis = "); // Voor debuggen
  Serial.print(waarde[0]); // Voor debuggen
  Serial.print(", Linker trim = "); // Voor debuggen
  Serial.print(waarde[1]); // Voor debuggen
  Serial.print(", Rechter y-axis = "); // Voor debuggen
  Serial.print(waarde[2]); // Voor debuggen
  Serial.print(", Rechter x-axis = "); // Voor debuggen
  Serial.print(waarde[3]); // Voor debuggen
  Serial.print(", Rechter trim = "); // Voor debuggen
  Serial.println(waarde[4]); // Voor debuggen

  printf("Verstuur pot posities...",waarde); // Print
  bool ok = radio.write( &waarde, sizeof(waarde) ); // Kijk of het versturen gelukt is
  if (ok) // Als het versturen gelukt is:
    printf("ok...\n\r"); // Print
  else // Als het versturen niet gelukt is:
  printf("failed.\n\r"); // Print

  delay(25); // Vertraging om de stabiliteit te verhogen
}











