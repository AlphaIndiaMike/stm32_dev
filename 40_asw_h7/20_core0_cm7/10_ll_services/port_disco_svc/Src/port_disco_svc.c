// port_disco_svc.c

#include "port_disco_svc.h"
#include "svo_svc.h"        // For serial_hal_svc_send
#include "stm32h7xx_hal.h"  // STM32 HAL library
#include <string.h>
#include <stdio.h>

/* Define boolean type for C standard */
typedef enum {
    GPIO_FALSE = 0U,
    GPIO_TRUE = 1U
} GPIO_Bool_Type;

/* 
 * Enumeration for GPIO modes.
 * (Kept here for consistency, though the code now uses strings
 * for display; you could remove or reuse this in the future.)
 */
typedef enum {
    GPIO_MODE_TYPE_INPUT = 0U,
    GPIO_MODE_TYPE_OUTPUT_PP,
    GPIO_MODE_TYPE_OUTPUT_OD,
    GPIO_MODE_TYPE_AF_PP,
    GPIO_MODE_TYPE_AF_OD,
    GPIO_MODE_TYPE_ANALOG,
    GPIO_MODE_TYPE_UNKNOWN
} GPIO_Mode_Type;

/* Structure for pin information */
typedef struct {
    uint8_t pin_number;
    /* FIX: changed from GPIO_Mode_Type mode to const char* mode */
    const char* mode;  /* store string pointers instead of an enum */
} GPIO_Pin_Info_Type;

/* Structure for port information */
typedef struct {
    const char* port_name;
    GPIO_TypeDef* port;
    GPIO_Pin_Info_Type pins[16U];
    uint8_t pin_count;
} GPIO_Port_Info_Type;

/* Global data structure */
typedef struct {
    GPIO_Port_Info_Type ports[11U];
    uint8_t total_enabled_ports;
} GPIO_Data_Type;

/* Global static data for GPIO information */
static GPIO_Data_Type gpio_data = {{{0U}}, 0U};

/* Static port mapping array */
static const struct {
    GPIO_TypeDef* port;
    const char* name;
} PORT_MAP[11U] = {
    {GPIOA, "GPIOA"},
    {GPIOB, "GPIOB"},
    {GPIOC, "GPIOC"},
    {GPIOD, "GPIOD"},
    {GPIOE, "GPIOE"},
    {GPIOF, "GPIOF"},
    {GPIOG, "GPIOG"},
    {GPIOH, "GPIOH"},
    {GPIOI, "GPIOI"},
    {GPIOJ, "GPIOJ"},
    {GPIOK, "GPIOK"}
};

/* Function to check if GPIO port clock is enabled */
static GPIO_Bool_Type is_gpio_port_enabled(const GPIO_TypeDef* const port)
{
    GPIO_Bool_Type is_enabled = GPIO_FALSE;
    
    if (port == GPIOA) {
        is_enabled = (__HAL_RCC_GPIOA_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOB) {
        is_enabled = (__HAL_RCC_GPIOB_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOC) {
        is_enabled = (__HAL_RCC_GPIOC_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOD) {
        is_enabled = (__HAL_RCC_GPIOD_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOE) {
        is_enabled = (__HAL_RCC_GPIOE_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOF) {
        is_enabled = (__HAL_RCC_GPIOF_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOG) {
        is_enabled = (__HAL_RCC_GPIOG_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOH) {
        is_enabled = (__HAL_RCC_GPIOH_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOI) {
        is_enabled = (__HAL_RCC_GPIOI_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOJ) {
        is_enabled = (__HAL_RCC_GPIOJ_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else if (port == GPIOK) {
        is_enabled = (__HAL_RCC_GPIOK_IS_CLK_ENABLED() != 0U) ? GPIO_TRUE : GPIO_FALSE;
    }
    else {
        /* Port not recognized, keep is_enabled as GPIO_FALSE */
    }
    
    return is_enabled;
}

/* Function to get pin mode as a string */
static const char* get_pin_mode(const GPIO_TypeDef* const port, const uint8_t pin)
{
    const char* mode_str = "UNKNOWN";
    
    if ((pin < 16U) && (port != NULL)) {
        const uint32_t pin_mode = (port->MODER >> (pin * 2U)) & 0x3U;
        const uint32_t otyper  = (port->OTYPER >> pin) & 0x1U;
        const uint32_t alternate = (pin_mode == 0x2U) ? 1U : 0U;
        
        if (pin_mode == 0x0U) {
            mode_str = "INPUT";
        }
        else if ((pin_mode == 0x1U) && (otyper == 0U)) {
            mode_str = "OUTPUT_PP";
        }
        else if ((pin_mode == 0x1U) && (otyper != 0U)) {
            mode_str = "OUTPUT_OD";
        }
        else if ((alternate != 0U) && (otyper == 0U)) {
            mode_str = "AF_PP";
        }
        else if ((alternate != 0U) && (otyper != 0U)) {
            mode_str = "AF_OD";
        }
        else if (pin_mode == 0x3U) {
            mode_str = "ANALOG";
        }
        else {
            /* Keep as "UNKNOWN" */
        }
    }
    
    return mode_str;
}

/* Function to collect GPIO information */
static void collect_gpio_info(void)
{
    uint8_t enabled_ports = 0U;
    
    /* Clear existing data */
    (void)memset(&gpio_data, 0, sizeof(gpio_data));
    
    /* Iterate through all possible ports */
    for (uint8_t i = 0U; (i < 11U) && (enabled_ports < 11U); i++) {
        if (is_gpio_port_enabled(PORT_MAP[i].port) == GPIO_TRUE) {
            GPIO_Port_Info_Type* const current_port = &gpio_data.ports[enabled_ports];
            uint8_t configured_pins = 0U;
            
            current_port->port_name = PORT_MAP[i].name;
            current_port->port = PORT_MAP[i].port;
            
            /* Check all pins on this port */
            for (uint8_t pin = 0U; (pin < 16U) && (configured_pins < 16U); pin++) {
                const char* const mode = get_pin_mode(PORT_MAP[i].port, pin);
                
                /* We only store pins with a known mode */
                if (0U != strcmp(mode, "UNKNOWN")) {
                    current_port->pins[configured_pins].pin_number = pin;
                    /* FIX: store string pointer instead of enum */
                    current_port->pins[configured_pins].mode = mode;
                    configured_pins++;
                }
            }
            
            current_port->pin_count = configured_pins;
            enabled_ports++;
        }
    }
    
    gpio_data.total_enabled_ports = enabled_ports;
}

/* Function to display enabled pins */
void display_enabled_pins(void)
{
    char line[64U] = {0U};
    
    collect_gpio_info();
    
    /* Send header */
    serial_hal_svc_send("Enabled GPIO Ports and Configured Pins:\n");
    
    /* Display information for each enabled port */
    for (uint8_t i = 0U; i < gpio_data.total_enabled_ports; i++) {
        const GPIO_Port_Info_Type* const current_port = &gpio_data.ports[i];
        
        /* Send port name */
        (void)snprintf(line, sizeof(line), "%s:\n", current_port->port_name);
        serial_hal_svc_send(line);
        
        /* Send pin information */
        for (uint8_t j = 0U; j < current_port->pin_count; j++) {
            const GPIO_Pin_Info_Type* const current_pin = &current_port->pins[j];
            (void)snprintf(line, sizeof(line), "  Pin%u: %s\n", 
                           (unsigned int)current_pin->pin_number, 
                           current_pin->mode);
            serial_hal_svc_send(line);
        }
        
        /* Add spacing between ports */
        serial_hal_svc_send("\n");
    }
}
