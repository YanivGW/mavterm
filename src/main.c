#include <drivers.h>
#include <proj_assert.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_MAVLINK_MSG_SIZE_BYTES 256
uint8_t buf[MAX_MAVLINK_MSG_SIZE_BYTES] = {0};

static void uart_cb(uint32_t event)
{
    proj_assert(event == ARM_USART_EVENT_RECEIVE_COMPLETE);
    printf("UART callback\n");
}

int main()
{
    printf("C is so fun!\n");

    int32_t res = UART->Initialize(uart_cb);
    proj_assert(res == ARM_DRIVER_OK);

    size_t idx = 0;
    while (res == ARM_DRIVER_OK && idx < sizeof(buf))
    {
        res = UART->Receive(&buf[idx++], 1);
    }

    printf("Receive complete: %s\n", buf);

    return 0;
}