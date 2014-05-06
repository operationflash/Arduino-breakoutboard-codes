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
  int waarde1 = analogRead(A5); // Lees adc uit en zet waarde om in een integer
  int waarde2 = analogRead(A4); // Lees adc uit en zet waarde om in een integer
  int waarde3 = analogRead(A3); // Lees adc uit en zet waarde om in een integer
  int waarde4 = analogRead(A2); // Lees adc uit en zet waarde om in een integer
  int waarde5 = analogRead(A1); // Lees adc uit en zet waarde om in een integer
  int waarde[5] = {
    waarde1, waarde2, waarde3, waarde4, waarde5           }; // Zet alle integers in een array om alles in 1 keer te versturen
  Serial.print("RX = ");
  Serial.print(waarde[0]); // Laat de uitgelezen waarde zien voor debuggen
  Serial.print(", RY = ");
  Serial.print(waarde[1]); // Laat de uitgelezen waarde zien voor debuggen
  Serial.print(", LY = ");
  Serial.print(waarde[2]); // Laat de uitgelezen waarde zien voor debuggen
  Serial.print(", R trimm = ");
  Serial.print(waarde[3]); // Laat de uitgelezen waarde zien voor debuggen
  Serial.print(", L trim = ");
  Serial.println(waarde[4]); // Laat de uitgelezen waarde zien voor debuggen

  printf("Verstuur pot posities...",waarde); // Print
  bool ok = radio.write( &waarde, sizeof(waarde) ); // Kijk of het versturen gelukt is
  if (ok) // Als het versturen gelukt is:
    printf("ok...\n\r"); // Print
  else // Als het versturen niet gelukt is:
  printf("failed.\n\r"); // Print

  delay(25); // Vertraging om de stabiliteit te verhogen
}







