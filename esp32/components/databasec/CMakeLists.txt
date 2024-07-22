idf_component_register(SRCS "databasec.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_client zlib structures globals driver mbedtls esp_netif)
