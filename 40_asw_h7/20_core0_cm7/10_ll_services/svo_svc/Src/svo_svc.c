
#include "svo_svc.h"
/**
 * @brief  Initialize SWO for ITM
 */
void serial_hal_svc_init(void)
{
    #define SWO_BASE (0x5C003000UL)
    #define SWTF_BASE (0x5C004000UL)

    uint32_t SWOSpeed = 2000000; // [Hz] we have 2 Mbps SWO speed in ST-Link SWV viewer
    uint32_t SWOPrescaler = (SystemCoreClock / SWOSpeed) - 1; /* divider value */

    //enable debug clocks
    DBGMCU->CR = 0x00700000; //enable debug clocks

    //UNLOCK FUNNEL
    //SWTF->LAR unlock
    *((__IO uint32_t *)(SWTF_BASE + 0xFB0)) = 0xC5ACCE55; //unlock SWTF

    //SWO->LAR unlock
    *((uint32_t *)(SWO_BASE + 0xFB0)) = 0xC5ACCE55; //unlock SWO

    //SWO divider setting
    //This divider value (0x000000C7) corresponds to 400Mhz core clock
    //SWO->CODR = PRESCALER[12:0]
    *((__IO uint32_t *)(SWO_BASE + 0x010)) = SWOPrescaler; //clock divider

    //SWO set the protocol
    //SWO->SPPR = PPROT[1:0] = NRZ
    *((__IO uint32_t *)(SWO_BASE + 0x0F0)) = 0x00000002; //set to NRZ

    //Enable ITM input of SWO trace funnel, slave 0
    //SWTF->CTRL bit 0 ENSO = Enable
    *((__IO uint32_t *)(SWTF_BASE + 0x000)) |= 0x00000001; //enable
}

void serial_hal_svc_send(const char *message) {
    while (*message) {
        // Wait until ITM or stimulus port is ready
        while (!(ITM->TER & (1UL << 0)));
        // Wait until ITM stimulus port 0 is ready
        while (!(ITM->PORT[0].u32));
        // Send character
        ITM_SendChar((uint32_t)*message++);
    }
}