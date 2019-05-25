# ESP8266_MYLINKS_LIB

劢领智能多年在WIFI方面的代码积累，现在向客户开放库，以加快用户的在ESP8266的开发速度以及开发后的稳定性。Lib有劢领智能持续维护，凡是在劢领智能的M0E1系列模块上开发，都可以使用此套库。

#### 注意：目前此Lib不能在SDK 3.0中使用，并且目前只支持RTOS版本，请将libmylinks.a拷贝到lib文件夹中，将在项目的Makefile中加载库。

当前库功能包括以下功能：

## 1.劢领智能模块认证功能:

只有使用了认证函数通过后才可以使用以下的库。如果你使用的不是劢领智能的M0E1系列的模块想要加快模块的开发进度，也可以使用注册函数在连接网络成功以后通过模块认证后加载以下的功能，有需要可以与劢领智能直接联系。函数具体见:include/mylinks.h

注意：如果要使用官方工具合成单一的烧写bin文件，bootloader一定要使用bin/boot-v1.7-4k-0x00000.bin

## 2.TM8211的I2S驱动:

目前此驱动提供了引脚的初始化，TM8211的I2S初始化，以及PCM写入I2S等功能，支持8K至44.1K的速度，以及单通道和双通道。函数具体见:include/i2s.h

## 3.只读mini文件系统：

此文件系统可以将文件打包成一个文件，并且在使用时消耗极小的内存开销，文件名只支持32个字节以下的文件，使用超级方便。函数具体见:include/mfs.h

## 4.通过HTTP更新WIFI固件：

通过HTTP的方式远程升级ESP8266的固件。函数具体见:include/mylinks.h

## 5.通过TCP方式连接MQTT服务器：

通过TCP的方式连接MQTT服务器，属别于原厂mqtt的库在于可以长时间连接而不断开。函数具体见:include/MQTTFreeRTOSTcp.h和include/MQTTClientTcp.h

使用时请注意头文件有依赖关系，具体如下：

#include "include/MQTTFreeRTOSTcp.h"

#include "include/MQTTClientTcp.h"

具体例程：example/mqtt

## 6.红外的38Khz波型：

通过GPIO14产生一个38Khz的波型，用于红外的发射使用。函数具体：

#include "include/38khz.h"



## 7.SSL连接应用：

通过SSL的方式连接服务器，函数具体见:include/mylinks_ssl.h

使用时需要在工程文件中加载libmbedtls.a和libopenssl.a



## RTOS 2.0.0编译器下载：

* [Windows](https://dl.espressif.com/dl/xtensa-lx106-elf-win32-1.22.0-88-gde0bdc1-4.8.5.tar.gz)
* [Mac](https://dl.espressif.com/dl/xtensa-lx106-elf-osx-1.22.0-88-gde0bdc1-4.8.5.tar.gz)
* [Linux(64)](https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-88-gde0bdc1-4.8.5.tar.gz)
* [Linux(32)](https://dl.espressif.com/dl/xtensa-lx106-elf-linux32-1.22.0-88-gde0bdc1-4.8.5.tar.gz)

## 联系我们：

如果有ESP8266使用上有任何的问题，可与我们联系：

##### 技术支持QQ群号：519630819

##### 样品企业店铺：shop.mqlinks.com
