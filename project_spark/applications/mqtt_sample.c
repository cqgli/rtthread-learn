/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 * 
 * Again edit by rt-thread group
 * Change Logs:
 * Date          Author          Notes
 * 2019-07-21    MurphyZhao      first edit
 */

#include "rtthread.h"
#include "dev_sign_api.h"
#include "mqtt_api.h"
#include "drv_gpio.h"
#include "cJSON.h"

#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)

extern float temp;
extern float humidity;
extern uint32_t ps_data;
extern float brightness;
int LightSwitch = 1;

 cJSON * cjson = RT_NULL;
 cJSON * params_ali = RT_NULL;

const char * ali_topic_set = "/%s/%s/user/set";
const char * ali_topic_get = "/%s/%s/user/get";
char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};

void *HAL_Malloc(uint32_t size);
void HAL_Free(void *ptr);
void HAL_Printf(const char *fmt, ...);
int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN]);
uint64_t HAL_UptimeMs(void);
int HAL_Snprintf(char *str, const int len, const char *fmt, ...);

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

rt_thread_t msg_receive = RT_NULL;
void msg_receive_entrry(void *pclient)
{
    while(1){
        /* handle the MQTT packet received from TCP or SSL connection */
        IOT_MQTT_Yield(pclient, 200);

        rt_thread_delay(1000);
    }

}

static void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_t     *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            cjson = cJSON_Parse(topic_info->payload);//将JSON字符串转换成JSON结构体
            rt_kprintf("1\n");
            if(cjson == RT_NULL)                       //判断转换是否成功
            {
                rt_kprintf("1\n");
                
            }
            else
            {
                params_ali = cJSON_GetObjectItem(cjson,"params");   //解析对象

                //判断是否有相对应的内容，有才会进行数据提取
                if(cJSON_GetObjectItem(params_ali, "temperature") != RT_NULL){
                    rt_kprintf("temp: %d -> ",temp);
                    temp = cJSON_GetObjectItem(params_ali, "temp")->valueint;
                    rt_kprintf("%.2f\n",temp);
                }

                if(cJSON_GetObjectItem(params_ali, "humidity") != RT_NULL){
                    rt_kprintf("humi: %.f -> ",humidity);
                    humidity = cJSON_GetObjectItem(params_ali, "humi")->valueint;
                    rt_kprintf("%.3f\n",humidity);
                }
                 if(cJSON_GetObjectItem(params_ali, "mlux") != RT_NULL){
                    rt_kprintf("mlux: %.f -> ",brightness);
                    brightness = cJSON_GetObjectItem(params_ali, "mlux")->valueint;
                    rt_kprintf("%.3f\n",brightness);
                }
                if(cJSON_GetObjectItem(params_ali, "ap3216") != RT_NULL){
                    rt_kprintf("ps_data: %.f -> ",ps_data);
                    ps_data = cJSON_GetObjectItem(params_ali, "ap3216")->valueint;
                    rt_kprintf("%.3f\n",ps_data);
                }
                rt_kprintf("2\n");
                if(cJSON_GetObjectItem(params_ali, "LightSwitch") != RT_NULL){
                    rt_kprintf("LightSwitch: %d -> ",LightSwitch);
                    LightSwitch = cJSON_GetObjectItem(params_ali, "LightSwitch")->valueint;
                    rt_kprintf("%d\n",LightSwitch);
                    if(LightSwitch){
                        rt_pin_write(GPIO_LED_R,PIN_LOW);
                        LightSwitch = 1;
                    }
                    else
                       {
                         rt_pin_write(GPIO_LED_R,PIN_HIGH);
                         LightSwitch = 0;
                       }
                }

            cJSON_Delete(cjson);
            }
            /* print topic name and topic message */
            EXAMPLE_TRACE("Message Arrived:");
            EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
            EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
            EXAMPLE_TRACE("\n");

            
            break;
        default:
            break;
    }
   
    
}

static int topic_subscribe(void * handle,const char *ali_topic)
{
    int res = 0;
    char *topic = NULL;
    int topic_len = 0;

    topic_len = strlen(ali_topic) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL) {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, ali_topic, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS1, example_message_arrive, NULL);
    if (res < 0) {
        EXAMPLE_TRACE("%s subscribe failed",ali_topic);
        HAL_Free(topic);
        return -1;
    }

    HAL_Free(topic);
    return 0;
}
static int example_subscribe(void *handle)
{
        topic_subscribe(handle, ali_topic_get);
}

static int example_publish(void *handle)
{
    int             res = 0;
    const char     *fmt = "/sys/%s/%s/thing/event/property/post";
    char           *topic = NULL;
    int             topic_len = 0;
    char           payload[300] = {0};

    rt_snprintf(payload,sizeof(payload),"{\"params\":{\"temperature\":%.2f,\"Humidity\":%.2f,\"LightSwitch\":%d,\"mlux\":%.2f,\"ap3216\":%d}}",temp,humidity,LightSwitch,brightness,ps_data);
    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL) {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, payload, strlen(payload));
    if (res < 0) {
        EXAMPLE_TRACE("publish failed, res = %d", res);
        HAL_Free(topic);
        return -1;
    }
    HAL_Free(topic);
    return 0;
}

static void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
 }


static int mqtt_example_main(int argc, char *argv[])
{
    void                   *pclient = NULL;
    int                     res = 0;
    int                     loop_cnt = 0;
    iotx_mqtt_param_t       mqtt_params;

    HAL_GetProductKey(DEMO_PRODUCT_KEY);
    HAL_GetDeviceName(DEMO_DEVICE_NAME);
    HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);

    EXAMPLE_TRACE("mqtt example");

    
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));
    mqtt_params.handle_event.h_fp =  example_message_arrive;

    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) {
        EXAMPLE_TRACE("MQTT construct failed");
        return -1;
    }

    res = example_subscribe(pclient);
    if (res < 0) {
        IOT_MQTT_Destroy(&pclient);
        return -1;
    }

     msg_receive = rt_thread_create("msg_receive", msg_receive_entrry, pclient, 4096, 18, 10);
    if(msg_receive == RT_NULL){
        rt_kprintf("rt_thread_create msg_receive failed...\n");
    }
    rt_thread_startup(msg_receive);

    while (1) {
        example_publish(pclient);
        rt_thread_mdelay(2000);
       
        lcd_show_string(16,109,16,"lightswitch: %d",LightSwitch);
    }

    return 0;
}


void  ali_mqtt(void)
{
    rt_thread_t mq = rt_thread_create("mqtt_thread",mqtt_example_main,RT_NULL,4096,20,10);
    if(mq != RT_NULL)
    {
        rt_thread_startup(mq);
    }
    else 
    rt_kprintf("mqtt create failed\n");

}
MSH_CMD_EXPORT(ali_mqtt,ali_mqtt);