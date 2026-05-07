#include "man.h"

static char *Man_StateTxt[MAN_STATE_MAX] =
{
    "起床",
    "干饭",
    "睡觉",
};

static bool Man_Process(Man_t *man, char *process, double timeMax)
{
    struct timeval currentTime;
    double diffTime;
    bool isFinish;

    gettimeofday(&currentTime, NULL);

    //diffTime = difftime(currentTime, man->EntryTime);
    diffTime = (currentTime.tv_sec - man->EntryTime.tv_sec) + (double)(currentTime.tv_usec - man->EntryTime.tv_usec) / 1000000;

    if(diffTime > timeMax)
    {
        printf("\r\033[K%s,过了%.1fs", process, timeMax);
        isFinish = true;
    }
    else
    {
        printf("\r\033[K%s,过了%.1fs", process, diffTime);
        isFinish = false;
    }
    fflush(stdout);

    return isFinish;
}

static char *Man_GetStateTxt(Man_State_t state)
{
    if(state < MAN_STATE_MAX)
    {
        return Man_StateTxt[state];
    }
    else
    {
        return "无";
    }
}

static void Man_GetupEntry(void *obj, uint32_t prevState)
{
    Man_t *man = obj;

    gettimeofday(&man->EntryTime, NULL);

    printf("好累啊(上一个状态:%s)\n", Man_GetStateTxt(prevState));
    fflush(stdout);
}

static uint32_t Man_GetupProcess(void *obj)
{
    Man_t *man = obj;

    if(false == Man_Process(man, "起床中", 1))
    {
        return MAN_STATE_GETUP;
    }
    else
    {
        return MAN_STATE_EAT;
    }
}

static void Man_GetupExit(void *obj, uint32_t nextState)
{
    //Man_t *man = obj;

    printf("\n");
    printf("起来了(下一个状态:%s)\n", Man_GetStateTxt(nextState));

    fflush(stdout);
}


static void Man_EatEntry(void *obj, uint32_t prevState)
{
    Man_t *man = obj;

    //man->EntryTime = time(NULL);
    gettimeofday(&man->EntryTime, NULL);
    printf("开炫(上一个状态:%s)\n", Man_GetStateTxt(prevState));
    fflush(stdout);
}

static uint32_t Man_EatProcess(void *obj)
{
    Man_t *man = obj;

    if(false == Man_Process(man, "干饭中", 1))
    {
        return MAN_STATE_EAT;
    }
    else
    {
        return MAN_STATE_SLEEP;
    }
}

static void Man_EatExit(void *obj, uint32_t nextState)
{
    //Man_t *man = obj;

    printf("\n");
    printf("饱了(下一个状态:%s)\n", Man_GetStateTxt(nextState));
    fflush(stdout);
}

static void Man_SleepEntry(void *obj, uint32_t prevState)
{
    Man_t *man = obj;

    gettimeofday(&man->EntryTime, NULL);
    printf("睡觉(上一个状态:%s)\n", Man_GetStateTxt(prevState));
    fflush(stdout);
}

static uint32_t Man_SleepProcess(void *obj)
{
    Man_t *man = obj;

    if(false == Man_Process(man, "活在梦里", 1))
    {
        return MAN_STATE_SLEEP;
    }
    else
    {
        return MAN_STATE_GETUP;
    }
}

static void Man_SleepExit(void *obj, uint32_t nextState)
{
    //Man_t *man = obj;

    printf("\n");
    printf("醒了(下一个状态:%s)\n", Man_GetStateTxt(nextState));
    fflush(stdout);
}

static const FSM_Item_t Man_StateTalbe[MAN_STATE_MAX] = 
{
#if 1
    {&Man_GetupEntry, &Man_GetupProcess, &Man_GetupExit},
    {&Man_EatEntry, &Man_EatProcess, &Man_EatExit},
    {&Man_SleepEntry, &Man_SleepProcess, &Man_SleepExit},
#else//测试函数为NULL情况
    {NULL, &Man_GetupProcess, &Man_GetupExit},
    {&Man_EatEntry, &Man_EatProcess, &Man_EatExit},
    {&Man_SleepEntry, &Man_SleepProcess, NULL},
#endif
};

void Man_Init(Man_t *man)
{
    FSM_Init(&man->FSM, Man_StateTalbe, MAN_STATE_MAX, MAN_STATE_GETUP);
}

void Man_Life(Man_t *man)
{
    FSM_Process(&man->FSM, man);
}