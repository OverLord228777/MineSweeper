#ifndef FUNC_H
#define FUNC_H

#include <string>
#include <iostream>
#include <vector>
#include <conio.h>

using std::string;
using std::cout;

enum easy {
    e_mine_count = 5,
    e_board_size = 5
};

enum norm {
    n_mine_count = 10,
    n_board_size = 10
};

enum hard {
    h_mine_count = 15,
    h_board_size = 15
};

enum class Key {    // Клавиши
    Up,
    Down,
    Left,
    Right,
    Space,
    Enter,
    Escape,
    Unknown
};

enum class Tab {
    None,
    Exit,
    Play,
    LeaderBoard,
    Back,
    Easy,
    Normal,
    Hard,
    Clear
};

struct Point {
    string row;
    string col;
};

class Game {
private:
    std::vector<std::vector<char>> board;
    unsigned board_size;
    unsigned mine_count;
    unsigned opened_count;
    bool game_over;

    void place_mines(unsigned mine_count, unsigned board_size) {
        for (unsigned int i = 0; i < board_size; ++i) {
            for (unsigned int j = 0; j < board_size; ++j) {
                board[i][j] = '#';
            }
        }

        unsigned int mine_placed = 0, row, col;
        while (mine_placed < mine_count) {
            row = rand() % board_size;
            col = rand() % board_size;
            if (board[row][col] != '*') {
                board[row][col] = '*';
                mine_placed++;
            }
        }
    }


    int count_mines(int row, int col) const {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int r = row + i, c = col + j;
                if (r >= 0 && r < board_size && c >= 0 && c < board_size && board[r][c] == '*')
                    count++;
            }
        }
        return count;
    }

    void reveal_mines() {
        for (int i = 0; i < board_size; i++)
            for (int j = 0; j < board_size; j++)
                if (board[i][j] == '*') board[i][j] = 'X';
    }

public:
    Game(unsigned size, unsigned mines) : board_size(size), mine_count(mines), opened_count(0), game_over(false) {
        board.resize(size, std::vector<char>(size, '#'));
        place_mines(mine_count, board_size);
        cout << "Mines are placed\n";
    }

    bool open_cell(int row, int col);
    void show_board(unsigned board_size, bool show_mines = false);
    Point find_unit(unsigned board_size);
    bool is_game_over() const { return game_over; }
    bool is_win() const { return opened_count == (board_size * board_size - mine_count); }
};

class GameInput {
public:
    GameInput() {}

    Key GetInput() {
        int ch = _getch();
        switch (ch) {
        case 72: return Key::Up;
        case 80: return Key::Down;
        case 75: return Key::Left;
        case 77: return Key::Right;
        case 32: return Key::Space;
        case 13: return Key::Enter;
        case 27: return Key::Escape;
        default: return Key::Unknown;
        }
    }
};

Tab intro();
Tab get_dif();
Tab ldbd();
void put_record(string name, unsigned mines, int time, string dif);
void clear_ldbd();

int read_int(const std::string& prompt, int min, int max);
void KeyInput();
//void place_mines(std::vector<std::vector<char>> board, unsigned mine_count, unsigned board_size);
//Point find_unit(unsigned board_size);
char open_unit(std::vector<std::vector<char>> board, Point pt, unsigned board_size);
//void show_board(std::vector<std::vector<char>> board, unsigned board_size, bool show_mines = false);
void lose();
void win();
string check(string str, unsigned int board_size);

#endif