#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> // 必须包含此头文件以使用 getch()
#include <ctype.h> // 必须包含此头文件以使用 isdigit()
#include <stdbool.h>
#include "common.h"
#include "Guess_num.h"

typedef enum{
    EASY,
    MEDUIUM,
    HARD,
    DIFFICULTY_COUNT
}GAME_DIFFICULTY;

/*   Variable define  */
char From_keyboard_buffer[20];
S8 s8g_random_number;
S8 s8g_input_num;
U1 u1g_zbq_flg = 0;
U1 u1g_create_random_flg = 1;
U1 u1g_gameover_flg = 1;


GAME_DIFFICULTY u1g_Difficulty_Mark = EASY;
U1 u1g_Keyboard_Up     = 0;
U1 u1g_Keyboard_Down   = 0;
U1 u1g_Keyboard_Left   = 0;
U1 u1g_Keyboard_Right  = 0;

/*   func define   */
void vdg_main_func(void);
void vdg_Get_CurrTime_Srand(void);
void vdg_Start_View(void);
void vdg_get_Random_num(void);
void vdg_Get_From_Keyboard(void);
void vdg_judge_Input_Buff(void);
U1 u1g_is_valid_integer(const char *str);
void vdg_judge_randAndInput(void);
void vdg_clearScreen(void);
void vdg_zbq(void);

/*******************************************************************/
void vdg_main_func(void)
{
    vdg_Get_CurrTime_Srand();

    vdg_clearScreen();
    
    while(1)
    {
        
        if(u1g_create_random_flg == 1)
        {
            vdg_Start_View();
            if(u1g_gameover_flg == 0)
            {
                break;
            }

            vdg_get_Random_num();

            u1g_create_random_flg = 0;
        }
        
        vdg_Get_From_Keyboard();
        if(u1g_gameover_flg == 0)
        {
            break;
        }

        vdg_judge_Input_Buff();

        vdg_zbq();

        vdg_judge_randAndInput();
                
        vdg_clearScreen();
    }
    
    return;
}

void vdg_Get_CurrTime_Srand(void)
{
    srand((U4)time(NULL)); // 使用当前时间作为种子

    return;
}

#if 1
void vdg_Start_View(void)
{
    char ch_Choose_symbol_point[] = ">  ";
    //char ch_Choose_symbol_space = " ";

    char ch_UpDwon_move;
    U1 i = 0;

    while(1)
    {
        printf("/======== SELECT DIFFICULTY ========/\n");
        printf("/*  %c easy(-100~100)               */\n",ch_Choose_symbol_point[0]);
        printf("/*  %c medium(-32767~32767)         */\n",ch_Choose_symbol_point[1]);
        printf("/*  %c hard(-2147450879~2147450879) */\n",ch_Choose_symbol_point[2]);
        printf("/===================================/\n");

        ch_UpDwon_move = getch();
        vdg_clearScreen();
    
        if( ch_UpDwon_move == 72)  //up  ↑
        {
            u1g_Keyboard_Up = 1;
        }
        else if(ch_UpDwon_move == 80)  //dwon   ↓
        {
            u1g_Keyboard_Down = 1;
        }
        else if(ch_UpDwon_move == 75)  //left    ←
        {
            u1g_Keyboard_Left = 1;
        }
        else if(ch_UpDwon_move == 77)  //right   →
        {
            u1g_Keyboard_Right = 1;
        }
        else if(ch_UpDwon_move == '\r')
        {
            break;
        }
        else if(ch_UpDwon_move == '\e')
        {
            u1g_gameover_flg = 0;
            break;
        }
        {
            /*do nothing*/
        }

        for(U1 j=0;j<3;j++)
        {
            ch_Choose_symbol_point[j] = ' ';
        }
        
        if(u1g_Keyboard_Up == 1)
        {
            if( (i>0) && (u1g_Difficulty_Mark > EASY) )
            {
                i--;
                u1g_Difficulty_Mark--;
            }
            else
            {
                i = 2;
                u1g_Difficulty_Mark = HARD;
            }
            ch_Choose_symbol_point[i] = '>';
            u1g_Keyboard_Up = 0;
        }
        else if(u1g_Keyboard_Down == 1)
        {
            if((i<2) && (u1g_Difficulty_Mark < HARD))
            {
                i++;
                u1g_Difficulty_Mark++;
            }
            else
            {
                i = 0;
                u1g_Difficulty_Mark = EASY;
            }
            ch_Choose_symbol_point[i] = '>';
            u1g_Keyboard_Down = 0;
        }
        else
        {

        }

    }
    vdg_clearScreen();

    return;
}


#endif

