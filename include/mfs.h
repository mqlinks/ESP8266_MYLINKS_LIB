#ifndef __MFS_H
#define __MFS_H

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,			/* 0: Successful */
	RES_ERROR = -1,		/* 1: R/W Error */
	RES_WRPRT = -2,		/* 2: Write Protected */
	RES_NOTRDY = -3,	/* 3: Not Ready */
	RES_PARERR = -4		/* 4: Invalid Parameter */
} DRESULT;

typedef enum
{
	_SEEK_SET = 0,//参数offset即为新的读写位置
	_SEEK_CUR,//从当前的读写位置往后增加offset个偏移地址
	_SEEK_END,//将读写位置指向文件尾后增加0ffset个偏移地址
} _SEEK;

struct minifs{
	//文件系统地址
	uint32_t start;
	//文件长度
	uint32_t fillen;
	//文件偏移地址
	uint32_t offset;
};



/***********************************************************************************************
*函数名 ： mfmount
*函数功能描述 ： 挂载文件系统
*函数参数 ： addr-mini只读文件系统在flash上的起始地址
*函数返回值 ： RES_ERROR-当前地址不是mini文件系统 RES_NOTRDY-无法读取flash RES_OK-文件系统挂载成功
*函数创建日期 ： 2019-3-10
*函数修改日期 ： 
***********************************************************************************************/
extern DRESULT mfmount(uint32_t addr);



/***********************************************************************************************
*函数名 ： mfopen
*函数功能描述 ： 打开一个文件句柄
*函数参数 ： fs-文件句柄 fname-打开的文件名
*函数返回值 ： RES_ERROR-未找到文件 RES_NOTRDY-无法读取flash RES_OK-文件打开成功
*函数创建日期 ： 2019-3-10
*函数修改日期 ： 
***********************************************************************************************/
extern DRESULT mfopen(struct minifs *fs,const char *fname);

//uint8_t buf:读取的缓冲开始地址,uin32_t len，读取长度
//返回:读取的文件长度

/***********************************************************************************************
*函数名 ： mfread
*函数功能描述 ： 打开一个文件句柄
*函数参数 ： fs-文件句柄 buf-读取的缓存初地址 len-读取的文件长度
*函数返回值 ： 实际读取的文件长度
*函数创建日期 ： 2019-3-10
*函数修改日期 ： 
***********************************************************************************************/
extern uint32_t mfread(struct minifs *fs,uint8_t *buf,uint32_t len);


/***********************************************************************************************
*函数名 ： mftell
*函数功能描述 ： 读取当前文件的文件长度
*函数参数 ： fs-文件句柄
*函数返回值 ： 当前文件长度
*函数创建日期 ： 2019-3-10
*函数修改日期 ： 
***********************************************************************************************/
extern uint32_t mftell(struct minifs *fs);

//uint32_t offset:位置地址,int fromwhere:从何处移动相对地址
//返回:当前文件的读取位置
/***********************************************************************************************
*函数名 ： mfseek
*函数功能描述 ： 在mopen语句打开的文件中指定当前的读位置
*函数参数 ： fs-文件句柄 offset-偏移位置 fromwhere-_SEEK_SET:文件开头,_SEEK_CUR:当前位置,_SEEK_END:文件结尾
*函数返回值 ： 当前文件位置
*函数创建日期 ： 2019-3-10
*函数修改日期 ： 
***********************************************************************************************/
extern uint32_t mfseek(struct minifs *fs, uint32_t offset,_SEEK fromwhere);

#endif

