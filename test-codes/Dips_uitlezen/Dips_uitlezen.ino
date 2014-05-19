const int buttonPin1 = 3;     // dip 1 is op deze pin aangesloten
const int buttonPin2 = 4;     // dip 2 is op deze pin aangesloten
const int buttonPin3 = 5;     // dip 3 is op deze pin aangesloten
const int buttonPin4 = 6;     // dip 4 is op deze pin aangesloten
const int ledPin =  9;        // de led is op deze pin aangesloten

// variables will change:
byte buttonState1 = 0;         // variabele voor het uitlezen van dip 1
byte buttonState2 = 0;         // variabele voor het uitlezen van dip 1
byte buttonState3 = 0;         // variabele voor het uitlezen van dip 1
byte buttonState4 = 0;         // variabele voor het uitlezen van dip 1

void setup() {

  pinMode(ledPin, OUTPUT);     // Zorg dat de led een output is
  pinMode(buttonPin1, INPUT);  // Zorg dat dip 1 een input is
  pinMode(buttonPin2, INPUT);  // Zorg dat dip 2 een input is
  pinMode(buttonPin3, INPUT);  // Zorg dat dip 3 een input is
  pinMode(buttonPin4, INPUT);  // Zorg dat dip 4 een input is

  // Start seriele communicatie
  Serial.begin(9600); // Baud snelheid voor seriele communicatie
}

void loop(){
  // Lees de dips uit:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

  // Kijk welke dips aan staan en vervolgens de bijbehorende actie starten
  if (buttonState1 == HIGH) {
    // Laat LED knipperen:
    Serial.println("Dip 1 is hoog" );  // print dit in de seriele monitor
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);

  }
  if (buttonState2 == HIGH) {
    // Laat LED knipperen:
    Serial.println("Dip 2 is hoog" );  // print dit in de seriele monitor
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);

  }
  if (buttonState3 == HIGH) {
    // Laat LED knipperen:
    Serial.println("Dip 3 is hoog" );  // print dit in de seriele monitor
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
  }
  if (buttonState4 == HIGH) {
    // Laat LED knipperen:
    Serial.println("Dip 4 is hoog" );  // print dit in de seriele monitor
    digitalWrite(ledPin, HIGH);
    delay(125);
    digitalWrite(ledPin, LOW);
    delay(125);
  }
  else {
    // Doe de LED uit:
    Serial.println("Dips zijn allemaal laag" );  // print dit in de seriele monitor
    digitalWrite(ledPin, LOW);
  }
}



