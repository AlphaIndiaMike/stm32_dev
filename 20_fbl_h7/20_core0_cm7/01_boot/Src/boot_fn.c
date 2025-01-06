// port_disco_svc.c

#include "boot_fn.h"
#include "svo_svc.h"        // For serial_hal_svc_send
#include "stm32h7xx_hal.h"  // STM32 HAL library
#include <string.h>
#include <stdio.h>

#define FBL_RESET_HANDLER_OFFSET	4
#define JUMP_ADDRESS                0x8060800


void show_logo(void)
{
    // Define the "AlphaBoot" ASCII art
    const char* alpha_boot_art =
"                                                                     \n "
"          d8888 888        888   888888b.                     888    \n "
"         d88888 8888      8888   888   88b                    888    \n " 
"        d88P888 888 88  88 888   888  .88P                    888    \n "
"       d88P 888 888  8888  888   8888888K.   .d88b.   .d88b.  888888 \n "
"      d88P  888 888   88   888   888   Y88b d88  88b d88  88b 888    \n "
"     d88P   888 888        888   888    888 888  888 888  888 888    \n "
"    d8888888888 888        888   888   d88P Y88..88P Y88..88P Y88b.  \n "
"   d88P     888 888        888   8888888P     Y88P     Y88P    Y8888 \n "
"                                                                     \n "
"                                                                     \n "
"                                                                     \n";

    // Send the "AlphaBoot" graphic
    serial_hal_svc_send(alpha_boot_art);
}

// Helper function to clean both caches
static inline void SCB_CleanInvalidateCache(void) {
    SCB_CleanDCache();
    SCB_InvalidateDCache();
    SCB_InvalidateICache();
}

void initialize_memory(void) {
    // First, enable the power interface clock
    RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;
    
    // Configure MPU settings before enabling caches
    // Disable MPU
    MPU->CTRL = 0;
    
    // Configure MPU regions as needed
    // Example: Configure region for AXISRAM
    MPU->RNR = 0;  // Region 0
    MPU->RBAR = 0x24000000;  // AXI SRAM base address
    MPU->RASR = (1 << MPU_RASR_ENABLE_Pos) |    // Enable region
                (0x13 << MPU_RASR_SIZE_Pos) |    // 256KB size
                (0x3 << MPU_RASR_AP_Pos) |       // Full access (0b011)
                (1 << MPU_RASR_B_Pos);           // Bufferable
    
    // Enable MPU with default memory map as background
    MPU->CTRL = (1 << MPU_CTRL_ENABLE_Pos) | 
                (1 << MPU_CTRL_PRIVDEFENA_Pos);
    
    // Enable D-Cache and I-Cache
    SCB_EnableICache();
    SCB_EnableDCache();
    
    // Enable RAM clocks
    RCC->AHB2ENR |= (RCC_AHB2ENR_SRAM1EN | 
                     RCC_AHB2ENR_SRAM2EN | 
                     RCC_AHB2ENR_SRAM3EN);
    RCC->AHB3ENR |= RCC_AHB3ENR_AXISRAMEN;
    
    __DSB();
    __ISB();
}

void fbl_jump_app(const unsigned long address) {
    // Validate the address
    if ((address & 0xFF000000) != 0x08000000 &&  // Flash
        (address & 0xFF000000) != 0x24000000) {  // AXISRAM
        serial_hal_svc_send("Error invalid address \n");
        return;  // Invalid address
    }
    
    // Verify stack pointer alignment and range
    uint32_t stack_pointer = *(volatile uint32_t*)address;
    if ((stack_pointer & 0x7) != 0 ||  // Must be 8-byte aligned
        stack_pointer < 0x24000000 ||   // Below AXISRAM
        stack_pointer > 0x24080000) {   // Above AXISRAM
        serial_hal_svc_send("Error invalid stack pointer \n");
        serial_hal_svc_send_param("Stack pointer address: ",stack_pointer);
        return;  // Invalid stack pointer
    }
    
    uint32_t reset_handler = *(volatile uint32_t*)(address + FBL_RESET_HANDLER_OFFSET);

    // Initialize memory and caches
    initialize_memory();
    
    // Disable all interrupts and SysTick
    __disable_irq();
    SysTick->CTRL = 0;
    
    // Clear and disable all NVIC interrupts
    for (int i = 0; i < 8; i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }
    
    // Set vector table location
    SCB->VTOR = address;
    
    // Clean caches before jump
    SCB_CleanInvalidateCache();
    __DSB();
    __ISB();
    
    // Set stack pointer
    __set_MSP(stack_pointer);
    
    // Jump to application
    ((void (*)(void))reset_handler)();
}

void boot_jump_app(void)
{
    show_logo();
    serial_hal_svc_send_param("Jumping to address: ",JUMP_ADDRESS);
    serial_hal_svc_send("\n\n");
    fbl_jump_app(JUMP_ADDRESS);
}
