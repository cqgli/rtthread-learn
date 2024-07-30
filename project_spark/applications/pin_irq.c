#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>

//定义按键
#define KEY_UP GET_PIN(C,5)
#define KEY_DOWN GET_PIN(C,1)

#define KEY_LEFT GET_PIN(C,0)
#define KEY_RIGHT GET_PIN(C,4)

//按键回调函数
void key_up_callback( void * args)
{
    uint32_t value  = rt_pin_read(KEY_UP);
    rt_kprintf("key up! %d ",value);
}
void key_down_callback( void * args)
{
    uint32_t value  = rt_pin_read(KEY_DOWN);
    rt_kprintf("key down! %d ",value);
}
void key_left_callback( void * args)
{
    uint32_t value  = rt_pin_read(KEY_LEFT);
    rt_kprintf("key left! %d ",value);
}
void key_right_callback( void * args)
{
    uint32_t value  = rt_pin_read(KEY_RIGHT);
    rt_kprintf("key right! %d ",value);
}
static int rt_irq_init(void)
{
    //初始化引脚模式
    rt_pin_mode(KEY_UP, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_DOWN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_LEFT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_RIGHT, PIN_MODE_INPUT_PULLUP);
    //将其与中断回调函数绑定
    rt_pin_attach_irq(KEY_UP, PIN_IRQ_MODE_FALLING,key_up_callback,RT_NULL);
    rt_pin_attach_irq(KEY_DOWN, PIN_IRQ_MODE_FALLING,key_down_callback,RT_NULL);
    rt_pin_attach_irq(KEY_LEFT, PIN_IRQ_MODE_FALLING,key_left_callback,RT_NULL);
    rt_pin_attach_irq(KEY_RIGHT, PIN_IRQ_MODE_FALLING,key_right_callback,RT_NULL);
    //使能中断
    rt_pin_irq_enable(KEY_UP, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY_DOWN, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY_LEFT, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY_RIGHT, PIN_IRQ_ENABLE);
    return RT_EOK;
}
MSH_CMD_EXPORT(rt_irq_init,rt_irq_init);