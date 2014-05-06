/*
 Ontvanger code
 Ontvangt 5 integers per keer
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10); // Juiste connecties voor de tranceiver
const uint64_t pipe = 0xF0F0F0F0D2LL; // Stel communicatie adress in

void setup(void)
{
  Serial.begin(57600); // Baud snelheid voor Seriële communicatie
  printf_begin(); // Start printf code
  radio.begin(); // Start tranceiver code
  radio.setRetries(15,15); // tijd tussen herzendingen, # herzendingen
  radio.setDataRate( RF24_2MBPS ); // Stel de communicatiesnelheid in (RF24_250KBPS of RF24_1MBPS of RF24_2MBPS)
  radio.setChannel(76); // Stel communicatiekanaal in (0-127)
  radio.openReadingPipe(1,pipe); // Open communicatie lijn om te kunnen lezen
  radio.startListening(); // Start met luisteren naar data
  radio.printDetails(); // Print hoe de tranceiver is ingesteld
  printf("De ontvanger is ingesteld\n\r"); // Print
  printf("Nu wachten op data...\n\r"); // Print
}

void loop(void)
{
  if ( radio.available() ) // Is de data klaar?
  {
    // Dump the payloads until we've gotten everything
    int got_time[5]; // Dit is een array om de ontvangen data in op te slaan
    bool done = false; // boolean done is false
    while (!done) //zolang done niet true is doe...
    {
      done = radio.read( &got_time, sizeof(got_time) ); // Haal de data binnen en kijk of dat de laatste data was
      printf("Data binnengekregen: ");
      Serial.print("RX = ");
      Serial.print(got_time[0]);
      Serial.print(", RY = ");
      Serial.print(got_time[1]);
      Serial.print(", LY = ");
      Serial.print(got_time[2]);
      Serial.print(", R trimm = ");
      Serial.print(got_time[3]);
      Serial.print(", L trim = ");
      Serial.println(got_time[4]);

      delay(5); // Vertraging om de stabiliteit te verhogen
    }
  }
}







