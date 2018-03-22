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

#ifndef SignalMQTT_H
#define SignalMQTT_H

#include "MQTT.h"

class Signal {

 private:
    void (*callback)(char*,uint8_t*,unsigned int);
    MQTT *_client;
    String _clientName;
    String _token;
    bool _debug = false;

 public:
    Signal(char* token, void (*callback)(char*,uint8_t*,unsigned int));
    bool connect();
    void initialize();
    bool isConnected();
    bool loop();
    bool reconnect();
    bool publishCoordinates(String coordinates);
    bool publishData(String variable, String data);
};

#endif