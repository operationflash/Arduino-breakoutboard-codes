/*
 Ontvanger code
 Ontvangt 5 integers per keer
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Alle declaraties voor in de sketch
bool timeout; // timeout is een boolean
byte outputVoor = 0;
byte outputAchter = 0;
byte outputLinks = 0;
byte outputRechts = 0;
byte outputPower = 0;
int trimAdjust1 = 0;
int trimAdjust2 = 0;
byte maxTijd = 75; // Maximale wachtijd voor ontvangst van data voor timeout
const byte powerPoort = 5; // Power control pin
const byte HAPoort = 6; // H-brug control pin 1
const byte HVPoort = 3; //  H-brug control pin 2
const byte RXM = 169; // Minimale waarde van de x-as van de rechter joystick
const byte RYM = 255; // Minimale waarde van de x-as van de rechter joystick
const byte LYM = 200; // Minimale waarde van de x-as van de rechter joystick
const int RXB = 705; // Rustpositie waarde van de x-as van de rechter joystick
const int RYB = 708; // Rustpositie waarde van de y-as van de rechter joystick
int got_data[5]; // Dit is een array om de ontvangen data in op te slaan
unsigned long startTijd;

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
  printf("De ontvanger is ingesteld\n\r"); // Voor debuggen
  printf("Nu wachten op data...\n\r"); // Voor debuggen
}

void loop(void)
{
  timeout = false; // Zet timeout boolean naar 0
  startTijd = millis(); // Starttijd is uptime Arduino

  while ( ! radio.available() && ! timeout ) // Als er geen rimeout is en radio is niet beschikbaar doe...
    if (millis() - startTijd > maxTijd ) // Controllerd of de maximale wachttijd verstreken is
      timeout = true; // timeout is waar

  if ( timeout ) // Als er een timeout is
  {
    Serial.println("Geen ontvangst, NOODSTOP"); // Voor debuggen
    analogWrite(powerPoort, 0); // Stuur uit signaal naar de hoofdrotors
    analogWrite(HAPoort, 0); // Stuur uit signaal naar h-brug
    analogWrite(HVPoort, 0); // Stuur uit signaal naar h-brug
  }
  else // Als er data wordt ontvangen.
  {
    radio.read( &got_data, sizeof(got_data) ); // Lees ontvangen data uit
    printf("Data binnengekregen: "); // Voor debuggen
    Serial.print("Linker y-axis = "); // Voor debuggen
    Serial.print(got_data[0]); // Voor debuggen
    Serial.print(", Linker trim = "); // Voor debuggen
    Serial.print(got_data[1]); // Voor debuggen
    Serial.print(", Rechter y-axis = "); // Voor debuggen
    Serial.print(got_data[2]); // Voor debuggen
    Serial.print(", Rechter x-axis = "); // Voor debuggen
    Serial.print(got_data[3]); // Voor debuggen
    Serial.print(", Rechter trim = "); // Voor debuggen
    Serial.println(got_data[4]); // Voor debuggen

    trimAdjust1 = map(got_data[1], 175, 1023, -100, 100); // Remap the left trim for adjusting the PWM signals
    trimAdjust2 = map(got_data[4], 110, 1023, -100, 100); // Remap the right trim for adjusting the PWM signals
    got_data[2] = got_data[2] + trimAdjust1; // Manupileer de y-as van de rechter joystick met de rechter trim waarde



    outputPower = map(got_data[0], LYM, 1023, 255, 0); // Verander de input waarden naar waarden van 0 tot 255
    if (got_data[0] < LYM) // Mocht de y-as van de joystick toch een lagere waarde krijgen dat willen we toch maximaal achteruit
    {
      outputAchter = 255;
    }

    if (got_data[2] >= RYB) // We willen naar voren als de joystick omhoog geduwd wordt
    {
      outputVoor = map(got_data[2], RYB, 1123, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
      outputAchter = 0; // Belangrijk voor de h-brug die we gebruiken
    }
    else if (got_data[2] < RYB) // We willen naar achter als de joystick omlaag geduwd wordt
    {
      outputVoor = 0; // Belangrijk voor de h-brug die we gebruiken
      outputAchter = map(got_data[2], RYB-100, RYM, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
      if (got_data[2] < RYM) // Mocht de y-as van de joystick toch een lagere waarde krijgen dat willen we toch maximaal achteruit
      {
        outputAchter = 255;
      }
    }

    if (got_data[3] >= RXB) // We willen naar voren als de joystick omhoog geduwd wordt
    {
      outputRechts = map(got_data[3], RXB, 1023, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
      outputLinks = 0; // Belangrijk voor de h-brug die we gebruiken
    }
    else if (got_data[3] < RXB) // We willen naar achter als de joystick omlaag geduwd wordt
    {
      outputRechts = 0; // Belangrijk voor de h-brug die we gebruiken
      outputLinks = map(got_data[3], RXB, RXM, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
      if (got_data[3] < RXM) // Mocht de x-as van de joystick toch een lagere waarde krijgen dat willen we toch maximaal achteruit
      {
        outputLinks = 255;
      }
    }

    Serial.print(outputPower); // Voor debuggen
    Serial.print(", "); // Voor debuggen
    Serial.print(outputVoor); // Voor debuggen
    Serial.print(", "); // Voor debuggen
    Serial.print(outputAchter); // Voor debuggen
    Serial.print(", "); // Voor debuggen
    Serial.print(outputLinks); // Voor debuggen
    Serial.print(", "); // Voor debuggen
    Serial.println(outputRechts); // Voor debuggen

    analogWrite(powerPoort, outputPower); // Stuur PWM signaal naar de hoofdrotors
    analogWrite(HAPoort, outputAchter); // Stuur PWM signaal naar h-brug
    analogWrite(HVPoort, outputVoor); // Stuur PWM signaal naar h-brug

    delay(5); // Vertraging om de stabiliteit te verhogen
  }
}















