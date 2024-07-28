#include <rtthread.h>
static rt_mq_t mq;

//线程一
static void thread1_entry(void * param)
{
    char buf = 0;
    rt_uint8_t count =0;
    while(1)
    {
        if(rt_mq_recv(mq,&buf,sizeof(buf),RT_WAITING_FOREVER)>0)
        {
             rt_kprintf("thread1: recv msg from msg queue, the content:%c\n", buf);
            if(count == 20)
            {
                break;
            }
        }
        count++;
        rt_thread_mdelay(50);
    }
    rt_kprintf("thread1 : delete mq\n");
    rt_mq_delete(mq);
}

static void thread2_entry(void * param)
{
    char buf = 'a';
    rt_err_t result;
    rt_uint8_t cout =0;
    while(1)
    {
        buf++;
        if(cout ==8)
        {
            result = rt_mq_urgent(mq,&buf,1);
            if(result == RT_EOK)
            {
                rt_kprintf("message urgent send done \n");
            }
            else
                rt_kprintf("message send failed\n");
        }
        else if ( cout >= 20)
        {
            rt_kprintf("message send over\n");
            break;
        }
        else
        {
            result = rt_mq_send(mq,&buf,1);
            if(result == RT_EOK)
            {
                rt_kprintf("message  send done \n");
            }
            else
                rt_kprintf("message send failed\n");
        }
        cout++;
        rt_thread_mdelay(50);
    }
}
int mq_sample(void)
{
    mq = rt_mq_create("mq1",5,20,RT_IPC_FLAG_PRIO);
    if(mq != RT_NULL)
    {
        rt_kprintf("mq1 create done\n");
    }
   rt_thread_t th1 = rt_thread_create("th1",thread1_entry,RT_NULL,1024,20,10);
    if(th1 != RT_NULL)
    {
        rt_kprintf("th1 create done\n");
        rt_thread_startup(th1);
    }
    rt_thread_t th2 = rt_thread_create("th2",thread2_entry,RT_NULL,1024,19,10);
    if(th2 != RT_NULL)
    {
        rt_kprintf("th2 create done\n");
        rt_thread_startup(th2);
    }
}
MSH_CMD_EXPORT(mq_sample,mq_sample);
