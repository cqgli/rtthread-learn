#include <rtthread.h>
#include <rtdevice.h>
#include <aht10.h>
#include <drv_lcd.h>
float temp;
float humidity;
extern rt_uint16_t ps_data;
extern float brightness;
static void aht10_entry(void *params)
{
    aht10_device_t dev;  
    const char * i2c_bus_name = "i2c3";
    int count =0;
    rt_thread_mdelay(1000);
    dev = aht10_init(i2c_bus_name);
     if(dev == RT_NULL)
    {
        rt_kprintf("ath_init failed \n");
    }
    while(1)
    {
        humidity = aht10_read_humidity(dev);
        temp =  aht10_read_temperature(dev);
        rt_thread_mdelay(500);
         lcd_show_string(16,29,16,"temp:%.2f",temp);
        lcd_show_string(16,49,16,"humidity:%.2f",humidity);
        lcd_show_string(16,69,16,"ps_data:%d",ps_data);
        lcd_show_string(16,89,16,"brightness:%.2f",brightness);
        //rt_kprintf("%d %d \n",humidity,temp);
    }
}
int rt_aht10_port(void)
{
    rt_thread_t m = rt_thread_create("aht_thread",aht10_entry,RT_NULL,1024,20,5);
    if(m != RT_NULL)
    {
        rt_thread_startup(m);
    }
    else 
        rt_kprintf(" aht thread create failed\n");
    return 1;
}
MSH_CMD_EXPORT(rt_aht10_port,rt_aht10_port);