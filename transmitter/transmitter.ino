#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial

#define AP_SSID        "Yoeil's iPhone"
#define AP_PASSWORD    "password1234"

#define DEVICE_A_ID      "c93b6116-913f-49a1-a139-c892bf99f983"
#define DEVICE_A_KEY     "Lv9MYn5znNJ9Q77g5pQCenPts94kR9LQj8LPcSBwEbfvypATknLgR3tJjkCZ6xyg"
#define DEVICE_B_ID      "ac81fd78-590a-47a9-bcbf-15ad677a59b3"
#define DEVICE_B_KEY     "jyuW7mM9FdQNrMrLgrNwsJ7wKzsMQBAtFJxSVw5AN3R8FEe3WSV2EvJ8NQqyy8np"

WiFiMulti wifiMulti;

void setup() {

    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    wifiMulti.addAP(AP_SSID, AP_PASSWORD);

}

void loop() {
    // wait for WiFi connection
    postDataToServer(DEVICE_A_ID, DEVICE_A_KEY, 7);
    postDataToServer(DEVICE_B_ID, DEVICE_B_KEY, 193);
     
    delay(10000);
}

void postDataToServer(String id, String key, int value) {
   // wait for WiFi connection
   if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin("http://172.105.84.140/api/add_entry"); //HTTP
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> doc;
        // Add values in the document
        doc["device_id"] = id;
        doc["key"] = key;
        doc["val"] = value;
        
        String requestBody;
        serializeJson(doc, requestBody);

        USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
        int httpCode = http.POST(requestBody);

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}
