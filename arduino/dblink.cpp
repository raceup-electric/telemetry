#include "dblink.h"

Database::Database(String device) {
    this->device = device;
};

Database::Database(String device, String email, String pass){
    this->device = device;

    db.begin(supabase_link, supabase_key);
    db.login_email(email, pass);

    get_log();
}

void Database::init(String email, String pass){
    this->device = device;

    db.begin(supabase_link, supabase_key);
    db.login_email(email, pass);

    get_log();
}

void Database::get_log(){
    String read = db.from(device).select("log_name").order("log_name", "desc", true).limit(1).doSelect();
    db.urlQuery_reset();

    this->log_name = (read == "[]") ? 1 : (read.substring(19, 20).toInt() + 1);
}

int Database::push_struct(log_struct logs) {
    String json = "{";

    json += "\"log_name\": \"STEST" + String(log_name) + "\",";
    json += "\"timestamp\": \"" + String(logs.timestamp) + "\",";
    json += "\"amk_status_fl\": \"" + String(logs.amk_status_fl) + "\"";

    json += "}";

    Serial.println(json);

    return db.insert("ecu", json, false);
}