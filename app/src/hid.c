#include <stdint.h>
#include <hid.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(hid);

const struct device *hid_dev;

static const uint8_t hid_report_descriptor[] = HID_MOUSE_REPORT_DESC(2);

#define BTN_REP_POS 0
#define X_REP_POS 1
#define Y_REP_POS 2

static uint8_t rep_status[4];
static enum usb_dc_status_code usb_status;

static void usb_status_callback(enum usb_dc_status_code sc, const uint8_t *param) {
	usb_status = sc;
}

uint8_t config_hid() {
	hid_dev = device_get_binding("HID_0");
	if (hid_dev == NULL) {
		LOG_ERR("Cannot get USB HID Device");
		return 0;
	}

	usb_hid_register_device(hid_dev, hid_report_descriptor, sizeof(hid_report_descriptor), NULL);
	usb_hid_init(hid_dev);
	if (usb_enable(usb_status_callback)) {
		LOG_ERR("Failed to enable USB");
		return 0;
	}
	return 1;
}

static uint8_t report = 0;
void hid_add_to_report(uint8_t btns, int8_t x, int8_t y) {
	rep_status[X_REP_POS] = (uint8_t) ((int8_t)rep_status[X_REP_POS] + x);
	rep_status[Y_REP_POS] = (uint8_t) ((int8_t)rep_status[Y_REP_POS] + y);
	rep_status[BTN_REP_POS] = btns;
	report = 1;
}

uint8_t hid_report() {
	if (!report) return 1;

	int ret = hid_int_ep_write(hid_dev, rep_status, sizeof(rep_status), NULL);
	if (ret) {
		LOG_ERR("write error, %d", ret);
		return 0;
	}
	report = 0;
	rep_status[BTN_REP_POS] = 0;
	rep_status[X_REP_POS] = 0;
	rep_status[Y_REP_POS] = 0;
	return 1;
}


