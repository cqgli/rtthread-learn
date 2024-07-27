#include <hello.h>

#include <rtthread.h>
#include <board.h>
#include <drv_gpio.h>
void led_open(void)
{
    rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_pin_write(GPIO_LED_B,PIN_HIGH);
}
void led_close(void)
{
     rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_pin_write(GPIO_LED_B,PIN_LOW);
}