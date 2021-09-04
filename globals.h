#pragma once
#include "main_entity.h"

namespace game
{
    struct in_use
    {
    
        Vector real_angle{};
        int m_shift = 0;
        bool send_packet;
        bool fake_lagging;
        bool need_to_update;
        bool in_trace = false;

        CUserCmd* command;
        CUserCmd * get_command()
        {
            return command;
        }
        
        c_base_entity* m_local;
        c_base_entity* get_local()
        {
            return m_local;
        }   


    };

    extern in_use main;
}

