#include <rtthread.h>

// 定义线程优先级和时间片
#define THREAD_PRIORITY      9
#define THREAD_TIMESLICE     5

// 定义线程栈大小
#define THREAD_STACK_SIZE    512

// 定义事件集标志位
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)
#define EVENT_FLAG3 (1 << 3)
#define EVENT_FLAG4 (1 << 4)
#define EVENT_FLAG5 (1 << 5)
#define EVENT_FLAG6 (1 << 6)
#define START_FLAG  (1 << 7)
#define OVER_FLAG   (1 << 8)

// 事件控制块
static struct rt_event event;

// 线程函数
static void event_1(void *param)
{
    rt_uint32_t e;
    if (rt_event_recv(&event, START_FLAG, RT_EVENT_FLAG_OR, RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("这是进程1\n");
        rt_event_send(&event, EVENT_FLAG1);
    }
}

static void event_2(void *param)
{
    rt_uint32_t e;
    if (rt_event_recv(&event, (START_FLAG | EVENT_FLAG1), RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("这是进程2\n");
        rt_event_send(&event, EVENT_FLAG2);
    }
}

static void event_3(void *param)
{
    rt_uint32_t e;
    if (rt_event_recv(&event, (START_FLAG | EVENT_FLAG1), RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("这是进程3\n");
        rt_event_send(&event, EVENT_FLAG3);
    }
}

static void event_4(void *param)
{
    rt_uint32_t e;
    if (rt_event_recv(&event, (START_FLAG | EVENT_FLAG1 | EVENT_FLAG2), RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("这是进程4\n");
        rt_event_send(&event, EVENT_FLAG4);
    }
}

static void event_5(void *param)
{
    rt_uint32_t e;
    if (rt_event_recv(&event, (START_FLAG | EVENT_FLAG1 | EVENT_FLAG2), RT_EVENT_FLAG_AND, RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("这是进程5\n");
        rt_event_send(&event, EVENT_FLAG5);
    }
}

static void event_6(void *param)
{
    rt_uint32_t e;
    if (rt_event_recv(&event, (START_FLAG | EVENT_FLAG1 | EVENT_FLAG2 | EVENT_FLAG3 | EVENT_FLAG4 | EVENT_FLAG5), 
                      (RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR), RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        rt_kprintf("这是进程6\n");
        rt_event_send(&event, OVER_FLAG);
    }
}

int MY_EVENT(void)
{
    rt_uint32_t e;

    // 初始化事件对象
    rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);

    // 创建线程
    rt_thread_t event1 = rt_thread_create("event1", event_1, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_t event2 = rt_thread_create("event2", event_2, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_t event3 = rt_thread_create("event3", event_3, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_t event4 = rt_thread_create("event4", event_4, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_t event5 = rt_thread_create("event5", event_5, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_t event6 = rt_thread_create("event6", event_6, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    // 启动线程
    if (event1 != RT_NULL) rt_thread_startup(event1);
    if (event2 != RT_NULL) rt_thread_startup(event2);
    if (event3 != RT_NULL) rt_thread_startup(event3);
    if (event4 != RT_NULL) rt_thread_startup(event4);
    if (event5 != RT_NULL) rt_thread_startup(event5);
    if (event6 != RT_NULL) rt_thread_startup(event6);

    // 发送初始事件信号
    rt_event_send(&event, START_FLAG);

    // 等待所有线程执行完毕并删除线程
    while (1)
    {
        if (rt_event_recv(&event, OVER_FLAG, (RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR), RT_WAITING_FOREVER, &e) == RT_EOK)
        {
            rt_thread_delete(event1);
            rt_thread_delete(event2);
            rt_thread_delete(event3);
            rt_thread_delete(event4);
            rt_thread_delete(event5);
            rt_thread_delete(event6);
            break;
        }
    }

    return 0;
}

// 导出到 msh 命令列表中
MSH_CMD_EXPORT(MY_EVENT, 事件集例子);
