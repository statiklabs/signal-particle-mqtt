## MySignal.io MQTT library for Particle Devices

This library allows you to easily connect your Particle device to the MySignal.io platform and publish your asset tracker data. For example on how to use, see this Hackster tutorial:

https://www.hackster.io/YanSarazin/particle-asset-tracker-with-mysignal-io-using-mqtt-1a853e

## Methods Available

### MySignal Constructor
```
MySignal(char* token, void (*callback)(char*,uint8_t*,unsigned int));
```
> Creates a MySignal instance, you must provide your Device TOKEN and the callback function. 

## connect
```
connect();
```
> Tries to connect to the broker
## initialize
```
initialize();
```
> Creates the proper credentials to connect to MySignal's broker
## loop
```
loop();
```
> Infinite loop for MQTT connection, insert it at the end of your routine
## publishCoordinates
```
publishCoordinates(String coordinates);
```
> Publish the GPS coorindates (latitude,longitude) of your device. Example?

```cpp
publishCoordinates("45.425114,-75.699642");
```

## publishData
```
publishData(String variable, String data);
```
> Publish any data. Provide a variable name for your data and and the data itself. Example:
```cpp
publishData("Temperature", "26.5");
```

## Example Sketch

To subscribe to a variable, you need to specify your device and variable lables as input arguments for the ubidotsSubscribe() function. The incoming value will be returned by the MQTT library in the payload variable, by default the subscribed function only gets the last value of the variable subscribed. Retain value feature is only available for business users.

```cpp
#include "SignalMQTT.h"
#include "AssetTracker.h"

// Once you created a device at mysignal.io, you'll begin
// given a device token. Insert the device token between the quotes
#ifndef TOKEN
#define TOKEN "your device token here"
#endif

void callback(char* topic, byte* payload, unsigned int length);

// Track last time coordinates were published
long lastPublish = 0;

// How many milliseconds between publishes 
int delayMillis = 10000;

AssetTracker t = AssetTracker();

// Define a client connection name 'signal'
MySignal signal(TOKEN, callback);

void callback(char* topic, uint8_t* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
}

void setup() {
    t.begin();
    
    t.gpsOn();

    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);
    
    // Initialize the connection to Signal
    signal.initialize();

}

// loop() runs continuously
void loop() {        

    // Reconnect if connection to Signal was lost
    if(!signal.isConnected()){
        signal.connect();
    }
    
    t.updateGPS();

    // Delay the loop
    if (millis()-lastPublish > delayMillis) {
        lastPublish = millis();

        if (t.gpsFix()) {
            if (signal.isConnected()) {
                // Publish coordinates
                signal.publishCoordinates(t.readLatLon());
            }
            Serial.println(t.readLatLon());
        }
    }

    // Maintain the connection to Signal if connected
    if (signal.isConnected()){
        signal.loop();
    }
}
```
