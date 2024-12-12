#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <Preferences.h>
#include <ESPmDNS.h>

// LED Configuration
#define LED_PIN     D0
#define LED_COUNT   72
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB

CRGB leds[LED_COUNT];
bool isOn = true;
int brightness = 128;
int animationMode = 0;
bool direction = true;
int animationSpeed = 50; // Speed in milliseconds

// Wi-Fi Configurations
const char* apSSID = "Lampy-Hotspot";
const char* apPassword = "password123";
const char* mdnsName = "lampy";
Preferences preferences;
AsyncWebServer server(80);

// Track Wi-Fi connection state
bool wifiConnected = false;

void setup() {
  Serial.begin(115200);

  // Initialize FastLED
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  // Initialize Preferences
  preferences.begin("lampy", false);

  // Check for saved Wi-Fi credentials
  String savedSSID = preferences.getString("wifi_ssid", "");
  String savedPassword = preferences.getString("wifi_password", "");

  if (savedSSID != "" && savedPassword != "") {
    // Attempt to connect to saved Wi-Fi credentials
    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
    Serial.printf("Connecting to Wi-Fi: %s\n", savedSSID.c_str());

    // Wait for connection (max 30 seconds)
    for (int i = 0; i < 30; i++) {
      if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        break;
      }
      delay(1000);
      Serial.print(".");
    }
  }

  if (wifiConnected) {
    Serial.println("\nWi-Fi Connected!");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());

    // Start mDNS
    if (!MDNS.begin(mdnsName)) {
      Serial.println("Error setting up mDNS responder!");
    } else {
      Serial.printf("mDNS responder started: http://%s.local\n", mdnsName);
    }

    // Start control server
    startControlServer();
  } else {
    // Failed to connect to Wi-Fi, start in Access Point mode
    startAPMode();
  }
}


// Animation Modes
void fire() {
  static byte heat[LED_COUNT];
  for (int i = 0; i < LED_COUNT; i++) {
    heat[i] = max(0, (int)(heat[i] - random(0, ((55 * 10) / LED_COUNT) + 2)));
  }
  for (int i = LED_COUNT - 1; i >= 2; i--) {
    heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
  }
  if (random(255) < 120) {
    heat[random(7)] = min(255, (int)(heat[random(7)] + random(160, 255)));
  }
  for (int i = 0; i < LED_COUNT; i++) {
    byte temp = heat[i];
    leds[i] = HeatColor(temp);
  }
}

void rainbow() {
  static uint8_t hue = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CHSV(hue + (direction ? i : -i) * 10, 255, 255);
  }
  hue++;
}

void firefly() {
  static byte fireflyBrightness[LED_COUNT] = {0};
  static float pulseCycle[LED_COUNT] = {0};
  static bool isActive[LED_COUNT] = {0};
  for (int i = 0; i < LED_COUNT; i++) {
    if (fireflyBrightness[i] > 0) {
      pulseCycle[i] += 0.1;
      float pulseValue = sin(pulseCycle[i]) * 0.5 + 0.5;
      leds[i] = CHSV(64, 255, fireflyBrightness[i] * pulseValue);
      fireflyBrightness[i] -= (fireflyBrightness[i] > 5) ? 1 : 0;
    } else if (random(100) < 5) {
      fireflyBrightness[i] = 255;
      pulseCycle[i] = 0;
    }
  }
}

void water() {
  static uint8_t waveOffset = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CHSV(160, 255, (sin8(waveOffset + i * 10) / 2) + 128);
  }
  waveOffset++;
}

void matrix() {
  static byte rain[LED_COUNT] = {0};
  for (int i = 0; i < LED_COUNT; i++) {
    if (rain[i] > 0) {
      rain[i] -= 10;
      leds[i] = CHSV(85, 255, rain[i]);
    } else if (random(100) < 5) {
      rain[i] = 255;
    }
  }
}

void aster() {
  static uint8_t position = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CHSV(position + i * 5, 255, 255);
  }
  position++;
}

void mandarin() {
  static uint8_t cycle = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = (i % 2 == 0) ? CRGB(255, 69, 0) : CRGB(34, 139, 34);
  }
  cycle++;
}

void loop() {
  if (!isOn) {
    fill_solid(leds, LED_COUNT, CRGB::Black);
    FastLED.show();
    return;
  }

  switch (animationMode) {
    case 0:
      fire();
      break;
    case 1:
      rainbow();
      break;
    case 2:
      firefly();
      break;
    case 3:
      water();
      break;
    case 4:
      matrix();
      break;
    case 5:
      aster();
      break;
    case 6:
      mandarin();
      break;
  }

  FastLED.show();
  delay(animationSpeed);
}

