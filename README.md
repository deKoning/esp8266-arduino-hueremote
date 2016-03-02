# esp8266-arduino-hueremote
Simple Hue remote based on the Sparkfun ESP8266 Thing created in Arduino IDE

Follow the [Sparkfun Tutorial](https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/all) to get started with your Thing.

To use this remote you'll need to do a few things:

Install the restclient library files [from github](https://github.com/modulusx/esp8266-arduino-restclient) to the arduinosketches/libraries/restclient directory. You should end up with libraries/restclient/restclient.h and .cpp

I recommend you set your bridge to have a static IP, then create a new key by following these steps:
1. Browse to http://your.hue.bridge.ip/debug/clip.html
2. Enter /api for the URL
3. Enter something like {"devicetype":"Thing#hueremote"}
4. Click POST
5. You should receive an API key back in the username field.
6. If this doesn't work, use the real [instructions](http://www.developers.meethue.com/documentation/getting-started).

To Use:
1. Update the Hue Bridge ip in hueremote.ino
2. Update your Wireless SSID and Password
3. Update the API key in the remote with your key.
4. Update or change the buttons to suit your needs.
..* You can GET /api/yourkey/ to view the names/numbers of each light/group.  You can also manage all of the lights and groups using the API, refer to the Hue developers page for more info.

By design, buttons 1 to 4 are wired to pins 0,4,13,12 respectively, and exploit internal pullup resistors, so you just need to GROUND the pin with your switch to trigger it.
