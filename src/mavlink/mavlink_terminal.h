/* Includes */
#include <Driver_USART.h>
#include <mavlink.h>

/* Macros */
#define MAVLINK_TERMINAL_GENERATE_OBJECT(name_, p_uart_)   \
    extern mavlink_terminal_t name_;                       \
                                                           \
    static void name_##uart_cb(uint32_t event)             \
    {                                                      \
        _mavlink_terminal_uart_cb(&name_, event);          \
    }                                                      \
                                                           \
    static mavlink_terminal_conf_t name_##_conf = {        \
        .p_uart = p_uart_,                                 \
        .uart_cb = name_##uart_cb,                         \
        .channel = MAVLINK_COMM_0,                         \
    };                                                     \
                                                           \
    mavlink_terminal_t name_ =                             \
        {                                                  \
            .p_conf = &name_##_conf,                       \
            .state = MAVLINK_TERMINAL_STATE_UNINITIALIZED, \
    }

/* Typedefs */
typedef void (*mavlink_terminal_cb_t)(mavlink_message_t *p_msg, void *arg);

typedef enum
{
    MAVLINK_TERMINAL_STATE_UNINITIALIZED,
    MAVLINK_TERMINAL_STATE_IDLE,
    MAVLINK_TERMINAL_STATE_LISTENING,
} mavlink_terminal_state_e;

typedef struct
{
    ARM_DRIVER_USART *p_uart;
    ARM_USART_SignalEvent_t uart_cb;
    uint8_t channel;
} const mavlink_terminal_conf_t;

typedef struct
{
    mavlink_terminal_conf_t *const p_conf;
    mavlink_terminal_cb_t cb;
    void *arg;
    mavlink_status_t status;
    mavlink_message_t message;
    uint8_t recv_byte;
    mavlink_terminal_state_e state;
} mavlink_terminal_t;

/* Public API */
void mavlink_terminal_initialize(mavlink_terminal_t *p_this, mavlink_terminal_cb_t cb, void *arg);
void mavlink_terminal_listen(mavlink_terminal_t *p_this);

/* Private API */
void _mavlink_terminal_uart_cb(mavlink_terminal_t *p_this, uint32_t event);
