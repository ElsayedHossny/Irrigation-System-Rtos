#ifndef SENSOR_DHT22_INT_H_
#define SENSOR_DHT22_INT_H_



void DHT_SensorINT(void);

void DHT_SensorRunnable(void);
u16 DHT_Tempgetter(void);
u16 DHT_Humiditygetter(void);
u16 DHT_CRCSumgetter(void);
u16 DHT_CheckSum(void);
u16 Check_Sum_Only(void);


#endif /* SENSOR_DHT22_INT_H_ */