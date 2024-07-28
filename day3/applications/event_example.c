#include <rtthread.h>

//定义事件
#define EVENT1 (1<<3)
#define EVENT2 (1<<5)

static  rt_event_t event;

static void thread1_recv_event(void *param)
{
    rt_uint32_t e;
    if(rt_event_recv(event,(EVENT1 | EVENT2),RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&e)==RT_EOK)
    {
        rt_kprintf("thread1 : OR recv success %x\n",e);
    }
    rt_thread_mdelay(1000);
    if(rt_event_recv(event,(EVENT1 | EVENT2),RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&e)==RT_EOK)
    {
        rt_kprintf("thread1 : AND recv success %x\n",e);
    }
    rt_kprintf("thread1 leave\n");
}

static void thread2_recv_event(void *param)
{
    rt_kprintf("thrread2 : send event1\n");
    rt_event_send(event,EVENT1);
    rt_thread_mdelay(200);

    rt_kprintf("thread2 : send event2\n");
    rt_event_send(event , EVENT2);
    rt_thread_mdelay(200);

    rt_kprintf("thrread2 : send event1\n");
    rt_event_send(event,EVENT1);
    rt_kprintf("thread leave\n");

}
//事件初始化
int event_sample(void)
{

    event = rt_event_create("event",RT_IPC_FLAG_PRIO);
    if(event != RT_NULL)
    {
        rt_kprintf("event create success\n");
    }
    rt_thread_t th1 = rt_thread_create("thread1",thread1_recv_event,RT_NULL,1024,20,5);
    if(th1 != RT_NULL)
    {
        rt_kprintf("thread1 create success\n");
        rt_thread_startup(th1);
    }

     rt_thread_t th2 = rt_thread_create("thread2",thread2_recv_event,RT_NULL,1024,19,5);
    if(th2 != RT_NULL)
    {
        rt_kprintf("thread2 create success\n");
        rt_thread_startup(th2);
    }
}
MSH_CMD_EXPORT(event_sample,eve);