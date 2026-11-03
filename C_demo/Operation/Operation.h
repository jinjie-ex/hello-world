#ifndef OPERATION
#define OPERATION

#include "common.h"

/* macro define */
#define U1G_KEYBOARD_UP      (U1)72
#define U1G_KEYBOARD_DWON    (U1)80
#define U1G_KEYBOARD_LEFT    (U1)75
#define U1G_KEYBOARD_RIGHT   (U1)77


/* struct define */
typedef struct
{
    U1 u1_up;
    U1 u1_dwon;
    U1 u1_left;
    U1 u1_right;
    //U1 u1_up_left;
    //U1 u1_up_right;
    //U1 u1_dwon_left;
    //U1 u1_dwon_right;
}Direction;

#endif