#include <rtthread.h>
#include <rtdevice.h>
#include <aht10.h>

float temp;
float humidity;

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