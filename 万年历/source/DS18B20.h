#ifndef _DS18B20_H_
#define _DS18B20_H_

bit	Start18B20();	 // 启动转换温度
bit Get18B20Temp(int16 *temp); // 获取温度

#endif