void vdg_get_Random_num(void)
{
    if (u1g_Difficulty_Mark == EASY) 
    {
        // 直接生成 0 ~ 100 的随机数，无需循环
        s8g_random_number = rand() % 101; 
    }
    else if (u1g_Difficulty_Mark == MEDUIUM) 
    {
        // 假设 MEDUIUM 范围是 -32767 ~ 32767
        s8g_random_number = rand(); 
    }
    else if (u1g_Difficulty_Mark == HARD) 
    {
        // 安全地拼接两个 15位 的随机数，生成 30位 的大随机数
        S4 high = (U4)(rand() & 0x7FFF) << 15;
        S4 low  = rand() & 0x7FFF;
        s8g_random_number = high | low;
    }
    else
    {

    }

    // 50% 的概率将正数变为负数
    // 注意：如果生成的数已经是 0，则无需取负
    if ((rand() % 2 == 0) && (s8g_random_number != 0)) 
    {
        s8g_random_number = -s8g_random_number;
    }

    return;
}

void vdg_Get_From_Keyboard(void)
{
    char ch_Input_char;
    U1 index = 0;

    printf("%d\n",u1g_Difficulty_Mark);

    if(u1g_Difficulty_Mark == EASY)
    {
        printf("输入一个数(范围-100~100):");
    }
    else if(u1g_Difficulty_Mark == MEDUIUM)
    {
        printf("输入一个数(范围-32767~32767):");
    }
    else if(u1g_Difficulty_Mark == HARD)
    {
        printf("输入一个数(范围-2147450879~2147450879):");
    }
    else
    {

    }

    memset(From_keyboard_buffer, 0, sizeof(From_keyboard_buffer));
    //u1g_gameover_flg = 1;

    while(1)
    {
        ch_Input_char = getch();

        if(ch_Input_char == '\r')
        {
            break;
        }

        if(ch_Input_char == '\e')
        {
            u1g_gameover_flg = 0;
            break;
        }

        if (ch_Input_char == 8 && index > 0)   //ASCII:8  = BACKSPACE
        {
            index--;
            printf("\b \b"); // 光标左移，打印空格覆盖，再左移
            continue;
        }

        if ( isdigit(ch_Input_char) || (ch_Input_char == '-') )
        {
            // 如果是数字，存入数组，并手动调用 putchar() 回显
            if (index < sizeof(From_keyboard_buffer) - 1)
            {
                From_keyboard_buffer[index++] = ch_Input_char;
                putchar(ch_Input_char); // 手动打印到屏幕上
            }
            
        }
        else if ((ch_Input_char == '*') || (ch_Input_char == '/') || (ch_Input_char == '+'))
        {
            u1g_zbq_flg++;
        }
        else if ( ( (ch_Input_char <= 'z') && (ch_Input_char >= 'a') ) || (ch_Input_char == ' ') )
        {
            if (index < sizeof(From_keyboard_buffer) - 1)
            {
                From_keyboard_buffer[index++] = ch_Input_char;
                putchar(ch_Input_char); // 手动打印到屏幕上
            }
        }
        else
        {

        }
    }
    
    From_keyboard_buffer[index] = '\0';
    printf("\n");

    return;
}

void vdg_judge_Input_Buff(void)
{
    char ch_Giveup_Game[] = "give up"; 
    if (u1g_is_valid_integer(From_keyboard_buffer)) 
    {
        s8g_input_num = atoi(From_keyboard_buffer); // 确认是整数后，再安全地转换
    }
    else if ( (From_keyboard_buffer[0] == '\0') && (u1g_zbq_flg != 2) )
    {
        printf("\x1B[41m 输入错误！ 没有输入数字 \x1B[0m\n");
    }
    else if ( strcmp(From_keyboard_buffer, ch_Giveup_Game) == 0 )
    {
        s8g_input_num = s8g_random_number;
    }
    else
    {
        printf("\x1B[41m 输入错误！'%s' 不是一个合法的整数 \x1B[0m\n", From_keyboard_buffer);
    }

    return ;
}

U1 u1g_is_valid_integer(const char *str)
{
    if (str == NULL || strlen(str) == 0)
    {
        return false;
    }
    char *endptr;
    S4 val = strtol(str, &endptr, 10);  //0:auto  10:10进制  16:16进制

    if (endptr == str || *endptr != '\0' )
    {
        return false;
    }

    if (val < INT_MIN || val > INT_MAX)
    {
        return false;
    }

    return true;
}

void vdg_judge_randAndInput(void)
{
    if(s8g_random_number == s8g_input_num)
    {
        printf("\x1B[42mrandom_number = %d\x1B[0m\n",s8g_random_number);
        u1g_create_random_flg = 1;
    }
    else if ( s8g_random_number < s8g_input_num )
    {
        printf("\x1B[41m 你输入的数大了 \x1B[0m\n");
    }
    else
    {
        printf("\x1B[41m 你输入的数小了 \x1B[0m\n");
    }

    printf("按任意键继续...");
    _getch();
    
    return;
}

void vdg_clearScreen(void)
{
    printf("\033[2J"); // 清除整个屏幕
    printf("\033[H");  // 将光标移动到屏幕左上角
    fflush(stdout);    // 确保立即输出
}

void vdg_zbq(void)
{
    if(u1g_zbq_flg == 2 )
    {
        s8g_random_number = s8g_input_num;

        u1g_zbq_flg = 0;
    }

    return;
}

