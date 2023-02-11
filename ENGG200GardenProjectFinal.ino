#include <DHT.h>;
#include <Adafruit_seesaw.h>;

Adafruit_seesaw ss;

// Constants for DHT Sensor
#define DHTPIN 7           // what pin we're connected to
#define DHTTYPE DHT11      // DHT 11
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino

// Pins
const int ledPin = 4;  // Led connected to digital pin 4
const int pumpPin = 8;

//Variables
float hum;                                //Stores humidity value
float temp;                               //Stores temperature value in Celsius
int lightState = HIGH;                     // current state of the LED
static unsigned long previousMillis = 0;  // variable to store the last time the LED was updated
unsigned long currentMillis = millis();   // current time
unsigned long previousOnMillis = 0;
unsigned long previousOffMillis = 0;

void setup() {
  Serial.begin(750);
  dht.begin();

  Serial.println("Running...");
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1) delay(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
  pinMode(ledPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
}

void csaw() {
  float tempC = ss.getTemp();
  uint16_t capRead = ss.touchRead(0);

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("*C");
  Serial.print("Capacitive: ");
  Serial.println(capRead);
}

void led_Light() {
  currentMillis = millis();
  digitalWrite(ledPin, lightState); // turn on led

  // if the LED is on and 6 hours have passed since it was turned on, then turn it off
  if (lightState == HIGH && currentMillis - previousOnMillis >= 21600000) { //6 hours on 
    previousOffMillis = currentMillis;
    lightState = LOW;
    digitalWrite(ledPin, lightState);
  }
  
  // if the LED is off and 18 hours have passed since it was turned off, then turn it on
  else if (lightState == LOW && currentMillis - previousOffMillis >= 64800000) { //18 hours off 
    previousOnMillis = currentMillis;
    lightState = HIGH;
    digitalWrite(ledPin, lightState);
  }  
}

void pump() {
  uint16_t capRead = ss.touchRead(0);
  Serial.println("90");
  if (lightState == LOW && capRead <= 400) {
    digitalWrite(pumpPin, HIGH);
    Serial.println("93");
  } else {
    digitalWrite(pumpPin, LOW);
    Serial.println("97");
  }
}

void loop() {
  led_Light();
  pump();
  csaw();
}