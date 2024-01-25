#include <Adafruit_NeoPixel.h>
#define LED_PIN 5
#define LED_COUNT 256
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int c = 0;

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(1);

  Serial.begin(115200);
  Serial.write("Listening.\n");
}

void loop() {
  if (Serial.available()) {
    c = Serial.read();
    Serial.println(c, DEC);

    if (c == '1') {
      strip.setPixelColor(0, strip.Color(255, 255, 255));
      strip.show();
    }
    if (c == '0') {
      strip.setPixelColor(0, strip.Color(0, 0, 0));
      strip.show();
    }
  }
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
