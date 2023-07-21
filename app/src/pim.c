#include <stdint.h>
#include <pim.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

static const uint16_t pim_addr = 0x0A;
static const uint8_t pim_led_reg = 0x00;
static const uint8_t pim_mov_reg = 0x04;
static const uint8_t pim_int_reg = 0xF9;

#define PIM_BTN_MSK 	0x80
#define PIM_INT_EN_MSK 	0x02

LOG_MODULE_REGISTER(PIM);

static const struct gpio_dt_spec pim_int = GPIO_DT_SPEC_GET(DT_NODELABEL(pim447_int), gpios);
static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));

uint8_t config_pim() {
	gpio_pin_configure_dt(&pim_int, GPIO_INPUT);

	if (i2c == NULL || !device_is_ready(i2c)) {
		LOG_ERR("i2c device setup failed!");
		return 0;
	}
	//enable interrupt pin
	if (i2c_reg_update_byte(i2c, pim_addr, pim_int_reg, PIM_INT_EN_MSK, PIM_INT_EN_MSK)) {
		LOG_ERR("i2c failed to enable interrupt pin");
	}
	return 1;
}

static void (*move_callback)(uint8_t l, uint8_t r, uint8_t u, uint8_t d, uint8_t s) = NULL;
void pim_set_callback(void (*cb)(uint8_t l, uint8_t r, uint8_t u, uint8_t d, uint8_t s)) {
	move_callback = cb;
}

static uint8_t color[4] = {0U, 0U, 0U, 0U};
void pim_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = w;
	if (i2c_burst_write(i2c, pim_addr, pim_led_reg, color, 4)) {
		LOG_ERR("i2c write color failed!");
	}
}

uint8_t move_buf[5] = {0U, 0U, 0U, 0U, 0U};
void poll_pim() {
	if (!gpio_pin_get_dt(&pim_int)) {
		if (i2c_burst_read(i2c, pim_addr, pim_mov_reg, move_buf, 5)) {
			LOG_ERR("i2c read move reg failed!");
			return;
		}
		if (move_callback != NULL) {
			move_callback(move_buf[0], move_buf[1], move_buf[2], move_buf[3], move_buf[4] & PIM_BTN_MSK);
		}
	}
}
