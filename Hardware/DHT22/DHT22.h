#ifndef __DHT22_H
#define	__DHT22_H

void DHT22_IPU(void);
void DHT22_pp(void);
void DHT22_Configuration(void);
void DHT22_0_Int_Init(void);
void set_tempers(void);
void reset_tempers(void);
void plus_tempers(void);
void plus_count_tempers(void);
void minus_tempers(void);
void minus_count_tempers(void);
void DHT22_ts(void);
void DHT22_tst_stop(void);
void DHT22_tst_err(void);
void DHT22_ts_bit(void);
void DHT22_checksum(void);
void DHT22_get(void);
void DHT22_time(void);
#endif
