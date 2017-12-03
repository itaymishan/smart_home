#include <Homie.h>

const int PIN_RELAY_ON = 5;
const int PIN_RELAY_OFF = 4;

HomieNode garageNode("garage", "switch");

bool garageOnHandler(const HomieRange& range, const String& value) {
  Serial.print("garageOnHandler");
  if (value != "true" && value != "false") return false;

  if(value == "true")
  {
    digitalWrite(PIN_RELAY_ON, HIGH);
    delay(5000);
    digitalWrite(PIN_RELAY_ON, LOW);
  }
  else
  {
    digitalWrite(PIN_RELAY_OFF, HIGH);
    delay(5000);
    digitalWrite(PIN_RELAY_OFF, LOW);
  }
  bool on = (value == "true");
  garageNode.setProperty("on").send(value);
  Homie.getLogger() << "Garage is " << (on ? "on" : "off") << endl;

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  pinMode(PIN_RELAY_ON, OUTPUT);
  digitalWrite(PIN_RELAY_ON, LOW);
  pinMode(PIN_RELAY_OFF, OUTPUT);
  digitalWrite(PIN_RELAY_OFF, LOW);

  Homie_setFirmware("awesome-relay", "1.0.0");

  garageNode.advertise("on").settable(garageOnHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}
