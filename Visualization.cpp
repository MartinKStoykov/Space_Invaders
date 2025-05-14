#include <Windows.h>
#include "Visualization.h"


HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color) {
    CHAR_INFO ch_info;
    ch_info.Char.AsciiChar = ch;
    ch_info.Attributes = foreground_color | (background_color << 4);

    COORD buf_size = { 1, 1 };
    COORD buf_coord = { 0, 0 };
    SMALL_RECT screen_pos = { x, y, x + 1, y + 1 };
    WriteConsoleOutput(hConsoleOutput, &ch_info, buf_size, buf_coord, &screen_pos);
}