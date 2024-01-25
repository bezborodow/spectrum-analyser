#include <Adafruit_NeoPixel.h>
#define LED_PIN 5
#define LED_COUNT 256
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(1);

  Serial.begin(115200);
  Serial.write("Listening.\n");
}

void loop() {
  int c = 0;


  if (Serial.available() > 0) {
    // read the incoming byte:
    //c = Serial.read();
    String message = Serial.readString();
    Serial.print(message);

    if (message == "on\n") {
      strip.show();
    }
    if (c == 'o') {
      strip.setPixelColor(0, strip.Color(0, 0, 0));
      strip.setPixelColor(7, strip.Color(0, 255, 0));
      strip.show();
    }
  }

  strip.setPixelColor(0, strip.Color(255, 255, 255));
  strip.setPixelColor(1, strip.Color(0, 255, 255));
  strip.setPixelColor(2, strip.Color(255, 0, 0));
  strip.setPixelColor(3, strip.Color(255, 255, 0));
  strip.setPixelColor(4, strip.Color(0, 255, 0));
  strip.setPixelColor(5, strip.Color(0, 0, 255));
  strip.setPixelColor(6, strip.Color(255, 0, 255));
}

void idle() {
  int n = strip.numPixels();
  // fph = FirstPixelHue
  for (long fph = 0; fph < 5 * 65536; fph += 256) {
    for (int i = 0; i < n; i++) {
      int pixelHue = fph + (i * 65536L / n);
      strip.setPixelColor(i,
                          strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(10);
  }
}