#ifndef PROTOBALL_PIM_H
#define PROTOBALL_PIM_H

uint8_t config_pim();

void pim_set_callback(void (*cb)(uint8_t l, uint8_t r, uint8_t u, uint8_t d, uint8_t s));

void pim_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

void poll_pim();

#endif /* PROTOBALL_PIM_H */
