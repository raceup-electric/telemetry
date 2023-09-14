#include <ESPping.h>
#include <ping.h>

#include <ESP32_Supabase.h>
#include "WiFi.h"

#include "dblink.h"

Database db("ecu");

log_struct d;

void setup() {
    Serial.begin(115200);

    WiFi.begin("BomboWiFi", "@Melbourne1960");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());

    db.init("ecu@raceup.it", "Centralina");
}

void loop() {
    bool ret = Ping.ping("www.google.com", 5);

    d.ping = Ping.averageTime();
    d.timestamp = millis();
    d.amk_status_fl = random(0, 255);
    d.throttle = random(0, 100);

    Serial.println(db.push_struct(d));

    delay(1500);
}
