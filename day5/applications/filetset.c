#include <board.h>
#include <rtthread.h>
#include <drv_gpiO.h>
#include <dfs_posix.h>

char buffer[100] = {};

char s[]  = "hello,rtthread,welcom to 2024-RSOC";
void File_test(void * parameter)
{
    //文件描述符
    int fd;

    //用只写的方式打开文件，如果没有该文件会新创建一个文件
    fd = open("/fal/FileTest.txt",O_WRONLY | O_CREAT);
    //如果打开成功
    if(fd >= 0 )
    {
        uint32_t a = 0;
        write(fd,s,sizeof(s));
        rt_kprintf("write success\n");
        // while(a<5)
        // {
        //     write(fd,)
        // }
        close(fd);
    }
    else
        rt_kprintf("file open failed\n");

    //以只读的方式打开文件
    fd = open("fal/FileTest.txt",O_RDONLY);

    if(fd >=0)
    {
        rt_thread_t m = read(fd,buffer,sizeof(buffer));
        if(m<0)
        {
            rt_kprintf("read file failed\n");
            return ;
        }
        rt_kprintf("%s\n",buffer);
        rt_kprintf("read file success\n");
        close(fd);
    }
}
MSH_CMD_EXPORT(File_test,FILe_test);

static void readdir_example(void)
{
    DIR * drip;
    struct dirent * d;
    drip = opendir("/fal");
    if(drip == RT_NULL)
    {
        rt_kprintf("open file failed");
        return ;
    }
    while((d = readdir(drip)) != RT_NULL)
    {
        rt_kprintf("found %s \n",d->d_name);
    }
    closedir(drip);
}
MSH_CMD_EXPORT(readdir_example,redirtest);