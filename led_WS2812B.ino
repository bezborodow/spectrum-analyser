#include <Adafruit_NeoPixel.h>
#define LED_PIN 5
#define LED_COUNT 256
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.setBrightness(1);
  strip.setPixelColor(1, strip.Color(255, 0, 0));
  strip.show();
  delay(100);
  
  strip.setPixelColor(1, strip.Color(0, 255, 0));
  strip.show();
  
  Serial.begin(9600);
  Serial.write("Listening.\n");
}

int c = 0;
int params[32] {0};
int i = 0;
int ko = 0;
int cmd = 0;

void loop() {
  if (Serial.available()) {
    c = Serial.read();

    if (c == 'I') {
      strip.setPixelColor(0, strip.Color(255, 255, 255));
    }
    if (c == 'O') {
      strip.setPixelColor(0, strip.Color(0, 0, 0));
    }
    if (c == 'G') {
      cmd = 'G';
    }

    if (c >= 0x30 && c <= 0x38 && i < 32) {
      params[i++] = c - 0x30;
    }

    if (c == '\n') {
      if (cmd == 'G') {
        // Show graph.
        int k = 0;
        for (i = 0; i < 32; i++) {
          for (int j = 0; j < 8; j++, k++) {
            ko = i % 2 ? k + 7 - 2*j : k; // k-offset.
            if (j < params[31 - i]) {
              strip.setPixelColor(ko, strip.Color(255, 255, 255));
            } else {
              strip.setPixelColor(ko, strip.Color(0, 0, 0));
            }
          }
        }
      }
      
      i = 0;
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