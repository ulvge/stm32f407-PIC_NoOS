#ifndef __SPI_COMMUNICATION_H
#define	__SPI_COMMUNICATION_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdbool.h>
#include <stdint.h>
#include "freertos.h"
#include "semphr.h"

// 定义宏
#define PROTOCOL_DATA_ID        0x03
#define PROTOCOL_DATA_WRITEBACK  0x04
#define PROTOCOL_CMD_CMD 0x0b

#define SPI_RECV_BUFF_GROUP_COUNT 100     // 128K 32000
#define PROTOCOL_CMD_FILED_LEN 4

typedef enum {
    RECVEVENT_WAIT_DATA_ID = 0,
    RECVEVENT_WAIT_DATA_HEAD1,
    RECVEVENT_WAIT_DATA_HEAD2,
    RECVEVENT_WAIT_DATA_COUNT_HI,
    RECVEVENT_WAIT_DATA_COUNT_LOW,
    RECVEVENT_WAIT_DATA_DATA,

    RECVEVENT_RECEVED_CMD_ID,
} FSM_RecvEvent;

typedef enum {
    PROTOCOL_TYPE_UNKNOWN = 0,
    PROTOCOL_TYPE_DATA,
    PROTOCOL_TYPE_DATA_WRITEBACK,
    PROTOCOL_TYPE_CMD,
} PROTOCOL_TYPE;

// 发送数据 PROTOCOL_DATA_ID
typedef struct {
    uint8_t *wp;
    uint32_t recvedGroupCount;
    uint32_t writeByteCount;
    uint8_t isRecvedFinished;
    uint8_t isRecvedOverflow;
    uint8_t isSending;
//Main Body
    uint16_t count __attribute__((aligned(2)));
    struct {
        uint16_t position;
        uint16_t slope;
    } data[SPI_RECV_BUFF_GROUP_COUNT] __attribute__((aligned(2)));
//Main Body end
} ProtocolData;

// 发送命令 PROTOCOL_CMD_CMD
typedef struct {
    uint8_t *wp;
    uint16_t writeByteCount;
//Main Body
    uint16_t reSendTimes __attribute__((aligned(2)));
    uint8_t sleepUsWave;
    uint8_t sleepUsGroupData;
//Main Body end
    uint8_t isRecvedFinished;
}__attribute__ ((packed)) ProtocolCmd ;

// 回读命令 PROTOCOL_DATA_WRITEBACK
typedef struct {
    uint16_t count __attribute__((aligned(2)));
}__attribute__ ((packed)) ProtocolWriteBack ;

extern SemaphoreHandle_t g_sem_WriteBack;

extern ProtocolData g_protocolData;
extern ProtocolCmd g_protocolCmd;
extern ProtocolWriteBack g_protocolWriteBack;
extern void SPI_ProtocolInit(void);
extern void SPI_ProtocolParsing(uint8_t val);
extern void Task_WriteBack(void *argument);
#ifdef __cplusplus
}
#endif

#endif /* __SPI_COMMUNICATION_H */

