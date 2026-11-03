#include <stdio.h>
#include <windows.h>

void setConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 1. 设置控制台缓冲区大小
    COORD bufferSize = {width, height};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    // 2. 设置控制台窗口大小
    // 注意：窗口大小的坐标是包含边界的，所以要减 1
    SMALL_RECT windowSize = {0, 0, width - 1, height - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

int Windows_Ctrl_main() {
    setConsoleSize(80, 30); // 将窗口设置为 80列 x 30行
    printf("窗口大小已调整！\n");
    return 0;
}