#include <ArduinoBLE.h>

BLEService flexService("1101");
BLEUnsignedCharCharacteristic flexLevelChar("2101", BLERead | BLENotify);

int flexin = A4;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  if(!BLE.begin()) {
    Serial.println("Starting BLE Failed!");
    while(1);
  }

  BLE.setLocalName("Dr.Turtle");
  BLE.setAdvertisedService(flexService);
  flexService.addCharacteristic(flexLevelChar);
  BLE.addService(flexService);

  BLE.advertise();
  Serial.println("Bluetooth Device Active, Waiting for Connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if(central) {
    Serial.print("Connected to Central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

    while(central.connected()) {
      int flex = map(analogRead(flexin),90,310,0,90);
//      int flex = analogRead(flexin);
      flexLevelChar.writeValue(flex);

//      while (!APDS.proximityAvailable()) {};
//      int Proximity = APDS.readProximity();
      Serial.print("flex = "); 
      Serial.println(flex);
      
//      flexLevelChar.writeValue(Proximity);
      delay(200);
    }
  }

  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected from Central: ");
  Serial.println(central.address());
}
