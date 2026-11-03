#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

/* 宏定义 - 遵循 MISRA C 规范，使用大写字母和下划线 */
#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define MAX_SNAKE_SIZE (MAP_WIDTH * MAP_HEIGHT)
#define SNAKE_CHAR 'O'
#define FOOD_CHAR '*'
#define EMPTY_CHAR ' '

/* 坐标结构体 */
typedef struct {
    int x;
    int y;
} Point;

/* 游戏状态变量 - 显式初始化 */
static Point snake[MAX_SNAKE_SIZE];
static int snake_length = 0;
static Point food;
static int score = 0;
static char direction = 'd'; /* 'w':上, 's':下, 'a':左, 'd':右 */

/* 函数声明 */
static void gotoxy(int x, int y);
static void init_game(void);
static void generate_food(void);
static void update_snake(void);
static void draw_map(void);

/* 光标定位函数 (Windows API) */
static void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* 初始化游戏 */
static void init_game(void) {
    snake_length = 3;
    /* 初始化蛇的位置 */
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = 5 - i;
        snake[i].y = 10;
    }
    score = 0;
    direction = 'd';
    generate_food();
}

/* 生成食物 */
static void generate_food(void) {
    int is_on_snake = 1;
    while (is_on_snake) {
        is_on_snake = 0;
        /* 使用 rand() 生成随机坐标 */
        food.x = (int)(rand() % (MAP_WIDTH - 2)) + 1;
        food.y = (int)(rand() % (MAP_HEIGHT - 2)) + 1;

        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                is_on_snake = 1;
                break;
            }
        }
    }
}

/* 更新蛇的逻辑 */
static void update_snake(void) {
    /* 计算新蛇头的位置 */
    Point new_head = snake[0];
    switch (direction) {
        case 'w': new_head.y--; break;
        case 's': new_head.y++; break;
        case 'a': new_head.x--; break;
        case 'd': new_head.x++; break;
        default: break;
    }

    /* 1. 撞墙检测：游戏结束 */
    if (new_head.x <= 0 || new_head.x >= MAP_WIDTH - 1 || 
        new_head.y <= 0 || new_head.y >= MAP_HEIGHT - 1) {
        gotoxy(MAP_WIDTH / 2 - 5, MAP_HEIGHT / 2);
        printf("GAME OVER! Score: %d", score);
        gotoxy(0, MAP_HEIGHT + 1);
        exit(0);
    }

    /* 2. 撞自己检测：不结束游戏，而是“吃掉自己” (截断身体) */
    int self_eat_index = -1;
    for (int i = 0; i < snake_length - 1; i++) {
        if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
            self_eat_index = i;
            break;
        }
    }

    if (self_eat_index != -1) {
        /* 吃到自己：分数增加，身体在撞击点处截断 */
        score += 10;
        snake_length = self_eat_index; 
        /* 注意：这里不执行常规的移动逻辑，直接重新生成新蛇头并结束本次更新 */
        snake[snake_length] = new_head;
        snake_length++;
        return;
    }

    /* 3. 吃食物检测 */
    if (new_head.x == food.x && new_head.y == food.y) {
        score += 10;
        /* 吃到食物：身体增长（不移除尾部），生成新食物 */
        for (int i = snake_length; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = new_head;
        snake_length++;
        generate_food();
    } else {
        /* 4. 正常移动：移除尾部，插入新头部 */
        for (int i = snake_length - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = new_head;
    }
}

/* 绘制地图 */
static void draw_map(void) {
    gotoxy(0, 0);
    /* 绘制上下边界 */
    for (int x = 0; x < MAP_WIDTH; x++) {
        gotoxy(x, 0);
        printf("#");
        gotoxy(x, MAP_HEIGHT - 1);
        printf("#");
    }
    /* 绘制左右边界 */
    for (int y = 0; y < MAP_HEIGHT; y++) {
        gotoxy(0, y);
        printf("#");
        gotoxy(MAP_WIDTH - 1, y);
        printf("#");
    }

    /* 绘制食物 */
    gotoxy(food.x, food.y);
    printf("%c", FOOD_CHAR);

    /* 绘制蛇 */
    for (int i = 0; i < snake_length; i++) {
        gotoxy(snake[i].x, snake[i].y);
        printf("%c", SNAKE_CHAR);
    }
    
    /* 绘制分数 */
    gotoxy(MAP_WIDTH + 2, 2);
    printf("Score: %d", score);
    gotoxy(MAP_WIDTH + 2, 4);
    printf("Controls:");
    gotoxy(MAP_WIDTH + 2, 5);
    printf("W: Up");
    gotoxy(MAP_WIDTH + 2, 6);
    printf("S: Down");
    gotoxy(MAP_WIDTH + 2, 7);
    printf("A: Left");
    gotoxy(MAP_WIDTH + 2, 8);
    printf("D: Right");
}

int main(void) {
    /* 隐藏光标 */
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hOutput, &cursor_info);
    cursor_info.bVisible = 0;
    SetConsoleCursorInfo(hOutput, &cursor_info);

    init_game();

    /* 游戏主循环 */
    while (1) {
        /* 非阻塞键盘检测 */
        if (_kbhit()) {
            char input = (char)_getch();
            /* 简单的反向移动防止逻辑 */
            if ((input == 'w' && direction != 's') ||
                (input == 's' && direction != 'w') ||
                (input == 'a' && direction != 'd') ||
                (input == 'd' && direction != 'a')) {
                direction = input;
            }
        }

        update_snake();
        draw_map();
        
        /* 控制游戏速度 (毫秒) */
        Sleep(150);
    }

    return 0;
}