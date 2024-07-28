#include <rtthread.h>

//邮箱控制块
static rt_mailbox_t mb = RT_NULL;
//struct rt_mailbox (初始化使用，与事件集创建方式类似)

static char mb1[] = "this is a mailbox";
static char mb2[] = "this is another mailbox";
static char mb3[] = "over";

static void thread1_entry1(void *param)
{
    char *str;
    while(1)
    {
        rt_kprintf("thread1 try to recv a mail\n");

        if(rt_mb_recv(mb,(rt_uint32_t *)&str,RT_WAITING_FOREVER)==RT_EOK)
        {
            rt_kprintf("recv success\n");
            if(str == mb3)
            {
                break;
            }
            rt_thread_mdelay(1000);
        }
    }
    rt_mb_delete(mb);
    rt_kprintf("mail over\n");
}

static void thread_entry2(void *param)
{
    rt_uint8_t count = 0;
    while(count <10)
    {
        count++;
        if(count & 0x1)
        {
            rt_mb_send(mb,(rt_uint32_t )&mb1);
        }
        else
        {
            rt_mb_send(mb,(rt_uint32_t )&mb2);
        }
        rt_thread_mdelay(200);
    }
    rt_mb_send(mb,(rt_uint32_t )&mb3);
}

//创建邮箱
int mailbox_sample(void)
{
    mb = rt_mb_create("mail1",1024,RT_IPC_FLAG_PRIO);
    if(mb != RT_NULL)
    {
        rt_kprintf("mb create success\n");
    }

    rt_thread_t th1 = rt_thread_create("thread1",thread1_entry1,RT_NULL,1024,20,10);
    if(th1 != RT_NULL)
    {
        rt_kprintf("th1 create done\n");
        rt_thread_startup(th1);
    }
    
    rt_thread_t th2 = rt_thread_create("thread2",thread_entry2,RT_NULL,1024,19,10);
    if(th2 != RT_NULL)
    {
        rt_kprintf("th3 create done\n");
        rt_thread_startup(th2);
    }
}
MSH_CMD_EXPORT(mailbox_sample,mail);