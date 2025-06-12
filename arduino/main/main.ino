
// F1 415nm
// F2 445nm
// F3 480nm
// F4 515nm
// F5 555nm
// F6 590nm
// F7 630nm
// F8 680nm

#include <stdio.h>
#include <Adafruit_SleepyDog.h>

#include "Adafruit_LTR390.h"
#include <Adafruit_AS7341.h>

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>  // Not actually used but needed to compile
#endif

#define MEASURE_INTERVAL 30000

// LT390 - UV
// AS7341 - 10 channel

Adafruit_LTR390 ltr = Adafruit_LTR390();
Adafruit_AS7341 as7341;

RH_ASK driver;

// State of the gain and resoultion of the LTR390
int ltrUVGain = 1;
int ltrALGain = 1;
int ltrResoultion = 1;

// AS7341 Gain
as7341_gain_t asGain = AS7341_GAIN_0_5X;

bool ltrOK = false;
bool asOK = false;
// Map the AGain from the enum to human-friendly value
int mapASGain() {
  switch (as7341.getGain()) {
    case AS7341_GAIN_0_5X:
      return 0;
    case AS7341_GAIN_1X:
      return 1;
    case AS7341_GAIN_2X:
      return 2;
    case AS7341_GAIN_4X:
      return 4;
    case AS7341_GAIN_8X:
      return 8;
    case AS7341_GAIN_16X:
      return 16;
    case AS7341_GAIN_32X:
      return 32;
    case AS7341_GAIN_64X:
      return 64;
    case AS7341_GAIN_128X:
      return 128;
    case AS7341_GAIN_256X:
      return 256;
    case AS7341_GAIN_512X:
      return 512;
  }
}

int mapLTRGain() {
  switch (ltr.getGain()) {
    case LTR390_GAIN_1:
      return 1;
    case LTR390_GAIN_3:
      return 3;
    case LTR390_GAIN_6:
      return 6;
    case LTR390_GAIN_9:
      return 9;
    case LTR390_GAIN_18:
      return 18;
  }
}

int mapLTRResoultion() {
  switch (ltr.getResolution()) {
    case LTR390_RESOLUTION_13BIT:
      ltrResoultion = 13;
      break;
    case LTR390_RESOLUTION_16BIT:
      ltrResoultion = 16;
      break;
    case LTR390_RESOLUTION_17BIT:
      ltrResoultion = 17;
      break;
    case LTR390_RESOLUTION_18BIT:
      ltrResoultion = 18;
      break;
    case LTR390_RESOLUTION_19BIT:
      ltrResoultion = 19;
      break;
    case LTR390_RESOLUTION_20BIT:
      ltrResoultion = 20;
      break;
  }
  return ltrResoultion;
}


/**
   Setup.
*/
void setup() {
  Serial.begin(115200);

  Serial.println("version 2");
  if (!driver.init()) {
    Serial.println("COMS Driver init failed");
  }

  uint8_t retry = 4;

  while (!ltr.begin() && retry > 0) {
    Serial.print("Couldn't find LTR sensor");
    Serial.println(retry);
    retry--;
    delay(200);
  }
  ltrOK = !(retry == 0);

  Serial.println(ltrOK);
  if (ltrOK) {
    Serial.print("Found LTR sensor ");
    ltr.setGain(LTR390_GAIN_3);
    Serial.print("Gain : ");
    Serial.println(mapLTRGain());
    ltrUVGain = LTR390_GAIN_3;
    ltrALGain = LTR390_GAIN_3;

    ltr.setResolution(LTR390_RESOLUTION_16BIT);
    Serial.print("Resolution : ");
    Serial.println(mapLTRResoultion());

    ltr.setThresholds(100, 1000);
    ltr.configInterrupt(true, LTR390_MODE_UVS);
  } else {
    Serial.println("Not found LTR");
  }

  retry = 4;
  /* 10channel light sensor set */
  while (!as7341.begin() && retry > 0) {
    Serial.print("Could not find AS7341");
    Serial.println(retry);
    retry--;
    delay(200);
  }

  asOK = !(retry == 0);
  if (asOK) {
    Serial.println("Found AS");

    as7341.setATIME(100);
    as7341.setASTEP(999);
    as7341.setGain(asGain);
  } else {
    Serial.println("Not found as");
  }
}



