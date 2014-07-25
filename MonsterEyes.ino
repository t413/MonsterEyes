/**
 * MonsterEyes.ino
 * by timo@t413.com | http://github.com/t413
 *
 * This work is licensed under a Creative Commons
 *  Attribution-NonCommercial 4.0 International License.
 *  http://creativecommons.org/licenses/by-nc/4.0/
 */

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 1, NEO_GRB + NEO_KHZ800);  // # of LEDS, port, type
#define TOP_INDEX 2 //The index of the top led (it's round, which one is 'up'?)

void setup() {
  randomSeed(analogRead(2));
  pixels.begin();
  pixels.setBrightness(80);
}


void loop() {
  uint8_t animation_speed = 50;
  int8_t hrzn_start = 0, hrzn_start_goal = 0;
  int8_t blink_value = 0;
  uint32_t next_hrzn_change = 0;
  uint32_t next_blink = 0;
  uint32_t next_ani_frame = 0;

  while(1) {
    uint32_t current = millis();
    uint8_t baseColor = current >> 9;
    if (current >= next_hrzn_change) {
      next_hrzn_change = current + random(50, 4000);
      hrzn_start_goal = random(0, 8 << 2);
    }
    if (current >= next_blink) {
      next_blink = current + random(50, 4000);
      animation_speed = random(15, 60);
      blink_value = 7; //1/4 of ring size * 2 (animates down and up)
    }
    if (current >= next_ani_frame) {
      next_ani_frame = current + animation_speed;
      hrzn_start += constrain(hrzn_start_goal - hrzn_start, -1, 1);
      blink_value = max(-1, blink_value - 1);
    }


    for (uint8_t i=0; i < 8; i++) {
      uint8_t hc = baseColor + i * 2;
      int8_t b = abs(blink_value - 4);
      int8_t on = (3 - abs((int8_t)i - 4)) < b;
      pixels.setPixelColor(led_position(i + (hrzn_start >> 2)), on? wheel(hc) : 0);
      pixels.setPixelColor(led_position(i + 8 + (hrzn_start >> 2)), on? wheel(hc + 70) : 0);
    }
    pixels.show();
    baseColor += 1;
  }
}

uint8_t led_position(uint8_t i) {
  return (i + TOP_INDEX) % 16;
}


uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint32_t wheel(int wheelPos) {
  uint8_t pos = wheelPos & 0xFF;
  if(pos < 85) {
    return color(pos * 3, 255 - pos * 3, 0);
  }
  else if(pos < 170) {
    pos -= 85;
    return color(255 - pos * 3, 0, pos * 3);
  }
  else {
    pos -= 170;
    return color(0, pos * 3, 255 - pos * 3);
  }
}

