idf_component_register(SRCS "supabasec.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_client logs
                    EMBED_TXTFILES ssl_cert.pem)
