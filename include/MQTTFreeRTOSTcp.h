/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

#if !defined(MQTTFreeRTOS_H)
#define MQTTFreeRTOS_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define MAX_MESSAGE_HANDLERS 4

typedef struct {
	int len;
	char* data;
} MQTTLenString;

typedef struct {
	unsigned char* cstring;
	MQTTLenString lenstring;
} MQTTString;


typedef struct {
	/** The eyecatcher for this structure.  must be MQTW. */
	char struct_id[4];
	/** The version number of this structure.  Must be 0 */
	int struct_version;
	/** The LWT topic to which the LWT message will be published. */
	MQTTString topicName;
	/** The LWT payload. */
	MQTTString message;
	/**
	  * The retained flag for the LWT message (see MQTTAsync_message.retained).
	  */
	unsigned char retained;
	/**
	  * The quality of service setting for the LWT message (see
	  * MQTTAsync_message.qos and @ref qos).
	  */
	char qos;
} MQTTPacket_willOptions;


typedef struct Timer {
	TickType_t xTicksToWait;
	xTimeOutType xTimeOut;
} Timer;

typedef struct Network Network;

struct Network {
	int my_socket;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);

typedef struct Mutex {
	xSemaphoreHandle sem;
} Mutex;

void FreeRTOS_MutexInit(Mutex*);
void FreeRTOS_MutexDeinit(Mutex* mutex);
int FreeRTOS_MutexLock(Mutex*);
int FreeRTOS_MutexUnlock(Mutex*);

typedef struct Thread {
	xTaskHandle task;
} Thread;


int FreeRTOS_read(Network*, unsigned char*, int, int);
int FreeRTOS_write(Network*, unsigned char*, int, int);
void FreeRTOS_closesocket(Network*);

void FreeRTOS_NetworkInit(Network*);
int FreeRTOS_NetworkConnect(Network*, char*, int);

#endif
