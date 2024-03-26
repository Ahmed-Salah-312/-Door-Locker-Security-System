#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

typedef enum {
    TIMER1_NORMAL,
    TIMER1_CTC
} Timer1_Mode;

typedef enum {
    TIMER1_NO_CLOCK,
    TIMER1_F_CPU_1,
    TIMER1_F_CPU_8,
    TIMER1_F_CPU_64,
    TIMER1_F_CPU_256,
    TIMER1_F_CPU_1024
} Timer1_Prescaler;

typedef struct {
    uint16 initial_value;
    uint16 compare_value; // It will be used in compare mode only.
    Timer1_Prescaler prescaler;
    Timer1_Mode mode;
} Timer1_ConfigType;

void Timer1_init(const Timer1_ConfigType *Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void (*a_ptr)(void));

#endif /* TIMER1_H_ */
