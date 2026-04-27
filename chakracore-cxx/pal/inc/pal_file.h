//
// Created by Han Hossain on 3/30/26.
//

#ifndef CHAKRA_RS_PAL_FILE_H
#define CHAKRA_RS_PAL_FILE_H

#include <stdint.h>

typedef int BOOL;
struct __sFILE;
#ifdef __APPLE__
typedef struct __sFILE FILE;
#else
typedef struct _IO_FILE FILE;
#endif

#endif //CHAKRA_RS_PAL_FILE_H
