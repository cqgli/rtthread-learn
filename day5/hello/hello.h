#ifndef __HELLO_H_
#define __HELLO_H_

#include <rtthread.h>
#include <board.h>
#include <drv_gpio.h>

#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)

void led_open(void);
void led_close(void);
#endif
