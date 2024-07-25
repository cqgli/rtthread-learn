/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)
static void thread1_entry (void *parameter)
{
    rt_uint32_t count = 0;
    while(1)
    {
        rt_kprintf("thread1 running..\n");
        rt_pin_write(GPIO_LED_B,PIN_HIGH);
         rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED_B,PIN_LOW);
    }
}
static char thread1_stack[1024];
static struct rt_thread thread1;
static void thread2_entry (void *parameter)
{
    rt_uint32_t count = 0;
    for(int i=0;i<10;i++)
        rt_kprintf("thread2 running..:%d\n",i);

        rt_pin_write(GPIO_LED_R,PIN_HIGH);
         //rt_thread_mdelay(1000);
        rt_pin_write(GPIO_LED_R,PIN_LOW);
       // rt_thread_mdelay(5000);
    rt_kprintf("thread2 exit ...\n");
}
int main(void)
{
    uint32_t m;
    rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_pin_mode(GPIO_LED_B, PIN_MODE_OUTPUT);
    rt_thread_init(&thread1,"thread1",thread1_entry, RT_NULL,&thread1_stack[0],sizeof(thread1_stack),20,5);
    m = rt_thread_create("thread2",thread2_entry,RT_NULL,1024,19,10);
    if(m != RT_NULL)
    {
        rt_kprintf("thread2 create success \n");
    }

    rt_thread_startup(&thread1);
    rt_thread_startup(m);
}