void startAPMode() {
  WiFi.softAP(apSSID, apPassword);
  IPAddress IP = WiFi.softAPIP();
  Serial.printf("AP Mode: Connect to %s (%s)\n", apSSID, IP.toString().c_str());

  // Serve Wi-Fi configuration page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html lang="en">
      <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Wi-Fi Setup</title>
      </head>
      <body>
        <h1>Wi-Fi Setup</h1>
        <form action="/save" method="POST">
          <label for="ssid">SSID:</label><br>
          <input type="text" id="ssid" name="ssid"><br>
          <label for="password">Password:</label><br>
          <input type="password" id="password" name="password"><br><br>
          <input type="submit" value="Save">
        </form>
      </body>
      </html>
    )rawliteral");
  });

  // Save Wi-Fi credentials
  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
      String ssid = request->getParam("ssid", true)->value();
      String password = request->getParam("password", true)->value();

      preferences.putString("wifi_ssid", ssid);
      preferences.putString("wifi_password", password);

      request->send(200, "text/plain", "Credentials saved. Rebooting...");

      delay(1000);
      ESP.restart();
    } else {
      request->send(400, "text/plain", "Invalid input");
    }
  });

  server.begin();
}

void startControlServer() {
  // Serve Vue.js control app
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html lang="en">
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Lampy</title>
        <script src="https://cdn.jsdelivr.net/npm/vue@2.6.14/dist/vue.js"></script>
        <script src="https://cdn.tailwindcss.com"></script>
      </head>
      <body class="bg-gray-900 text-white">
        <div id="app" class="container mx-auto py-10">
          <div class="max-w-md mx-auto bg-gray-800 p-5 rounded-lg shadow-md">
            <h1 class="text-xl font-bold mb-4">Lampy Control</h1>
            <label class="flex items-center mb-4">
              <input type="checkbox" v-model="power" @change="togglePower" class="mr-2"> On/Off
            </label>
            <div class="mb-4">
              <label>Brightness:</label>
              <input type="range" v-model="brightness" min="0" max="255" @input="updateBrightness" class="w-full">
            </div>
            <div class="mb-4">
              <label>Animation Mode:</label>
              <select v-model="mode" @change="changeMode" class="w-full bg-gray-700 text-white">
                <option v-for="animation in animations" :value="animation.value">
                  {{ animation.text }}
                </option>
              </select>
            </div>
            <label class="flex items-center mb-4">
              <input type="checkbox" v-model="direction" @change="toggleDirection" class="mr-2">
              {{ direction ? 'Forward' : 'Backward' }}
            </label>
            <div class="mb-4">
              <label>Animation Speed:</label>
              <input type="range" v-model="speed" min="10" max="200" @input="updateSpeed" class="w-full">
            </div>
          </div>
        </div>
        <script>
          new Vue({
            el: '#app',
            data: {
              power: true,
              brightness: 128,
              mode: 0,
              direction: true,
              speed: 50,
              animations: [
                { text: 'Fire', value: 0 },
                { text: 'Rainbow', value: 1 },
                { text: 'Firefly', value: 2 },
                { text: 'Water', value: 3 },
                { text: 'Matrix Rain', value: 4 },
                { text: 'Aster', value: 5 },
                { text: 'Mandarin', value: 6 }
              ]
            },
            methods: {
              togglePower() {
                fetch(`/power?state=${this.power}`);
              },
              updateBrightness() {
                fetch(`/brightness?value=${this.brightness}`);
              },
              changeMode() {
                fetch(`/mode?value=${this.mode}`);
              },
              toggleDirection() {
                fetch(`/direction?value=${this.direction}`);
              },
              updateSpeed() {
                fetch(`/speed?value=${this.speed}`);
              }
            }
          });
        </script>
      </body>
      </html>
    )rawliteral");
  });

  // Handle control endpoints
  server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state")) {
      isOn = request->getParam("state")->value() == "true";
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      brightness = request->getParam("value")->value().toInt();
      FastLED.setBrightness(brightness);
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      animationMode = request->getParam("value")->value().toInt();
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/direction", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      direction = request->getParam("value")->value() == "true";
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      animationSpeed = request->getParam("value")->value().toInt();
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}
