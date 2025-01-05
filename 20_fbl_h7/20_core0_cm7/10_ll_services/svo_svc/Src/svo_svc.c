
#include "svo_svc.h"
#include <stdio.h>
#include <string.h>

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

int intToStrThreadSafe(int number, char *buffer, size_t bufferLen)
{
    #define BASE_NO   16

    // Sanity checks
    if (buffer == NULL || bufferLen == 0) {
        return -1;  // Failure
    }

    // We need at least space for one digit + null terminator.
    // If number is negative, we also need space for the '-' sign.
    if (bufferLen < 2) {
        return -1;  // Not enough space even for "0\0"
    }

    // If the number is zero, just return "0"
    if (number == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 0;   // Success
    }

    int i = 0;
    int isNegative = 0;

    // Check sign
    if (number < 0) {
        isNegative = 1;
        // WARNING: If number == INT_MIN, doing `-number` could overflow in 32-bit.
        // For a robust solution, handle INT_MIN explicitly.
        number = -number;
    }

    // Convert digits in reverse
    while (number > 0) {
        if ((size_t)i >= bufferLen - 1) {
            // No space left for digit + null terminator
            return -1;  // Failure
        }
        buffer[i++] = (char)('0' + (number % BASE_NO));
        number /= BASE_NO;
    }

    // Add '-' if negative
    if (isNegative) {
        if ((size_t)i >= bufferLen - 1) {
            // No space for '-' + null terminator
            return -1;  
        }
        buffer[i++] = '-';
    }

    // Add null terminator
    if ((size_t)i >= bufferLen) {
        // Just in case
        return -1;
    }
    buffer[i] = '\0';

    // Reverse the string in-place
    {
        int start = 0;
        int end = i - 1;
        while (start < end) {
            char tmp = buffer[start];
            buffer[start] = buffer[end];
            buffer[end] = tmp;
            start++;
            end--;
        }
    }

    return 0; // Success
}

void serial_hal_svc_send_param(const char *message, long long param) {
    char output[255] = "";
    char param_str[15] = "";
    strcat(output, message);
    intToStrThreadSafe(param,param_str,15);
    strcat(output, param_str);
    serial_hal_svc_send(output);
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