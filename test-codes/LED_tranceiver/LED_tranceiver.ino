///---Benodigde librarys---///
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
///------///

RF24 radio(9,10); // Stel de nRF24L01+ module in op de  pinnen 9 & 10

const int RolPin = 3; // Schakelaar op pin 3 om de rol in te stellen
const int Signaal = 2; // Pin om ontvangs signaal in te stellen (zoals led of speaker)
const uint64_t kanalen[2] = {
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; //Stel 2 communicatie kanalen in


typedef enum {
  basis_station = 1, antwoord_station } // De 2 verschilende rollen die de code aankan
role_e;

const char* role_friendly_name[] = {
  "Probleem", "Basis station", "Antwoord station"}; // De rol namen voor Seriële communicatie

role_e rol; // De huidige rol die gerbuikt wordt

void setup(void)
{
  pinMode(RolPin, INPUT); // Stel RolPin in als input
  digitalWrite(RolPin,HIGH);
  delay(20); // Zorgt voor een stabielere uitlezing van de RolPin

  ///--- Bepaal welke rol er aangenomen wordt [1]---///
  if ( ! digitalRead(RolPin) ) // "!" betekend niet
    rol = basis_station;
  else
    rol = antwoord_station;
  ///---[1]---///

  Serial.begin(57600); // Baud snelheid voor Seriële communicatie
  printf_begin();
  printf("ROL: %s\n\r",role_friendly_name[rol]);

  ///--- Configureer de nRF24L01+ chip [2]---///
  radio.begin();
  //  radio.setChannel(126); // Welk communicatiekanaal word er gebruikt (0-127 beschikbaar)
  radio.setDataRate( RF24_2MBPS ); // Zet de overdrachtssnelheid.  (RF24_250KBPS of RF24_1MBPS of RF24_2MBPS)
  radio.setRetries(15,15); // Stel wachttijd en aantal pogingen in om te herzenden mocht er geen antwoord komen van het andere station
  radio.setPayloadSize(8); // Stel de grote van de payload in


  if ( rol == basis_station ) // Stel de kanalen in naar hun rol
  {
    radio.openWritingPipe(kanalen[0]); // Primaire kanaal is voor versturen
    radio.openReadingPipe(1,kanalen[1]); // Secundaire kanaal is voor ontvangen
  }
  else
  {
    radio.openReadingPipe(1,kanalen[0]); // Primaire kanaal is voor ontvangen
    radio.openWritingPipe(kanalen[1]); // Secundaire kanaal is voor versturen
  }

  radio.startListening(); // Start met luisteren
  radio.printDetails(); // Print de chip instellingen
  ///---[2]---///
}

void loop(void)
{
  ///---Basis station code [3]---///
  if (rol == basis_station)
  {
    radio.stopListening(); // Stop met luisteren om te versturen

    unsigned long tijd = millis(); // Stel de arduino aan-tijd in als tijd
    printf("Verzonden op %lums, ",tijd);
    bool ok = radio.write( &tijd, sizeof(unsigned long) );

    if (ok) // Controleer of er een antwoord is van het antwoord station
    {
      printf("Versturen gelukt\n\r");
      tone(Signaal, 1000, 250); // Zorgt voor een toon(pin, frequentie, duur)
    }
    else
      printf("Versturen mislukt\n\r");

    radio.startListening(); // Start weer met luisteren

    ///--- Wachten op antwoord of op de maximale wachttijd (200ms) [4]---///
    unsigned long verstuurTijd = millis(); // Stel in wanneer de radio verstuurd heeft
    bool max_wachttijd = false; // Maak een bolean van max_wachtijd
    while ( ! radio.available() && ! max_wachttijd ) // Zolang de radio niet ontvangen heeft:
      if (millis() - verstuurTijd > 200 ) // Check of de maximale wachttijd verstreken is
        max_wachttijd = true; // Max wachttijd is waar
    ///---[4]---///

    if ( max_wachttijd ) // Als de maximale wachttijd verstreken is:
    {
      printf("Bericht niet ontvangen, maximale wachttijd verstreken.\n\r");
    }
    else // Als het versturen gelukt is:
    {
      unsigned long ontvangen_op; // Stel de arduino aan-tijd in als tijd dat het bericht ontvangen is
      radio.read( &ontvangen_op, sizeof(unsigned long) ); // Lees het ontvangen vericht
      printf("Ontvangen door antwoord station op: %lums, Antwoord na: %lums\n\r",ontvangen_op,millis()-ontvangen_op);
    }

    delay(250); // Wacht
  }
  ///---[3]---///

  ///---Antwoord station code [5]---///

  if ( rol == antwoord_station )
  {
    if ( radio.available() ) // Kijk of er data uit te lezen valt
    {
      ///---Het volledige bericht uitlezen [6]---///
      unsigned long ontvangen_op;
      bool done = false;
      while (!done)
      {
        done = radio.read( &ontvangen_op, sizeof(unsigned long) ); // Lees data uit en kijk of er nog meer data aankomt

        printf("Bericht intvangen: %lums",ontvangen_op); // Print de gevonden data

        delay(10); // Vertraging om het basis station tijd te geven om te gaan luisteren
      }
      ///---[6]---///

      tone(Signaal, 500, 250); // Zorgt voor een toon(pin, frequentie, duur)
      radio.stopListening(); // Stop met luisteren
      radio.write( &ontvangen_op, sizeof(unsigned long) ); // Verstuur data terug
      printf("Antwoord verstuurd\n\r");

      radio.startListening(); // Begin weer met luisteren
    }
  }
  ///---[5]---///
}


























