#include <rtthread.h>
#include <rtdevice.h>

#define LOG_TAG "i2c_app"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>
//传输单个字节函数
void i2c_sample_single_byte_write(void)
{
    struct rt_i2c_bus_device * i2c_bus;
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[2];

    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c2");
    if(i2c_bus == RT_NULL)
    {
        LOG_E(" can't find %s device ! \n","i2c2");
    }
    buf[0] = 0x6B;

    //i2c传输信息格式，地址，标志，内容，长度
    msgs.addr = 0x68;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 1;
    if(rt_i2c_transfer(i2c_bus,&msgs,1) == 1)
    {
        LOG_I("single byte write success!");
    }
    else
        LOG_E("sigle byte write failed!");
}
MSH_CMD_EXPORT(i2c_sample_single_byte_write,i2c_sample_single_byte_write);
