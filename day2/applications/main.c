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
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#endif /* RT_USING_NANO */

#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)


rt_thread_t th1_ptr = NULL;
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};
void th1_entry(void *parmeter)
{
    for (int i=0;i<10;i++)
    {
        rt_kprintf("th1_entry running\n");
         rt_pin_write(GPIO_LED_B,PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED_B,PIN_LOW);
        rt_thread_mdelay(1000);
    }
}
void th2_entry(void *parmeter)
{
    for (int i=0;i<5;i++)
    {
        rt_pin_write(GPIO_LED_R, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED_R, PIN_LOW);
        rt_thread_mdelay(500);
        rt_kprintf("th2_entry running \n");
        rt_thread_mdelay(500);
    }
}
int main(void)
{
    int ret =0;
     rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_pin_mode(GPIO_LED_B, PIN_MODE_OUTPUT);
    th1_ptr = rt_thread_create("th1_demo", th1_entry, NULL,1024, 20, 5);
    //判断是否创建线程
    if(!ret)
    {
        rt_kprintf("创建失败\n");
        return -RT_ENOMEM;
    }
    rt_kprintf("创建成功\n");
    //启动线程
    rt_thread_startup(th1_ptr);
    //创建第二个线程
    ret = rt_thread_init(&th2,"th2_demo",th2_entry,NULL,th2_stack,sizeof(th2_stack),19,5);
    if(ret<0)
    {
        rt_kprintf("创建失败\n");
        return ret;
    }
    rt_kprintf("创建成功2\n");
    //启动
    rt_thread_startup(&th2);
    return RT_EOK;

}


