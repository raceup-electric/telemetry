#ifndef DBLINK_H_
#define DELINK_H_

#include <ESP32_Supabase.h>

struct log_struct {
    float ping;
    long timestamp;
    uint8_t amk_status_fl;
    uint8_t throttle;
};

class Database {
    private:
        String supabase_link = "https://wvjiwtbxvehgbdskepdn.supabase.co";
        String supabase_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo";
        Supabase db;

        uint8_t log_name;
        void get_log();
    public:
        String device;

        Database(String device);
        Database(String device, String email, String pass);

        void init(String email, String pass);

        int push_struct(log_struct logs);
};

#endif
