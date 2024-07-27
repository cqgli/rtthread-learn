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
#include <board.h>
#include "drv_lcd.h"
#include "aht10.h"
#include <icm20608.h>
#endif /* RT_USING_NANO */
#include <ulog.h>

static rt_sem_t thread_sem = RT_NULL;

void AHT10_enty(void *parm)
{
    rt_sem_take(thread_sem, RT_WAITING_FOREVER);
    //湿度，温度
    float humidity, temperature;
    //创建设备
    aht10_device_t dev;
    const char *i2c_bus_name = "i2c3";
    int count =0;
    rt_thread_mdelay(1000);

    dev = aht10_init(i2c_bus_name);
    if(dev == RT_NULL)
    {
        LOG_E("dev init failed ");
        return 0;
    }
    while(count++ < 10)
    {
        lcd_clear(WHITE);
        humidity = aht10_read_humidity(dev);
        lcd_show_string (10,69,20,(char)humidity);
        rt_thread_mdelay(100);
        temperature = aht10_read_temperture(dev);
        lcd_show_string(20,69,20,(char)temperature);
        rt_thread_mdelay(1000);
    }
    rt_sem_release(thread_sem);
}
void icm_thread_entry(void *parameter)
{
    rt_sem_take(thread_sem, RT_WAITING_FOREVER);
    icm20608_device_t dev = RT_NULL;
    const char *i2c_bus_name = "i2c2";
    int count = 0;
    rt_err_t result;

    /* 初始化 icm20608 传感器 */
    dev = icm20608_init(i2c_bus_name);
    if (dev == RT_NULL)
    {
        LOG_E("The sensor initializes failure");
    }
    else
    {
        LOG_D("The sensor initializes success");
    }

    /* 对 icm20608 进行零值校准：采样 10 次，求取平均值作为零值 */
    result = icm20608_calib_level(dev, 10);
    if (result == RT_EOK)
    {
        LOG_D("The sensor calibrates success");
    }
    else
    {
        LOG_E("The sensor calibrates failure");
        icm20608_deinit(dev);
    }

    while (count++ < 100)
    {
        rt_int16_t accel_x, accel_y, accel_z;
        rt_int16_t gyros_x, gyros_y, gyros_z;

        /* 读取三轴加速度 */
        result = icm20608_get_accel(dev, &accel_x, &accel_y, &accel_z);
        if (result == RT_EOK)
        {
            LOG_D("current accelerometer: accel_x%6d, accel_y%6d, accel_z%6d", accel_x, accel_y, accel_z);
        }
        else
        {
            LOG_E("The sensor does not work");
        }

        /* 读取三轴陀螺仪 */
        result = icm20608_get_gyro(dev, &gyros_x, &gyros_y, &gyros_z);
        if (result == RT_EOK)
        {
            LOG_D("current gyroscope    : gyros_x%6d, gyros_y%6d, gyros_z%6d", gyros_x, gyros_y, gyros_z);
        }
        else
        {
            LOG_E("The sensor does not work");
            break;
        }
        rt_thread_mdelay(1000);
    }
    rt_sem_delete(thread_sem);
}
int thread_semaphore(void)
{
    thread_sem = rt_sem_create("tsem",0,RT_IPC_FLAG_FIFO);
    if(thread_sem = RT_NULL)
    {
        rt_kprintf("create thread-sem failed\n");
        return -1;
    }

    uint32_t m = rt_thread_create("ath10",AHT10_enty,RT_NULL,1024,20,10);
    if(m != RT_NULL)
    {
        rt_kprintf("ath10 create success \n");
        rt_thread_startup(m);
        return 0;
    }

   uint32_t res = rt_thread_create("icm", icm_thread_entry, RT_NULL, 1024, 19, 50);
    if(res == RT_NULL)
    {
        rt_kprintf(" icm create failed \n");
        return -RT_ERROR;
    }
    rt_thread_startup(res);
    rt_thread_mdelay(1000);
    rt_thread_delete(m);
    rt_thread_delete(res);
}
int main(void)
{
    lcd_clear(WHITE);
    lcd_set_color(WHITE,BLACK);
    lcd_show_string(10,69,16,"hello rtthread !");
    thread_semaphore();

    return RT_EOK;
}




