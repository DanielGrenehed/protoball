cmake_minimum_required(VERSION 3.20.0)
set(BOARD nrf52840dongle_nrf52840)
set(DTC_OVERLAY_FILE protoball.overlay)

find_package(Zephyr)
project(protoball)

include_directories(src)
target_sources(app PRIVATE src/main.c)

set_property(GLOBAL APPEND PROPERTY HEX_FILES_TO_MERGE
	${app_bootloader_hex}
	${PROJECT_BINARY_DIR}/${KERNEL_HEX_NAME}
	${app_provision_hex})
