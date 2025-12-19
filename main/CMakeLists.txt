# Collect all source files from subdirectories
file(GLOB AUDIO_SOURCES "audio/*.c")
# TODO: Uncomment when modules are created
# file(GLOB ADC_SOURCES "adc/*.c")
# file(GLOB DETECTION_SOURCES "detection/*.c")
# file(GLOB SD_CARD_SOURCES "sd_card/*.c")

# Register main component
idf_component_register(
    SRCS
        "main.c"
        ${AUDIO_SOURCES}
        # ${ADC_SOURCES}
        # ${DETECTION_SOURCES}
        # ${SD_CARD_SOURCES}
    INCLUDE_DIRS
        "."
        "audio"
        # "adc"
        # "detection"
        # "sd_card"
    PRIV_REQUIRES
        esp_driver_i2s
        esp_driver_gpio
        driver
)
