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
