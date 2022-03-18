/**
 * \file itoa.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-10-31
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef ITOA_H
#define ITOA_H

#ifdef __cplusplus
extern "C" {
#endif

#define uitoa _uitoa
#define itoa _itoa

void _uitoa(unsigned int value, char* string, unsigned char radix);
void _itoa(int value, char* string, unsigned char radix);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ITOA_H
