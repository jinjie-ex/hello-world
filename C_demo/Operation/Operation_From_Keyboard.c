#include <stdio.h>
#include "common.h"
#include "Operation.h"

Direction Str_Direction;

void Init(void)
{
    memset(&Str_Direction,0,sizeof(Str_Direction));

    return;
}

void vdg_OperatFromKeyboard(void)
{
    char ch_Key;
    while(1)
    {
        if (_kbhit()) 
        {
            ch_Key = getch();
            
            switch(ch_Key)
            {
                case U1G_KEYBOARD_UP:
                    Str_Direction.u1_up = true;
                break;

                case U1G_KEYBOARD_DWON:
                    Str_Direction.u1_dwon = true;
                break;

                case U1G_KEYBOARD_LEFT:
                    Str_Direction.u1_left = true;
                break;

                case U1G_KEYBOARD_RIGHT:
                    Str_Direction.u1_right = true;
                break;

                default:
                    break;
            }
            
        }
        else
        {

        }
    }

    return;
}