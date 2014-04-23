const int analogInPin = A7;  // Hier is de pot-meter op aangesloten
const int analogOutPin = 9; // Hier is de led op aangesloten

int sensorWaarde = 0;        // waarde uit ad-converter
int outputWaarde = 0;        // PWM-waarde integer introduceren

void setup() {
  // Start seriele communicatie
  Serial.begin(9600); // Baud snelheid voor seriele communicatie
}

void loop() {
  // read the analog in value:
  sensorWaarde = analogRead(analogInPin);
  outputWaarde = map(sensorWaarde, 0, 1023, 0, 255);   //  Vertaal de waardes van de potmeter naar PWM waardes
  analogWrite(analogOutPin, outputWaarde);             // Zorgt voor PWM pulsen naar de led

  // print het volgende in de seriele monitor
  Serial.print("sensor = " );
  Serial.print(sensorWaarde);
  Serial.print("\t output = ");
  Serial.println(outputWaarde);

  delay(5);                              // Aantal miliseconde pause in het programma
}


