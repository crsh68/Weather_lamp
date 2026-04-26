#ifndef EFFECTS_PARTLY_CLOUDY_H
#define EFFECTS_PARTLY_CLOUDY_H

// ===== PARTLY CLOUDY EFFECT =====
// Kombinacija oblačnog neba (effects_clouds) + pulsirajuće sunce (kao u effects_sunny)
// Koristi se za: scattered clouds, broken clouds
//
// LEDs 20-23 = pulsirajuće sunce (prigušenije nego u pure sunny)
// LEDs 0-19  = valoviti oblaci (sive nijanse kao u effects_clouds)

static float pcSunPulse   = 0.0f;
static float pcPhase1     = 0.0f;
static float pcPhase2     = 0.0f;

const float   PC_SUN_SPEED   = 0.045f;     // nešto sporije pulsiranje od sunny
const float   PC_CLOUDS_SP1  = 0.0525f * 1.2f;
const float   PC_CLOUDS_SP2  = 0.0270f * 1.2f;
const uint8_t PC_CLOUDS_BASE = 56;         // ista baza kao u effects_clouds
const unsigned long PC_FRAME_MS = 30;

static unsigned long pcLastFrame = 0;

void partlyCloudy() {
  unsigned long now = millis();
  if (now - pcLastFrame < PC_FRAME_MS) return;
  pcLastFrame = now;

  // ===== SUNCE (LEDs 20-23) =====
  // Pulsirajuće žuto sunce, ali malo prigušenije nego u sunny()
  // (max ~80% intenziteta jer je djelomično pokriveno oblacima)
  pcSunPulse += PC_SUN_SPEED;
  float sunIntensity = sin(pcSunPulse) * 0.25f + 0.65f;  // 0.40 - 0.90 range
  uint8_t sunB = (uint8_t)(220 * sunIntensity);           // max 220 (ne 255)
  CRGB sunColor = CRGB(sunB, (uint8_t)(sunB * 0.78f), 0);

  leds[23] = sunColor;
  leds[22] = sunColor;
  leds[21] = sunColor;
  leds[20] = sunColor;

  // ===== OBLACI (LEDs 0-19) =====
  // Isti valoviti algoritam kao u clouds(), ali samo prvih 20 LEDs
  for (int i = 0; i < 20; i++) {
    float v =
      sin(pcPhase1 + i * 0.35f) * 0.6f +
      sin(pcPhase2 + i * 0.18f) * 0.4f;
    uint8_t b = (uint8_t)constrain(PC_CLOUDS_BASE + v * PC_CLOUDS_BASE, 0, 255);
    leds[i] = CRGB(b, b, b);
  }

  pcPhase1 += PC_CLOUDS_SP1;
  pcPhase2 += PC_CLOUDS_SP2;
}

#endif  // EFFECTS_PARTLY_CLOUDY_H
