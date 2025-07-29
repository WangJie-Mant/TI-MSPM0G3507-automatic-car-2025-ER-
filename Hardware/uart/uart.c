#include "ti_msp_dl_config.h"
#include "uart.h"

volatile unsigned char uart_data = 0;

void uart_sendChar(char ch)
{
    while (DL_UART_isBusy(UART1_INST) == true);
    DL_UART_Main_transmitData(UART1_INST, ch);
}

void uart_sendString(char* str)
{
    while (*str != 0 && str != 0)
    {
        uart_sendChar(*str++);
    }
}

void UART1_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART1_INST))
    {
        case DL_UART_IIDX_RX:
        {
            uart_data = DL_UART_Main_receiveData(UART1_INST);
            uart_sendChar(uart_data);
            break;
        }
        default: break;
    }
}
