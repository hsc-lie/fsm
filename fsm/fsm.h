#ifndef __FSM_H_
#define __FSM_H_

#include <stddef.h>
#include <stdint.h>

typedef void (*FSM_Entry_t)(void *obj, uint32_t prevState);
typedef uint32_t (*FSM_Process_t)(void *obj);
typedef void (*FSM_Exit_t)(void *obj, uint32_t nextState);

typedef struct
{
    FSM_Entry_t Entry;        //进入状态执行的函数
    FSM_Process_t Process;    //该状态处理的函数
    FSM_Exit_t Exit;          //退出状态执行的函数
}FSM_Item_t;

typedef struct
{
    uint32_t CurrentState;
    uint32_t PrevState;

    const FSM_Item_t *Table;        //状态列表
    uint32_t TableLen;        //状态列表的长度
}FSM_t;

extern void FSM_Init(FSM_t *fsm, const FSM_Item_t *table, uint32_t tableLen, uint32_t firstState);
extern void FSM_Process(FSM_t *fsm, void *obj);
extern void FSM_ChangeState(FSM_t *fsm, void *obj, uint32_t nextState);
extern uint32_t FSM_GetCurrentState(FSM_t *fsm);

#endif /*__FSM_H_*/
