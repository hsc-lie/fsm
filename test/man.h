#ifndef __MAN_H_
#define __MAN_H_

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include "fsm.h"

typedef enum
{
    MAN_STATE_GETUP = 0,
    MAN_STATE_EAT,
    MAN_STATE_SLEEP,

    MAN_STATE_MAX,
}Man_State_t;

typedef struct
{
    struct timeval EntryTime;
    FSM_t FSM;
}Man_t;

void Man_Init(Man_t *man);
void Man_Life(Man_t *man);

#endif /*__MAN_H_*/
