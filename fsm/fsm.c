#include "fsm.h"

/*
 * @函数名  FSM_Init
 * @用  途  状态机初始化
 * @参  数  fsm: 状态机结构体
 *          table: 状态的列表
 *          tableLen: 状态列表的长度
 *          firstState: 初始的状态
 * @返回值
*/
void FSM_Init(FSM_t *fsm, const FSM_Item_t *table, uint32_t tableLen, uint32_t firstState)
{
    fsm->CurrentState = firstState;
    fsm->PrevState = firstState - 1;//保正一开始上一个状态和当前的状态不一致，能够调用Entry函数
    fsm->Table = table;
    fsm->TableLen = tableLen;
}

/*
 * @函数名  FSM_Init
 * @用  途  状态机初始化
 * @参  数  fsm: 状态机结构体
 *          obj: 使用状态机的对象
 * @返回值
*/
void FSM_Process(FSM_t *fsm, void *obj)
{
    uint32_t prevState;
    uint32_t currentState;
    uint32_t nextState;
    const FSM_Item_t *table;

    if((NULL == fsm)
    || (NULL == fsm->Table)
    || (NULL == obj)
    )
    {
        return;
    }

    currentState = fsm->CurrentState;
    prevState = fsm->PrevState;
    table = fsm->Table;

    if(currentState >= fsm->TableLen)
    {
        return;
    }

    //执行进入新的状态的处理
    if(currentState != prevState)
    {
        if(NULL != table[currentState].Entry)
        {
            table[currentState].Entry(obj, prevState);
        }
        fsm->PrevState = currentState;
    }

    //执行当前的状态处理
    if(NULL == table[currentState].Process)
    {
        return;
    }
    nextState = table[currentState].Process(obj);

    if(nextState >= fsm->TableLen)
    {
        return;
    }

    //执行退出状态的处理
    if(nextState != currentState)
    {
        if(NULL != table[currentState].Exit)
        {
            table[currentState].Exit(obj, nextState);
        }
        fsm->CurrentState = nextState;
    }
}

/*
 * @函数名  FSM_ChangeState
 * @用  途  改变状态
 * @参  数  fsm: 状态机结构体
 *          obj: 使用状态机的对象
 *          nextState:下一个状态
 * @返回值
*/
void FSM_ChangeState(FSM_t *fsm, void *obj, uint32_t nextState)
{
    uint32_t currentState;
    const FSM_Item_t *table;

    if((NULL == fsm)
    || (NULL == fsm->Table)
    || (NULL == obj)
    )
    {
        return;
    }

    currentState = fsm->CurrentState;
    table = fsm->Table;

    if((currentState >= fsm->TableLen)
    || (nextState >= fsm->TableLen)
    )
    {
        return;
    }

    //执行退出状态的处理
    if(nextState != currentState)
    {
        if(NULL != table[currentState].Exit)
        {
            table[currentState].Exit(obj, nextState);
        }
        fsm->CurrentState = nextState;
    }
}

/*
 * @函数名  FSM_GetCurrentState
 * @用  途  获取当前状态
 * @参  数  fsm: 状态机结构体
 * @返回值  当前状态
*/
uint32_t FSM_GetCurrentState(FSM_t *fsm)
{
    if(NULL == fsm)
    {
        return 0u;
    }

    return fsm->CurrentState;
}