// Anpassung des Codes von FedeZara (Quelle - https://github.com/FedeZara/WaterHardness) 
// durch Felix Wengert(WEF) im Rahmen der Maturarbeit, August 2023
//
//
#include <Arduino.h>
// WEF vgl. unten - Verwendung neuer Library 
// WEF zur Steuerung der Touchscreen Funktionalität
// WEF (auskommentiert): #include <URTouch.h>
#include "TouchScreen.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_TCS34725.h"
#include "EEPROM.h"

#include "Point.h"
#include "CalibrationData.h"
#include "LinearRegression.h"
#include "Label.h"
#include "Button.h"
#include "Page.h"
#include "NavigationController.h"

#include "MainPage.h"
#include "MeineSeite.h"
#include "StartCalibrationPage.h"
#include "EndCalibrationPage.h"
#include "CalibrationPage.h"
#include "MeasurementPage.h"

using namespace std;

// For the Adafruit shield, these are the default.
// WEF (auskommentiert): #define TFT_CLK 13
// WEF (auskommentiert): #define TFT_MISO 12
// WEF (auskommentiert): #define TFT_MOSI 11
// WEF (auskommentiert): #define TFT_DC 9
// WEF (auskommentiert): #define TFT_CS 10
// WEF (auskommentiert): #define TFT_RST 8
// If using the breakout, change pins as desired
// WEF Anpassen auf Belegung des Adafruit ILI9341 Examples "Graphictest" für Breakout Touchscreen
// WEF "graphictest_ILI9341" (vgl. Dokumentation unter:
// WEF "https://learn.adafruit.com/adafruit-2-8-and-3-2-color-tft-touchscreen-breakout-v2/spi-wiring-and-test")
//
#define TFT_CLK 52
#define TFT_MISO 50
#define TFT_MOSI 51
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
// WEF - Ende der Anpassungen

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// WEF Bisherige Pin Belegung für die Benutzung der Touchscreen (ts) Utility auf 
// WEF Basis der "URTouch" Library (neu wird die Touchscreen Library aus der 
// WEF "Adafruit ILI9341" Library verwendet).
// WEF (auskommentiert): #define t_SCK 6
// WEF (auskommentiert): #define t_CS 5
// WEF (auskommentiert): #define t_MOSI 4
// WEF (auskommentiert): #define t_MISO 3
// WEF (auskommentiert): #define t_IRQ 2
// WEF (auskommentiert): ts = URTouch(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

// WEF Neue Pin Belegung für die Benutzung der Touchscreen (ts) Utility auf 
// WEF Basis der "TouchScreen" Library, welche in der "Adafruit ILI9341" 
// WEF Library enthalten ist.
// WEF Code und Belegung gemäss Adafruit ILI9341 Example "breakouttouchpaint" 
// WEF (vgl. Dokumentation unter: 
// WEF "https://learn.adafruit.com/adafruit-2-8-and-3-2-color-tft-touchscreen-breakout-v2/resistive-touchscreen").
// WEF Achtung, diese Anpassung bedarf gleichteitig Anpassungen an "Page.h"!!
// 
// WEF Beginn des neu eingefügten Codes
// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 9   // can be any digital pin
#define XP 8   // can be any digital pin
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// WES Ende des neu eingefügten Codes

/* Connect SCL    to analog 5 (Arduino Uno) or digital 21 (Arduino Mega)
   Connect SDA    to analog 4 (Arduino Uno) or digital 20 (Arduino Mega)
   Connect VCC    to 5V DC
   Connect GROUND to common ground */

//WEF (auskommentiert): Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
// WEF "TCS34725_INTEGRATIONTIME_700MS" auf "TCS34725_INTEGRATIONTIME_614MS", da mit 700 Fehlermeldung!
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

NavigationController navigationController;

CalibrationData calData;
LinearRegression model;

void DefinePages(){
  navigationController.SetNumPages(6);

  navigationController.AddPage(new MainPage(), 0);
  navigationController.AddPage(new StartCalibrationPage(), 1);
  navigationController.AddPage(new CalibrationPage(), 2);
  navigationController.AddPage(new EndCalibrationPage(), 3);
  navigationController.AddPage(new MeasurementPage(), 4);
  navigationController.AddPage(new MeineSeite(), 5);
}

void setup()
{
  Serial.begin(9600);
  // WEF
  pinMode(13, OUTPUT); // LED an Pin 13
  Serial.print("Initial: LED ausschalten\n");
  digitalWrite(13, LOW);
  // WEF
  tft.begin();
  // WEF vgl. "https://learn.adafruit.com/adafruit-gfx-graphics-library/rotating-the-display"
  // WEF 0 sets the display to a portrait (tall) mode (GND/SPI befindet sich links oben)
  // WEF 1 is landscape (wide) mode (Drehung bzl.initiale Orientierung um 90 Grad)
  // WEF 2 is portrait mode (Drehung bzl.initiale Orientierung um 180 Grad))
  // WEF 3 is landscape mode (Drehung bzl.initiale Orientierung um 270 Grad)
  // WEF Achtung, dies andert nicht die Orientierung de sKoordinatensystems 
  // WEF der Touch Fnuktionality!! 
  //
  tft.setRotation(3);

  // WEF Nachfolgende ursprünglich Initialisierung für URTouch wird mit TouchScreen 
  // WEF nicht mehr benötigt
  // WEF (auskommentiert): ts.InitTouch();
  // WEF (auskommentiert): ts.setPrecision(PREC_EXTREME);

  tcs.begin();

  // Clear EEPROM 
  // Suggested if using the device for the first time
  /* for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.write(i, 0);
  }*/

  DefinePages(); 

  navigationController.NavigateTo(5);
}

void loop(void)
{
  navigationController.OnLoop();
}
