idf_component_register(SRCS "databasec.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_client zlib structures globals driver
                    EMBED_TXTFILES ssl_cert.pem)
