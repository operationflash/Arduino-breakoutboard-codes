
const int analogInPin = A6; //Analoge port waarop het raw voltage wordt gemeten
int sensorWaarde = 0;        // waarde uit ad-converter
float Voltage = 1;          // introduceer de float genaamd Voltage

void setup() {
  // Start seriele communicatie
  Serial.begin(9600); // Baud snelheid voor seriele communicatie
}

void loop() {
  sensorWaarde = analogRead(analogInPin);  // Lees de ad converter uit en ken waarde sensorWaarde toe


  Voltage = sensorWaarde *(6.6/1023);      // Het voltage berkenen

  // print het volgende in de seriele monitor
  Serial.print("sensor = " );
  Serial.print(sensorWaarde);
  Serial.print("\t Raw Voltage = ");
  Serial.print(Voltage);
  Serial.println(" Volt");

  delay(5);                              // Aantal miliseconde pause in het programma
}

