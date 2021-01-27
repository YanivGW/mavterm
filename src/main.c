#include <drivers.h>
#include <mavlink_terminal.h>
#include <proj_assert.h>
#include <project_messages.h>
#include <stdint.h>
#include <stdio.h>

MAVLINK_TERMINAL_GENERATE_OBJECT(terminal, UART);

void terminal_cb(mavlink_message_t *p_msg, void *arg)
{
    printf("MAVLink message received! Len = %d\n", p_msg->len);
    switch (p_msg->msgid)
    {
    // Option #1 - Parsing whole command into struct
    case MAVLINK_MSG_ID_COMMAND_INT:
    {
        mavlink_command_int_t cmd;
        mavlink_msg_command_int_decode(p_msg, &cmd);
        switch (cmd.command)
        {
        case MAV_CMD_DO_GRIPPER:
        {
            uint32_t instance = (uint32_t)cmd.param1;
            GRIPPER_ACTIONS action = (GRIPPER_ACTIONS)cmd.param2;
            // do whatever needed..
            break;
        }
        default:
        {
            break;
        }
        }
    }
    // Option #2 - Parsing field by field
    case MAVLINK_MSG_ID_COMMAND_LONG:
    {
        switch (mavlink_msg_command_long_get_command(p_msg))
        {
        case MAV_CMD_DO_GRIPPER:
        {
            uint32_t instance = (uint32_t)mavlink_msg_command_long_get_param1(p_msg);
            GRIPPER_ACTIONS action = (GRIPPER_ACTIONS)mavlink_msg_command_long_get_param2(p_msg);
            // do whatever needed..
            break;
        }
        default:
        {
            break;
        }
        }
    }
    default:
    {
        break;
    }
    }
}

int main()
{
    mavlink_terminal_initialize(&terminal, terminal_cb, NULL);
    mavlink_terminal_listen(&terminal);

    return 0;
}