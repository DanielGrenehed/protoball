#ifndef PROTOBALL_HID_H
#define PROTOBALL_HID_H

#define LEFT_MOUSE_BTN BIT(0)
#define CENTER_MOUSE_BTN BIT(2)
#define RIGHT_MOUSE_BTN BIT(1)

uint8_t config_hid();

void hid_add_to_report(uint8_t btns, int8_t x, int8_t y);

uint8_t hid_report();

#endif /* PROTOBALL_HID_H */