/** Loop */
void loop() {
  char buffer[100];

  /* raw uv count*/
  uint32_t uv = 0;

  /* raw visible count */
  uint32_t als = 0;

  /* Light channels, corresponds to specific wavelength */
  uint16_t f1=0, f2=0, f3=0, f4=0, f5=0, f6=0, f7=0, f8 = 0;
  uint16_t clr=0, nir =0 ;

  /* used gain values for LTR */
  int usedUVGainh = 0;
  int usedALGainh = 0;

  Serial.println("---");
  // enable the sensor
  ltr.enable(true);
  while (!ltr.enabled()) {
    delay(10);
  }

  // Get the UV reading
  ltr.setMode(LTR390_MODE_UVS);
  ltr.setGain(ltrUVGain);
  while (!ltr.newDataAvailable()) {
    delay(10);
  }
  uv = ltr.readUVS();
  usedUVGainh = mapLTRGain();

  ltr.setMode(LTR390_MODE_ALS);
  ltr.setGain(ltrALGain);
  while (!ltr.newDataAvailable()) {
    delay(10);
  }
  als = ltr.readALS();
  usedALGainh = mapLTRGain();

  // Disable for power saving
  ltr.enable(false);

  if (ltrUVGain == LTR390_GAIN_3 && uv > 50000) {
    ltrUVGain = LTR390_GAIN_1;
  } else if (ltrUVGain == LTR390_GAIN_1 && uv < 10000) {
    ltrUVGain = LTR390_GAIN_3;
  }

  if (ltrALGain == LTR390_GAIN_3 && als > 50000) {
    ltrALGain = LTR390_GAIN_1;
  } else if (ltrALGain == LTR390_GAIN_1 && als < 10000) {
    ltrALGain = LTR390_GAIN_3;
  }

  /* ------------------------------------- */
  if (asOK && as7341.readAllChannels()) {
    f1 = as7341.getChannel(AS7341_CHANNEL_415nm_F1);
    f2 = as7341.getChannel(AS7341_CHANNEL_445nm_F2);
    f3 = as7341.getChannel(AS7341_CHANNEL_480nm_F3);
    f4 = as7341.getChannel(AS7341_CHANNEL_515nm_F4);
    f5 = as7341.getChannel(AS7341_CHANNEL_555nm_F5);
    f6 = as7341.getChannel(AS7341_CHANNEL_590nm_F6);
    f7 = as7341.getChannel(AS7341_CHANNEL_630nm_F7);
    f8 = as7341.getChannel(AS7341_CHANNEL_680nm_F8);
    clr = as7341.getChannel(AS7341_CHANNEL_CLEAR);
    nir = as7341.getChannel(AS7341_CHANNEL_NIR);
  }

  // Format the string for sending the raw data
  sprintf(buffer, "1:%u 2:%u 3:%u 4:%u 5:%u 6:%u 7:%u 8:%u", f1, f2, f3, f4, f5, f6, f7, f8);

  Serial.println(buffer);
  driver.setHeaderId(1);
  driver.send((uint8_t *)buffer, strlen(buffer));
  driver.waitPacketSent();

  // record the gain values for the spectral sensor
  sprintf(buffer, "spg:%d uvg:%d alg:%d ltrr:%d", mapASGain(), usedUVGainh, usedALGainh, ltrResoultion);
  Serial.println(buffer);
  driver.setHeaderId(5);
  driver.send((uint8_t *)buffer, strlen(buffer));
  driver.waitPacketSent();

  sprintf(buffer, "nir:%u clr:%u", nir, clr);
  Serial.println(buffer);
  driver.setHeaderId(2);
  driver.send((uint8_t *)buffer, strlen(buffer));
  driver.waitPacketSent();

  sprintf(buffer, "uv:%lu als:%lu ##%d:%d", uv, als, asOK,ltrOK);
  Serial.println(buffer);
  driver.setHeaderId(3);
  driver.send((uint8_t *)buffer, strlen(buffer));
  driver.waitPacketSent();

  (void)Watchdog.sleep(MEASURE_INTERVAL);
}
