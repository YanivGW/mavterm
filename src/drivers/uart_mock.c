#include <Driver_USART.h>
#include <proj_assert.h>
#include <stdint.h>
#include <stdio.h>

static int32_t initialize(ARM_USART_SignalEvent_t cb_event);
static int32_t receive(void *data, uint32_t num);

ARM_DRIVER_USART uart_mock = {
    .Initialize = initialize,
    .Receive = receive,
};

typedef struct
{
    ARM_USART_SignalEvent_t cb;
    char *filename;
    FILE *fptr;
} resources_t;

static resources_t resources = {
    .filename = "data/uart_input.bin",
};

static int32_t initialize(ARM_USART_SignalEvent_t cb_event)
{
    resources_t *this = &resources;

    this->cb = cb_event;

    this->fptr = fopen(this->filename, "r");
    proj_assert(this->fptr != NULL);

    return ARM_DRIVER_OK;
}

static int32_t receive(void *data, uint32_t num)
{
    resources_t *this = &resources;

    size_t count = fread(data, 1, num, this->fptr);
    if (count < num)
    {
        fclose(this->fptr);
        return ARM_DRIVER_ERROR;
    }

    this->cb(ARM_USART_EVENT_RECEIVE_COMPLETE);
    return ARM_DRIVER_OK;
}
