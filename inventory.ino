#define BLYNK_TEMPLATE_ID "TMPL3B4U7_RKx"
#define BLYNK_TEMPLATE_NAME "Weight"
#define BLYNK_AUTH_TOKEN "bp3lULHBxjH5xEsJpfZ5U-81ExycWLcn"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HX711.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DOUT 23
#define CLK 19
HX711 scale;  

#define BLYNK_TEMPLATE_ID "TMPL3B4U7_RKx"
#define BLYNK_TEMPLATE_NAME "Weight"
#define BLYNK_AUTH_TOKEN "bp3lULHBxjH5xEsJpfZ5U-81ExycWLcn"

char ssid[] = "vivo 1920";
char pass[] = "Tilak1234";

float weight;
float calibration_factor = -200.0345;  

void setup() {
  Serial.begin(115200);

  // OLED display
  if (!display.begin(i2c_Address, true)) {
    Serial.println(F("SH1106 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  // HX711
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.begin(DOUT, CLK);  
  scale.set_scale();
  scale.tare(); // Reset the scale to 0
  long zero_factor = scale.read_average(); 
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  measureweight();
  displayWeightOnOLED();
}

void measureweight() {
  scale.set_scale(calibration_factor); 
  Serial.print("Reading: ");
  weight = scale.get_units(5);
  if (weight < 0) {
    weight = 0.00;
  }
  Serial.print("gram:");
  Serial.print(weight);
  Serial.print(" g");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  Blynk.virtualWrite(V0, weight);
  Blynk.virtualWrite(V1, weight);
  
  delay(100);
}

void displayWeightOnOLED() {
  
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("TOTAL WEIGHT ");
  display.setCursor(30, 45);
  display.setTextSize(2);
  display.print(weight);
  display.print(" gm");
  display.display();

  delay(1000); 
}
