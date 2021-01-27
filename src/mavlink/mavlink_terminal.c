/* Includes */
#include "mavlink_terminal.h"
#include <proj_assert.h>

/* Prototypes */
static void clear_resources(mavlink_terminal_t *p_this);

/* Public API */
void mavlink_terminal_initialize(mavlink_terminal_t *p_this, mavlink_terminal_cb_t cb, void *arg)
{
    proj_assert(p_this->state == MAVLINK_TERMINAL_STATE_UNINITIALIZED);

    int32_t res = p_this->p_conf->p_uart->Initialize(p_this->p_conf->uart_cb);
    proj_assert(res == ARM_DRIVER_OK);

    clear_resources(p_this);

    p_this->cb = cb;
    p_this->arg = arg;

    p_this->state = MAVLINK_TERMINAL_STATE_IDLE;
}

void mavlink_terminal_listen(mavlink_terminal_t *p_this)
{
    proj_assert(p_this->state == MAVLINK_TERMINAL_STATE_IDLE);

    int32_t res = p_this->p_conf->p_uart->PowerControl(ARM_POWER_FULL);
    proj_assert(res == ARM_DRIVER_OK);

    p_this->state = MAVLINK_TERMINAL_STATE_LISTENING;

    /* Trigger the first receive */
    res = p_this->p_conf->p_uart->Receive(&p_this->recv_byte, 1);
    proj_assert(res == ARM_DRIVER_OK);
}

/* Private API */
void _mavlink_terminal_uart_cb(mavlink_terminal_t *p_this, uint32_t event)
{
    proj_assert(event == ARM_USART_EVENT_RECEIVE_COMPLETE);

    uint8_t parse_res = mavlink_parse_char(p_this->p_conf->channel, p_this->recv_byte, &p_this->message, &p_this->status);
    switch (parse_res)
    {
    case MAVLINK_FRAMING_INCOMPLETE:
    {
        break;
    }
    case MAVLINK_FRAMING_BAD_CRC:
    {
        clear_resources(p_this);
        break;
    }
    case MAVLINK_FRAMING_OK:
    {
        // osMessageQueuePut(p_this->mq, &p_this->message);
        // invoke_thread(callback_caller);
        p_this->cb(&p_this->message, p_this->arg); // TODO: temporary
        clear_resources(p_this);
        break;
    }
    default:
    {
        proj_assert(false);
        break;
    }
    }

    int32_t res = p_this->p_conf->p_uart->Receive(&p_this->recv_byte, 1);
    proj_assert(res == ARM_DRIVER_OK);
}

/* Private functions */
static void clear_resources(mavlink_terminal_t *p_this)
{
    memset(&p_this->status, 0, sizeof(p_this->status));
    memset(&p_this->message, 0, sizeof(p_this->message));
    p_this->recv_byte = 0;
}