#include <stdint.h>
#include <leds.h>
#include <pim.h>
#include <hid.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define MS_REPORT 8

LOG_MODULE_REGISTER(main);

uint8_t scaler = 5;

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	set_led_color(r,g,b);
	pim_set_color(r,g,b,w);
} 

void on_tb_move(uint8_t left, uint8_t right, uint8_t up, uint8_t down, uint8_t button) {
	//LOG_INF("left: %i, right: %i, up: %i, down: %i, button: %i", left, right, up, down, button);
	int8_t x = (right - left) * scaler;
	int8_t y = (down - up) * scaler;
	uint8_t b = button ? LEFT_MOUSE_BTN : 0;
	hid_add_to_report(b,x,y);
}

void main() {
	config_leds();
	if (!config_pim()) return;
	if (!config_hid()) return;
	pim_set_callback(on_tb_move);
	LOG_INF("Success!");

	set_color(255, 255, 255, 0);
	uint32_t last_report = 0;
	while (1) {
		k_msleep(1);
		poll_pim();
		if (last_report + MS_REPORT < k_uptime_get()) {
			hid_report();
			last_report = k_uptime_get();
		}
	}
}
