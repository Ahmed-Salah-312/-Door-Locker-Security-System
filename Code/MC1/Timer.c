#include "Timer.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "common_macros.h"

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_OVF_vect)
{
    if (g_callBackPtr != NULL_PTR) {
        (*g_callBackPtr)();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (g_callBackPtr != NULL_PTR) {
        (*g_callBackPtr)();
    }
}

void Timer1_init(const Timer1_ConfigType *Config_Ptr)
{
    TCNT1 = Config_Ptr->initial_value;

    switch (Config_Ptr->mode) {
        case TIMER1_NORMAL:
            SREG |= (1 << 7); // Enable global interrupts
            TIMSK |= (1 << TOIE1); // Overflow Interrupt Enable
            TCCR1A = 0;
            TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);
            break;

        case TIMER1_CTC:
            SREG |= (1 << 7); // Enable global interrupts
            TIMSK |= (1 << OCIE1A); // Compare Interrupt Enable
            OCR1A = Config_Ptr->compare_value;
            TCCR1A = 0;
            TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);
            TCCR1B |= (1 << WGM12); // Set Compare Match Mode
            break;
    }
}

void Timer1_deInit(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    CLEAR_BIT(TIMSK, OCIE1A);
    CLEAR_BIT(TIMSK, OCIE1B);
    CLEAR_BIT(TIMSK, TOIE1);
}

void Timer1_setCallBack(void (*a_ptr)(void))
{
    g_callBackPtr = a_ptr;
}
