#include <rtthread.h>
#include <rtdevice.h>
#include <ap3216c.h>

rt_uint16_t ps_data;
float brightness;


void ap3216_entry(void *param)
{
    const char * i2c_bus_name = "i2c2";
    //初始化设备
    ap3216c_device_t dev = ap3216c_init(i2c_bus_name);
    if(dev == RT_NULL)
    {
        rt_kprintf("dev init failed");
        return ;
    }
    //获取数据
   
    while(1)
    {
        ps_data = ap3216c_read_ps_data(dev);
        brightness = ap3216c_read_ambient_light(dev);

        rt_thread_mdelay(500);
    }
   
}
int rt_ap3216(void)
{
    rt_thread_t ap = rt_thread_create("rt_ap3216",ap3216_entry,RT_NULL,1024,20,5);
    if(ap != RT_NULL)
    {
        rt_thread_startup(ap);
    }
    else 
        rt_kprintf(" rt_ap3216 thread create failed\n");
    return 1;
}
MSH_CMD_EXPORT(rt_ap3216,rt_ap3216);