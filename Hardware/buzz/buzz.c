#include "buzz.h"
#include "ti_msp_dl_config.h"

void buzz_Init(void)
{
    DL_GPIO_clearPins(BUZZ_PORT, BUZZ_BUZZ_1_PIN);
}

void Buzz(uint8_t duration)
{
    DL_GPIO_setPins(BUZZ_PORT, BUZZ_BUZZ_1_PIN);
    while (duration > 0)
    {
        duration--;
    }
    DL_GPIO_clearPins(BUZZ_PORT, BUZZ_BUZZ_1_PIN);
}

