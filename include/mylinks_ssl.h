#ifndef __MYLINKS_SSL_H
#define __MYLINKS_SSL_H
/***********************************************************************************************
*函数名 ： platform_ssl_connect
*函数功能描述 ：SSL连接
*函数参数 ： socket-socket句柄 ca_crt-CA文件地址 ca_crt_len-CA文件长度 client_crt-client_crt文件地址，client_crt_len-client_crt长度
			 client_key-client_key文件地址 client_key_len=client_key长度
*函数返回值 ： NULL-SSL连接失败 SSL句柄-SSL连接成功
*函数创建日期 ： 2019-5-25
*函数修改日期 ：
***********************************************************************************************/
extern void * platform_ssl_connect(int socket, const char *ca_crt, int ca_crt_len,const char *client_crt, int client_crt_len,const char *client_key,int client_key_len){
;


/***********************************************************************************************
*函数名 ： platform_ssl_recv
*函数功能描述 ： SSL读取数据
*函数参数 ： ssl-ssl句柄 buf-数据读取起始地址len-数据读取长度
*函数返回值 ： -1-SSL连接出错 >=0-读取数据的长度
*函数创建日期 ： 2019-5-25
*函数修改日期 ： 
***********************************************************************************************/
extern int platform_ssl_recv(void *ssl, char *buf, int len);

/***********************************************************************************************
*函数名 ： platform_ssl_send
*函数功能描述 ： SSL发送数据
*函数参数 ： ssl-ssl句柄 buf-数据发送起始地址 len-数据发送长度
*函数返回值 ： -1-SSL连接出错 >=0-发送数据的长度
*函数创建日期 ： 2019-5-25
*函数修改日期 ： 
***********************************************************************************************/
extern int platform_ssl_send(void *ssl, const char *buf, int len);


/***********************************************************************************************
*函数名 ： platform_ssl_close
*函数功能描述 ： 关闭SSL连接
*函数参数 ： ssl-ssl句柄
*函数返回值 ： 0-SSL关闭成功
*函数创建日期 ： 2019-5-25
*函数修改日期 ： 
***********************************************************************************************/
extern int platform_ssl_close(void *ssl);



#endif


