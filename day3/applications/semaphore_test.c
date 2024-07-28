#include <rtthread.h>
static rt_sem_t sem1 = RT_NULL;

static thread1_entry(void *paramter)
{
    rt_uint8_t count = 0;
    while(1)
    {
        if(count <= 100)
        {
            count++;
        }
        else
            return ;
        if(0 == (count %10))
        {
            rt_kprintf("t1 release sem1 semaphore \n");
            rt_sem_release(sem1);
        }
    }
}

static thread2_entry(void * paramter)
{
    rt_err_t result;
    rt_uint32_t num = 0;
    while(1)
    {
        result = rt_sem_take(sem1,RT_WAITING_FOREVER);
        if(result != RT_EOK)
        {
            rt_kprintf("thread2 take sem1 failed\n");
            rt_sem_delete(sem1);
            return ;
        }
        else
        {
            num++;
            rt_kprintf("thread2 take sem1 success num:%d",num);
        }
    }
}
//初始化信号量
static void semaphore_sample(void)
{
    sem1 = rt_sem_create("sema",RT_NULL,RT_IPC_FLAG_PRIO);
    if(sem1 == RT_NULL)
    {
        rt_kprintf(" sem1 create failed\n");
        return ;
    }
    else
        rt_kprintf("sem1 create success\n");
    rt_thread_t th1 = rt_thread_create("thread1",thread1_entry,RT_NULL,1024,20,10);
    if(th1 != RT_NULL)
    {
        rt_kprintf("thread1 create done\n");
        rt_thread_startup(th1);
    }
     rt_thread_t th2 = rt_thread_create("thread2",thread2_entry,RT_NULL,1024,19,10);
    if(th2 != RT_NULL)
    {
        rt_kprintf("thread2 create done\n");
        rt_thread_startup(th2);
    }
}
MSH_CMD_EXPORT(semaphore_sample,semaphore_sample);