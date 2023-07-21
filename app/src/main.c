#include <stdint.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

static const struct gpio_dt_spec pim_int = GPIO_DT_SPEC_GET(DT_NODELABEL(pim447_int), gpios);
static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));

void set_color(uint8_t r, uint8_t g, uint8_t b) {
	
} 

void main() {
	gpio_pin_configure_dt(&pim_int, GPIO_INPUT);
	
	if (i2c == NULL || !device_is_ready(i2c)) {
		LOG_ERR("i2c device failed!");
		return;
	}
	
	LOG_INF("Success!");
}
