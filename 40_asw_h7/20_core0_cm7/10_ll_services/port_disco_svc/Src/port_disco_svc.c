// port_disco_svc.c

#include "port_disco_svc.h"
#include "svo_svc.h"        // For serial_hal_svc_send
#include "stm32h7xx_hal.h"  // STM32 HAL library
#include <string.h>
#include <stdio.h>

// Function to check if a GPIO port's clock is enabled
static int is_gpio_port_enabled(GPIO_TypeDef* port)
{
    if (port == GPIOA)
        return __HAL_RCC_GPIOA_IS_CLK_ENABLED();
    if (port == GPIOB)
        return __HAL_RCC_GPIOB_IS_CLK_ENABLED();
    if (port == GPIOC)
        return __HAL_RCC_GPIOC_IS_CLK_ENABLED();
    if (port == GPIOD)
        return __HAL_RCC_GPIOD_IS_CLK_ENABLED();
    if (port == GPIOE)
        return __HAL_RCC_GPIOE_IS_CLK_ENABLED();
    if (port == GPIOF)
        return __HAL_RCC_GPIOF_IS_CLK_ENABLED();
    if (port == GPIOG)
        return __HAL_RCC_GPIOG_IS_CLK_ENABLED();
    if (port == GPIOH)
        return __HAL_RCC_GPIOH_IS_CLK_ENABLED();
    if (port == GPIOI)
        return __HAL_RCC_GPIOI_IS_CLK_ENABLED();
    if (port == GPIOJ)
        return __HAL_RCC_GPIOJ_IS_CLK_ENABLED();
    if (port == GPIOK)
        return __HAL_RCC_GPIOK_IS_CLK_ENABLED();
    
    // Add additional ports if necessary
    return 0;
}

// Function to get the mode of a specific pin as a string
static const char* get_pin_mode(GPIO_TypeDef* port, int pin)
{
    uint32_t pin_mode = (port->MODER >> (pin * 2)) & 0x3;

    switch(pin_mode)
    {
        case GPIO_MODE_INPUT:
            return "INPUT";
        case GPIO_MODE_OUTPUT_PP:
            return "OUTPUT_PP";
        case GPIO_MODE_OUTPUT_OD:
            return "OUTPUT_OD";
        case GPIO_MODE_AF_PP:
            return "AF_PP";
        case GPIO_MODE_AF_OD:
            return "AF_OD";
        case GPIO_MODE_ANALOG:
            return "ANALOG";
        default:
            return "UNKNOWN";
    }
}

// Function to display enabled GPIO ports and their configured pins with functions
void display_enabled_pins(void)
{
    // Buffer to accumulate the list of enabled ports and their pins
    char buffer[4096]; // Increased buffer size to accommodate detailed output
    size_t offset = 0;

    // Start the message with a header
    const char* header = "Enabled GPIO Ports and Configured Pins:\n";
    strncpy(buffer, header, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null-termination
    offset += strlen(header);

    // Array of GPIO port base addresses and their names
    typedef struct {
        GPIO_TypeDef* port;
        const char* name;
    } GPIO_Port;

    GPIO_Port gpio_ports[] = {
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
        {GPIOK, "GPIOK"},
    };

    const int num_ports = sizeof(gpio_ports) / sizeof(GPIO_Port);

    // Iterate through each GPIO port
    for(int i = 0; i < num_ports; i++)
    {
        GPIO_TypeDef* port = gpio_ports[i].port;
        const char* port_name = gpio_ports[i].name;

        // Check if the GPIO port's clock is enabled
        if(is_gpio_port_enabled(port))
        {
            // Append the port name
            int written = snprintf(buffer + offset, sizeof(buffer) - offset, "%s:\n", port_name);
            if(written < 0 || (size_t)written >= sizeof(buffer) - offset)
            {
                // If buffer is full or an error occurred, send the current buffer and reset
                serial_hal_svc_send(buffer);
                offset = 0;
                buffer[0] = '\0';
                // Retry adding the current port after reset
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%s:\n", port_name);
                if(written > 0 && (size_t)written < sizeof(buffer) - offset)
                {
                    offset += written;
                }
                else
                {
                    // If still unable to write, skip to next port
                    continue;
                }
            }
            else
            {
                offset += written;
            }

            // Iterate through each pin (0-15) in the port to check its configuration
            for(int pin = 0; pin < 16; pin++)
            {
                const char* mode = get_pin_mode(port, pin);

                // Define criteria for "configured" pins
                // Here, all modes except "INPUT", "ANALOG", and "UNKNOWN" are considered configured
                // Adjust this condition based on your specific requirements
                if(strcmp(mode, "UNKNOWN") != 0)
                {
                    // Append the pin information on a new line with indentation
                    written = snprintf(buffer + offset, sizeof(buffer) - offset, "  Pin%d: %s\n", pin, mode);
                    if(written < 0 || (size_t)written >= sizeof(buffer) - offset)
                    {
                        // If buffer is full or an error occurred, send the current buffer and reset
                        serial_hal_svc_send(buffer);
                        offset = 0;
                        buffer[0] = '\0';
                        // Retry adding the current pin after reset
                        written = snprintf(buffer + offset, sizeof(buffer) - offset, "  Pin%d: %s\n", pin, mode);
                        if(written > 0 && (size_t)written < sizeof(buffer) - offset)
                        {
                            offset += written;
                        }
                        else
                        {
                            // If still unable to write, skip to next pin
                            continue;
                        }
                    }
                    else
                    {
                        offset += written;
                    }
                }
            }

            // Add an empty line for better readability between ports
            written = snprintf(buffer + offset, sizeof(buffer) - offset, "\n");
            if(written > 0 && (size_t)written < sizeof(buffer) - offset)
            {
                offset += written;
            }
        }
    }

    // Ensure the buffer is null-terminated before sending
    buffer[sizeof(buffer) - 1] = '\0';

    // Send the accumulated buffer
    serial_hal_svc_send(buffer);
}
