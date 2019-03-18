#ifndef __MYLINKS_H
#define __MYLINKS_H
/***********************************************************************************************
*函数名 ： encryption_device
*函数功能描述 ： 判断设备是否已经注册
*函数参数 ： 无
*函数返回值 ： -1-设备未注册 0-设备已成功
*函数创建日期 ： 2019-3-9
*函数修改日期 ： 
***********************************************************************************************/
extern int encryption_device();


/***********************************************************************************************
*函数名 ： encryption_from_block
*函数功能描述 ： 从Flash某一个块判断设备是否已经注册
*函数参数 ： 无
*函数返回值 ： -1-设备未注册 0-设备已成功
*函数创建日期 ： 2019-3-11
*函数修改日期 ： 
***********************************************************************************************/
extern int encryption_from_block(uint16_t block);

/***********************************************************************************************
*函数名 ： get_serial_number
*函数功能描述 ： 从服务器获取设备的注册信息，并保存在flash上
*函数参数 ： user_name-用户登录名 block-注册信息Flash上存放的块的位置
*函数返回值 ： -1-未连接路由器 -2-服务器连接失败 -3-向服务器发送数据失败
            -4-未接收到服务器的注册信息 -5-注册信息错误 0-设备已成功
*函数创建日期 ： 2019-3-9
*函数修改日期 ： 
***********************************************************************************************/
extern int get_serial_number(char *user_name,uint16_t block);


/***********************************************************************************************
*函数名 ： update_by_http
*函数功能描述 ： 通过HTTP方式远程升级WIFI固件
*函数参数 ： http_url-升级文件的HTTP路径 port-HTTP服务器端口号 user1_bin-user1文件名 user2_bin-user2文件名
*函数返回值 ： -1-ESP8266未注册 -2-HTTP路径错误 -3-HTTP服务器连接失败 -4-分区擦除失败
            -5-升级失败 -6-HTTP头错误 0-升级成功
*函数创建日期 ： 2019-3-15
*函数修改日期 ： 
***********************************************************************************************/
extern int update_by_http(const char *http_url,int port,const char *user1_bin,const char *user2_bin);



/***********************************************************************************************
*函数名 ： get_the_number_of_temporary_serial_numbers
*函数功能描述 ： 32次临时使用的权限，可提供给客户在未连接网络却需要测试时的使用
*函数参数 ： 无
*函数返回值 ： 剩余使用的次数
*函数创建日期 ： 2019-3-18
*函数修改日期 ： 
***********************************************************************************************/
extern int get_the_number_of_temporary_serial_numbers(void);

#endif


