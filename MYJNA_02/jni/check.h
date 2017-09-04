/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/wukun/task\checknum.hpp
#    Author       : Sunsolo
#    Email        : wukun@kunyan-inc.com
#    Date         : 2015-12-09 16:28
#    Description  : 
=============================================================================*/


#ifndef TASK_CHECKSUM_H
#define TASK_CHECKSUM_H

typedef signed char int8_t;
typedef short int  int16_t;
typedef int        int32_t;


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis          checksum加密算法
 *
 * @Param src_data    被加密的字符串 
 *
 * @Param check_ret   int16_t类型的加密结果
 *
 * @Returns           加密是否成功
 */
/* ----------------------------------------------------------------------------*/

extern "C" bool checksum(const char* src_data, unsigned short& check_ret);

#endif
