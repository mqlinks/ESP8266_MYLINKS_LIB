/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "espressif/espconn.h"
#include "espressif/airkiss.h"
#include "MQTTFreeRTOSTcp.h"
#include "MQTTClientTcp.h"

static uint8_t sendbuf[512];
static uint8_t readbuf[512];
static Network n;
static MQTTClient client={0};

#define CHECK_MQTT_ERROR() do{if(g_rc == FAILURE){MQTT_deinit();printf("mqtt err:%d\n",g_rc);}}while(0)
#define MQTT_SERVER "mqtt.mqlinks.com"
#define MQTT_PORT 1883
#define SSID "Mylinks"
#define PWD "welcometomylinks"

void ICACHE_FLASH_ATTR
smartconfig_done(sc_status status, void *pdata)
{
    switch(status) {
        case SC_STATUS_WAIT:
            printf("SC_STATUS_WAIT\n");
            break;
        case SC_STATUS_FIND_CHANNEL:
            printf("SC_STATUS_FIND_CHANNEL\n");
            break;
        case SC_STATUS_GETTING_SSID_PSWD:
            printf("SC_STATUS_GETTING_SSID_PSWD\n");
            sc_type *type = pdata;
            if (*type == SC_TYPE_ESPTOUCH) {
                printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
            } else {
                printf("SC_TYPE:SC_TYPE_AIRKISS\n");
            }
            break;
        case SC_STATUS_LINK:
            printf("SC_STATUS_LINK\n");
            struct station_config *sta_conf = pdata;
	        wifi_station_disconnect();	
	        wifi_station_set_config(sta_conf);
	        wifi_station_connect();
            break;
        case SC_STATUS_LINK_OVER:
            printf("SC_STATUS_LINK_OVER\n");
            if (pdata != NULL) {
				//SC_TYPE_ESPTOUCH
                uint8 phone_ip[4] = {0};

                memcpy(phone_ip, (uint8*)pdata, 4);
                printf("Phone ip: %d.%d.%d.%d\n",phone_ip[0],phone_ip[1],phone_ip[2],phone_ip[3]);
            } 
            smartconfig_stop();
            break;
    }
	
}



/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}
static void ICACHE_FLASH_ATTR
MQTT_recv_data(MessageData* md)
{
	int t;
	MQTTMessage* message = md->message;
	MQTTString * topicName = md->topicName;
	
	/******* Debug ********/
	printf("topic:%s,data:%s\n",topicName->lenstring.data,message->payload);

	return;
}

static void ICACHE_FLASH_ATTR
MQTT_deinit(void)
{
	if (client.isconnected){
		MQTTDisconnect(&client);
		printf("mqtt off\r\n");
	}
	MQTTClientDeinit(&client);
	FreeRTOS_closesocket(&n);
	return;
}


LOCAL int ICACHE_FLASH_ATTR
MQTT_init(void)
{
	int _rc = 0;
	int keeplive = 4000;
	FreeRTOS_NetworkInit(&n);
	//只需要填写MQTT协议的服务器IP或者域名，端口号。
	_rc = FreeRTOS_NetworkConnect(&n,MQTT_SERVER,MQTT_PORT);
	printf("_rc:%d\n",_rc);
	if (_rc < 0) {
		goto exit;
	}
	printf("mqtt_init\n");

	setsockopt(n.my_socket, SOL_SOCKET, SO_KEEPALIVE, (char *) &keeplive,sizeof(int)) ;
	//注册MQTT发送和接收的缓存
	MQTTClientInit(&client, &n, 2000, sendbuf, sizeof(sendbuf) - 1, readbuf, sizeof(readbuf) - 1);
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

	/***注意：这里请自行修改，以免与其它用户的ID号重命名***/
	data.clientID.cstring = "test_001";
	data.MQTTVersion = 4;
	//如果无账号密码。则使用NULL
	data.username.cstring = NULL;
	data.password.cstring = NULL;
	data.keepAliveInterval = 60;//60s
	data.cleansession = 1;
	data.willFlag = 0;
	data.will.topicName.cstring = NULL;
	data.will.message.cstring = NULL;
	data.will.retained = 0;
	data.will.qos = 0;

	//连接MQTT服务器
	_rc = MQTTConnect(&client, &data);
	if (_rc != SUCCESS) {
		goto exit;
	}
	//设置MQTT的订阅号以及订阅号的回调函数
	_rc = MQTTSubscribe(&client, "iottest", QOS1, MQTT_recv_data);
	if (_rc == FAILURE) {
		goto exit;
	}

	printf("mqtt on\r\n");
	return _rc;
exit:
	MQTT_deinit();
	_rc = FAILURE;
	return _rc;
}


void ICACHE_FLASH_ATTR
mqtt_task(void *pvParameters)
{
	int g_rc = FAILURE;
	for(;;){
		if(STATION_GOT_IP != wifi_station_get_connect_status()){
			if(g_rc == FAILURE)
				goto out;
			g_rc = FAILURE;
			MQTT_deinit();
		}else if(FAILURE == g_rc){
			g_rc = MQTT_init();
			CHECK_MQTT_ERROR();
		}else{
			g_rc = MQTTYield(&client, 50);
			CHECK_MQTT_ERROR();
		}
out:
		vTaskDelay((g_rc == FAILURE)?600:10);
	}
}








/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_init(void)
{
	struct station_config stationConf;
	if(!encryption_device())
	{
		printf("encryption ok\r\n");
	}
    printf("SDK version:%s\n", system_get_sdk_version());

	
	//spi_flash_erase_sector(user_rf_cal_sector_set() - 2);
    /* need to set opmode before you set config */
    wifi_set_opmode(STATION_MODE);

    memset(&stationConf,0,sizeof(struct station_config));
	strcpy(stationConf.ssid,SSID);
	strcpy(stationConf.password,PWD);
    wifi_station_set_config_current(&stationConf);
    wifi_station_connect();
    xTaskCreate(mqtt_task, "smartconfig_task", 256, NULL, 2, NULL);
}

