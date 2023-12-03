#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80); // 80 is the port number

const char* ssid = "********";//SSID 
const char* password = "Your Password";//Password

int buttonPin = 10; //  Push button is connected to GPIO pin 10
bool lightsState = LOW; // Initially, lights are off

void toggleLights() {
  lightsState = !lightsState; // Toggle the lights state
  digitalWrite(5, lightsState);
  digitalWrite(4, lightsState);
  digitalWrite(0, lightsState);
  digitalWrite(2, lightsState);

  if (lightsState) {
    server.send(200, "text/html", "Lights turned on");
  } else {
    server.send(200, "text/html", "Lights turned off");
  }
}
void flashDefaultLED() {
    // Configure the default onboard LED pin (D0 or GPIO16) as an output
    pinMode(16, OUTPUT);

    // Flash the LED three times with a short delay in between
    for (int i = 0; i < 3; i++) {
        digitalWrite(16, HIGH);   // Turn on the LED
        delay(500);                // Delay for 500 milliseconds (0.5 seconds)
        digitalWrite(16, LOW);    // Turn off the LED
        delay(500);                // Delay for another 500 milliseconds
    }

    // Turn off the LED after flashing
    digitalWrite(16, LOW);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/toggleLights", toggleLights);

  server.begin();

  pinMode(5, OUTPUT); // D1
  pinMode(4, OUTPUT); // D2
  pinMode(0, OUTPUT); // D3
  pinMode(2, OUTPUT); // D4
  pinMode(buttonPin, INPUT_PULLUP); 
    // Flash the default onboard LED
    flashDefaultLED();
}

void loop() {
  server.handleClient();
  // Check for button press
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(buttonPin) == LOW) {
      toggleLights();
      delay(1000); // Debounce delay to avoid multiple toggles
    }
  }
}
