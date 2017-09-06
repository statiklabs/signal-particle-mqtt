/*
Copyright (c) 2017 Statik Labs Inc.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include "SignalMQTT.h"


Signal::Signal(char* token, void (*callback)(char*,uint8_t*,unsigned int)){
    this->callback = callback;
    _token = token;
    _clientName = System.deviceID();
}



bool Signal::isConnected(){
    return _client->isConnected();
}


bool Signal::connect(){
    bool connected = false; 
    if(!_client->isConnected()){
        connected = reconnect();
    }
    return connected;
}

void Signal::initialize(){
    this->_client = new MQTT("mqtt.mysignal.io", 1883, this->callback);
    _clientName = System.deviceID();
    _client->connect(_clientName, _token, NULL);
    bool connected = _client->isConnected();
    if(connected){
        Serial.println("connected to broker");
    }
}


bool Signal::loop(){
    return _client->loop();
}

bool Signal::reconnect(){
    if(!_client->isConnected()){
        Serial.println("attemping to connect");
    }
    while(!_client->isConnected()){
        _client->connect(_clientName, _token, NULL);
        Serial.print(".");
        delay(1000);
    }
    return true;
}


bool Signal::publishCoordinates(String coordinates){
    return _client->publish("device/coordinates", coordinates);
}