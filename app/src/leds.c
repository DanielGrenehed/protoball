#include <stdint.h>
#include <leds.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#define NLEDS 3

static const struct pwm_dt_spec rgb_led[NLEDS] = {
	PWM_DT_SPEC_GET(DT_NODELABEL(red_pwm_led)),
	PWM_DT_SPEC_GET(DT_NODELABEL(green_pwm_led)),
	PWM_DT_SPEC_GET(DT_NODELABEL(blue_pwm_led))
};

LOG_MODULE_REGISTER(leds);

static uint32_t max_period;
static uint32_t period_mult;
static uint32_t rgb_ps[NLEDS];

void config_leds() {
	for (int i = 0; i<NLEDS; i++) if (!device_is_ready(rgb_led[i].dev)) LOG_ERR("PWM rgb_led[%i] not ready", i);
	max_period = rgb_led[0].period;
	period_mult = max_period/255;
	set_led_color(0,0,0);
}

void set_led_color(uint8_t r, uint8_t g, uint8_t b) {
	rgb_ps[0] = r * period_mult;
	rgb_ps[1] = g * period_mult;
	rgb_ps[2] = b * period_mult;

	for (int i = 0; i < NLEDS; i++) {
		pwm_set_pulse_dt(&rgb_led[i], rgb_ps[i]);
	}
}
