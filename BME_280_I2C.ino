#include <BME280I2C.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
int scanTime = 1;
BLEScan* pBLEScan;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
    }
};

#define SERIAL_BAUD 115200

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(SERIAL_BAUD);

  while(!Serial) {} // Wait

  Wire.begin();

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

}

//////////////////////////////////////////////////////////////////
void loop()
{
   printBME280Data(&Serial);
   Serial.println();
   delay(100);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);


  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  pBLEScan->clearResults();
   

   /*client->print("Temp: ");*/
   client->print(temp);
   client->print(',');
   /*client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   /*client->print("\t\tHumidity: ");*/
   client->print(hum);
   client->print(',');
   /*client->print("% RH");*/
   /*client->print("\t\tPressure: ");*/
   client->print(pres);
   client->print(',');
   client->print(foundDevices.getCount());

   delay(100);

}
