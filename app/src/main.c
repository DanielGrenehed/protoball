#include <stdint.h>
#include <leds.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define PIM_ADDR 	0x0A
#define PIM_LED_REG 	0x00
#define PIM_TRK_REG	0x04
#define PIM_INT_REG	0xF9
#define PIM_INT_EN_MSK	0x02



LOG_MODULE_REGISTER(main);

static const struct gpio_dt_spec pim_int = GPIO_DT_SPEC_GET(DT_NODELABEL(pim447_int), gpios);
static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));

void i2c_read_reg(uint16_t addr, uint8_t reg, uint8_t *buf, uint8_t buf_size) {
	i2c_write(i2c, &reg, 1, addr);
	if (i2c_read(i2c, buf, buf_size, addr)) {
		LOG_ERR("i2c read failed! addr: %x, reg: %x, buf_size: %i", addr, reg, buf_size);
	}
}

void i2c_write_reg(uint16_t addr, uint8_t reg, uint8_t *buf, uint8_t buf_size) {
	i2c_write(i2c, &reg, 1, addr);
	if (i2c_write(i2c, buf, buf_size, addr)) {
		LOG_ERR("i2c write failed! addr: %x, reg: %x, buf_size: %i", addr, reg, buf_size);
	}
}

static uint8_t color[3] = {0U, 0U, 0U};
void set_color(uint8_t r, uint8_t g, uint8_t b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
	set_led_color(r,g,b);
	i2c_write_reg(PIM_ADDR, PIM_LED_REG, color, 3);
} 
void main() {
	config_leds();
	gpio_pin_configure_dt(&pim_int, GPIO_INPUT);
	
	if (i2c == NULL || !device_is_ready(i2c)) {
		LOG_ERR("i2c device failed!");
		return;
	}
	
	LOG_INF("Success!");

	set_color(255, 255, 255);
}
