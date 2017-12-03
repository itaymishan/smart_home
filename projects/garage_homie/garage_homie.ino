#include <Homie.h>

const int PIN_RELAY = 5;
const int LED_INDICATOR = 4;

HomieNode garageNode("garage", "switch");

bool garageOnHandler(const HomieRange& range, const String& value) {
  Serial.print("garageOnHandler");
  if (value != "true" && value != "false") return false;

  bool on = (value == "true");
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  digitalWrite(LED_INDICATOR, on ? HIGH : LOW);
  garageNode.setProperty("on").send(value);
  Homie.getLogger() << "Garage is " << (on ? "on" : "off") << endl;

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  pinMode(LED_INDICATOR, OUTPUT);
  digitalWrite(LED_INDICATOR, LOW);

  Homie_setFirmware("awesome-relay", "1.0.0");

  garageNode.advertise("on").settable(garageOnHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}
