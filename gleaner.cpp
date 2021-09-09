#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

/* Original */
// http://www.cplusplus.com/forum/general/117719/

struct walk {
    short walk_count;
    short x;
    short y;
    short back;
};

struct target {
    short x;
    short y;
};

std::vector<walk> BFSArray;
std::vector<target> walk_queue;
std::vector<std::string> tmp_map(18);

std::vector<std::string> map = {
    "+#############################+",
    "|                             |",
    "|                             |",
    "|## ########### ##   #########|",
    "|   |                         |",
    "| | |### |  |           |     |",
    "| |      |  | |###  |   |  |  |",
    "| | #####|  | |      ## |     |",
    "| |           |###  |      |  |",
    "| |##### ###         ##       |",
    "|          ######  ####### ###|",
    "|                             |",
    "|# ### ####      ###   #######|",
    "|                             |",
    "|                             |",
    "|                             |",
    "|                             |",
    "+#############################+"
};

void ShowMap() {
    for(const auto& line : map)
        std::cout << line << std::endl;
}

void gotoxy(short x, short y) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};

    SetConsoleCursorPosition(hStdout, position);
}

void AddArray(int x, int y, int wc , int back) {
    if(tmp_map[y][x] == ' ' || tmp_map[y][x] == '.') {
        tmp_map[y][x] = '#';
        walk tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.walk_count = wc;
        tmp.back = back;
        BFSArray.push_back(tmp);
    }
}

void FindPath(int sx, int sy, int x, int y) {
    std::copy(map.begin(), map.end(), tmp_map.begin());
    BFSArray.clear();
    walk tmp;
    tmp.x = sx;
    tmp.y = sy;
    tmp.walk_count = 0;
    tmp.back = -1;
    BFSArray.push_back(tmp);

    for(int i=0; i<BFSArray.size(); i++){
        if(BFSArray[i].x == x && BFSArray[i].y == y) {
            walk_queue.clear();
            target tmp2;
            while(BFSArray[i].walk_count != 0) {
                tmp2.x = BFSArray[i].x;
                tmp2.y = BFSArray[i].y;
                walk_queue.push_back(tmp2);

                i = BFSArray[i].back;
            }

            break;
        }

        AddArray(BFSArray[i].x+1, BFSArray[i].y, BFSArray[i].walk_count+1, i);
        AddArray(BFSArray[i].x-1, BFSArray[i].y, BFSArray[i].walk_count+1, i);
        AddArray(BFSArray[i].x, BFSArray[i].y+1, BFSArray[i].walk_count+1, i);
        AddArray(BFSArray[i].x, BFSArray[i].y-1, BFSArray[i].walk_count+1, i);
    }

    BFSArray.clear();
}

void Run() {
    int old_x;
    int old_y;

    int x = 15; // me x
    int y = 16; // me y

    int ex = 1; // enemy x
    int ey = 1; // enemy y

    int pts = 0;
    int frame = 0;
    int speedmod = 3;

    system("cls");
    ShowMap();
    gotoxy(x, y);
    std::cout << "M";

    gotoxy(0, 19);
    std::cout << "Press any key to start." << std::endl;
    system("pause > nul");

    std::cout << "\033[1A\033[2K" << std::endl;

    FindPath(ex, ey, x, y);

    while(ex != x || ey != y) {
        if(pts >= 50 && pts < 100)
            speedmod = 2;
        else if(pts >= 100)
            speedmod = 1;

        gotoxy(x, y);
        std::cout << " ";

        old_x = x;
        old_y = y;

        if(GetAsyncKeyState(VK_UP)) {
            if(map[y-1][x] == '.') {
                map[y-1][x] = ' ';
                pts++;
                y--;
            }
            else if(map[y-1][x] == ' ') y--;
        }
        if(GetAsyncKeyState(VK_DOWN)) {
            if(map[y+1][x] == '.') {
                map[y+1][x] = ' ';
                pts++;
                y++;
            }
            else if(map[y+1][x] == ' ') y++;
        }
        if(GetAsyncKeyState(VK_LEFT)) {
            if(map[y][x-1] == '.') {
                map[y][x-1] = ' ';
                pts++;
                x--;
            }
            else if(map[y][x-1] == ' ') x--;
        }
        if(GetAsyncKeyState(VK_RIGHT)) {
            if(map[y][x+1] == '.') {
                map[y][x+1] = ' ';
                pts++;
                x++;
            }
            else if(map[y][x+1] == ' ') x++;
        }

        if(old_x != x || old_y != y)
            FindPath(ex, ey, x, y);

        gotoxy(x, y);
        std::cout << "M";

        map[ey][ex] = '.';
        gotoxy(ex, ey);
        std::cout << ".";

        if(frame%speedmod == 0 && walk_queue.size() != 0) {
            ex = walk_queue.back().x;
            ey = walk_queue.back().y;
            walk_queue.pop_back();
        }

        gotoxy(ex, ey);
        std::cout << "E";

        gotoxy(32, 1);
        std::cout << pts;
        Sleep(100);
        frame++;
    }

    gotoxy(32, 1);
    std::cout << "\033[0K";

    gotoxy(0, 19);
    std::cout << "GAME OVER!!" << std::endl;
    std::cout << "Your score is " << pts << "!" << std::endl;
}

int main() {
    Run();
    return 0;
}