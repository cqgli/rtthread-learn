#include <rtthread.h>

static rt_mutex_t mutex1 = RT_NULL;
static rt_uint8_t number1,number2 = 0;

static rt_thread_entry1(void * parameter)
{
    while(1)
    {
        rt_mutex_take (mutex1,RT_WAITING_FOREVER);
        number1++;
        rt_thread_mdelay(100);
        number2++;
        rt_mutex_release(mutex1);
    }
}
static rt_thread_entry2(void *parameter)
{
    while(1)
    {
        rt_mutex_take(mutex1,RT_WAITING_FOREVER);
        if(number1 != number2)
        {
            rt_kprintf("thread don't protect number1 : %d \n",number1);
        }
        else
        {
            rt_kprintf("thread protect number1 == number2\n");
        }
        number1++;
        number2++;
        rt_mutex_release(mutex1);
        if(number1 >30)
            return ;
    }
}
int mutex_sample(void)
{
    mutex1 = rt_mutex_create("mutex1",RT_IPC_FLAG_PRIO);
    if(mutex1 ==RT_NULL)
    {
        rt_kprintf(" create mutex failed \n");
        return -1;
    }
    rt_thread_t th1 = rt_thread_create("thread1",rt_thread_entry1,RT_NULL,1024,20,10);
    if(th1 != RT_NULL)
    {
        rt_kprintf("thread1 create success\n");
        rt_thread_startup(th1);
    }

      rt_thread_t th2 = rt_thread_create("thread2",rt_thread_entry2,RT_NULL,1024,19,10);
    if(th2 != RT_NULL)
    {
        rt_kprintf("thread2 create success\n");
        rt_thread_startup(th2);
    }
}
MSH_CMD_EXPORT(mutex_sample,mutex_sample);