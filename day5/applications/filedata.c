#include <board.h>
#include <rtthread.h>
#include <drv_gpiO.h>
#include <dfs_posix.h>

extern float temp;
extern float humidity;
char buffer1[1000] = {0};

char s1[100] = {0};
void file_data(void * parameter)
{
    //文件描述符
    int fd1;

    //用只写的方式打开文件，如果没有该文件会新创建一个文件
    fd1 = open("/fal/FileData.txt",O_WRONLY | O_CREAT);
    //如果打开成功
    if(fd1 >= 0 )
    {
        uint32_t a = 0;
        rt_kprintf("write success\n");
        while(a++<5)
        {
            rt_snprintf(s1,sizeof(s1),"temperature: %.2f,humidity: %.2f",temp,humidity);
            write(fd1,s1,sizeof(s1));
            rt_thread_mdelay(1000);
        }
        close(fd1);
    }
    else
        rt_kprintf("file open failed\n");

    //以只读的方式打开文件

    fd1 = open("fal/Filedata.txt",O_RDONLY);

    if(fd1 >=0)
    {
        rt_thread_t m = read(fd1,buffer1,sizeof(buffer1));
        if(m<0)
        {
            rt_kprintf("read file failed\n");
            return ;
        }
        rt_kprintf("%s\n",buffer1);
        rt_kprintf("read file success\n");
        close(fd1);
    }
}
MSH_CMD_EXPORT(file_data,FILe_test);
