/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/wukun/task\checknum.cc
#    Author       : Sunsolo
#    Email        : wukun@kunyan-inc.com
#    Date         : 2015-12-09 16:21
#    Description  : 
=============================================================================*/

#include "check.h"
#include <string.h>
extern "C"{

bool checksum(const char* src_data, unsigned short&  check_ret) {
    bool ret = true;

    do {
        const int16_t* opt_data = reinterpret_cast<const int16_t*>(src_data);
        if (opt_data == NULL || src_data == NULL) {
            ret = false;
            /*TODO*/
            // warn_log();
            break;
        }
        int32_t accu_sum = 0;
        int32_t data_len = strlen(src_data);

        while (data_len > 1) {
            accu_sum += *(opt_data);
            opt_data++;
            data_len = data_len - 2;

            if (accu_sum & 0x80000000) {
                accu_sum = (accu_sum >> 16) + (accu_sum & 0xFFFF);
            }
        }

        if (data_len == 1) {
            accu_sum += *(reinterpret_cast<const int8_t*>(opt_data));
        }

        while (accu_sum >> 16) {
            accu_sum = (accu_sum >> 16) + (accu_sum & 0xFFFF);
        }
        check_ret = 0xFFFF & accu_sum;
        
//        check_ret = (accu_sum == 0xFFFF)?~accu_sum:accu_sum;

//        check_ret = (accu_sum == 0xFFFF)?accu_sum:~accu_sum;
    }while(0);

    return ret;
}
}





