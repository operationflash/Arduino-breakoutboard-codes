/*
 Led ontvanger code
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Dit zijn constantes
RF24 radio(9,10); // Juiste connecties voor de tranceiver
const uint64_t pipe = 0xF0F0F0F0D2LL; // Stel communicatie adress in
byte ledPin1 = 2; // Pin waar led op is aangesloten
byte ledPin2 = 3; // Pin waar led op is aangesloten
byte ledPin3 = 4; // Pin waar led op is aangesloten
byte ledPin4 = 5; // Pin waar led op is aangesloten


void setup(void)
{
  pinMode(ledPin1, OUTPUT); // Stel pin in als output
  pinMode(ledPin2, OUTPUT); // Stel pin in als output
  pinMode(ledPin3, OUTPUT); // Stel pin in als output
  pinMode(ledPin4, OUTPUT); // Stel pin in als output 
  Serial.begin(57600); // Baud snelheid voor Seriële communicatie
  printf_begin(); // Start printf code
  radio.begin(); // Start tranceiver code
  radio.setRetries(15,15); // Tijd tussen herzendingen, # herzendingen
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
    int  vergregen_data[4]; // Dit is een array om de ontvangen data in op te slaan
    bool done = false; // Boolean done is false
    while (!done) // Zolang done niet true is doe...
    {
      done = radio.read( &vergregen_data, sizeof(vergregen_data) ); // Haal de data binnen en kijk of dat de laatste data was
      printf("Data binnengekregen: "); // Voor debuggen
      Serial.print("DIP 1 = "); // Voor debuggen
      Serial.print(vergregen_data[0]); // Voor debuggen
      Serial.print(", DIP 2 = "); // Voor debuggen
      Serial.print(vergregen_data[1]); // Voor debuggen
      Serial.print(", DIP 3 = "); // Voor debuggen
      Serial.print(vergregen_data[2]); // Voor debuggen
      Serial.print(", DIP 4 = "); // Voor debuggen
      Serial.println(vergregen_data[3]); // Voor debuggen


      digitalWrite(ledPin1, vergregen_data[0]); // Geef hoog of laag signaal als output, afhankelijk van de verkregen data
      digitalWrite(ledPin2, vergregen_data[1]); // Geef hoog of laag signaal als output, afhankelijk van de verkregen data
      digitalWrite(ledPin3, vergregen_data[2]); // Geef hoog of laag signaal als output, afhankelijk van de verkregen data
      digitalWrite(ledPin4, vergregen_data[3]); // Geef hoog of laag signaal als output, afhankelijk van de verkregen data

      delay(10); // Vertraging om de stabiliteit te verhogen
    }
  }
}










