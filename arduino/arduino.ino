#include <ESP32_Supabase.h>
#include "WiFi.h"

#include "dblink.h"

Database db("ecu");

log_struct d;

void setup() {
    Serial.begin(115200);

    WiFi.begin(".", "8caratteri");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());

    db.init("ecu@raceup.it", "Centralina");
}

void loop() {
    d.timestamp = millis();
    d.amk_status_fl = random(0, 255);

    Serial.println(db.push_struct(d));

    delay(1500);
}