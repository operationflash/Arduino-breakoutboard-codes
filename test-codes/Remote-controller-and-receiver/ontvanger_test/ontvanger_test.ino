/*
 Ontvanger code
 Ontvangt 5 integers per keer
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
int outputVoor = 0;
int outputAchter = 0;
int outputLinks = 0;
int outputRechts = 0;
int outputPower = 0;
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
  pinMode(3, OUTPUT); // Stel pin in als output
  pinMode(6, OUTPUT); // Stel pin in als output
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
      printf("Data binnengekregen: "); // Voor debuggen
      Serial.print("Linker y-axis = "); // Voor debuggen
      Serial.print(got_time[0]); // Voor debuggen
      Serial.print(", Linker trim = "); // Voor debuggen
      Serial.print(got_time[1]); // Voor debuggen
      Serial.print(", Rechter y-axis = "); // Voor debuggen
      Serial.print(got_time[2]); // Voor debuggen
      Serial.print(", Rechter x-axis = "); // Voor debuggen
      Serial.print(got_time[3]); // Voor debuggen
      Serial.print(", Rechter trim = "); // Voor debuggen
      Serial.println(got_time[4]); // Voor debuggen


      outputPower = map(got_time[0], 200, 1023, 225, 0); // Verander de input waarden naar waarden van 0 tot 255
      if (got_time[0] < 200) // Mocht de y-as van de joystick toch een lagere waarde krijgen dat willen we toch maximaal achteruit
      {
        outputAchter = 225;
      }

      /*
       708 is de normale positie van de y-as van de rechter joystick bij de controller waar deze code voor wordt gemaakt
       225 is de laagste waarde die de y-s van de rechter joystick krijgt
       */

      if (got_time[2] > 708) // We willen naar voren als de joystick omhoog geduwd wordt
      {
        outputVoor = map(got_time[2], 708, 1023, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
        outputAchter = 0; // Belangrijk voor de h-brug die we gebruiken
      }
      else if (got_time[2] < 708) // We willen naar achter als de joystick omlaag geduwd wordt
      {
        outputVoor = 0; // Belangrijk voor de h-brug die we gebruiken
        outputAchter = map(got_time[2], 707, 225, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
        if (got_time[2] < 225) // Mocht de y-as van de joystick toch een lagere waarde krijgen dat willen we toch maximaal achteruit
        {
          outputAchter = 225;
        }
      }

      /*
       705 is de normale positie van de x-as van de rechter joystick bij de controller waar deze code voor wordt gemaakt
       169 is de laagste waarde die de x-as van de rechter joystick krijgt
       */


      if (got_time[3] > 705) // We willen naar voren als de joystick omhoog geduwd wordt
      {
        outputRechts = map(got_time[3], 705, 1023, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
        outputLinks = 0; // Belangrijk voor de h-brug die we gebruiken
      }
      else if (got_time[3] < 708) // We willen naar achter als de joystick omlaag geduwd wordt
      {
        outputRechts = 0; // Belangrijk voor de h-brug die we gebruiken
        outputLinks = map(got_time[3], 705, 169, 0, 255); // Verander de input waarden naar waarden van 0 tot 255
        if (got_time[3] < 169) // Mocht de y-as van de joystick toch een lagere waarde krijgen dat willen we toch maximaal achteruit
        {
          outputLinks = 225;
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

      analogWrite(6, outputVoor); // Voor debuggen
      analogWrite(3, outputAchter); // Voor debuggen


      delay(5); // Vertraging om de stabiliteit te verhogen
    }
  }
}





















