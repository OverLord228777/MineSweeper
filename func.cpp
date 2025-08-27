#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#include "func.h"

using std::cin;
using std::cout;
using std::string;
using std::endl;

void KeyInput() {
    Tab but = Tab::None;
    string str_menu = "\033[32mBack\033[0m Clear";
    GameInput input;
    Key menu_key;
}

Tab intro() {
    Tab sym = Tab::None;
    string str_menu = "\n\tExit \033[32mPlay\033[0m LeaderBoard";
    GameInput input;
    Key menu_key;
    int x = 1;

    do {
        std::cout << "\t\tWelcome to Minesweeper\n";
        std::cout << "\tPress 'Esc' to end game\n";
        cout << str_menu;

        menu_key = input.GetInput();
        switch (menu_key) {
        case Key::Left: x--; break;
        case Key::Right: x++; break;
        case Key::Escape: sym = Tab::Exit; break;
        case Key::Unknown: break; // Просто игнорируем неизвестные нажатия
        }

        // Ограничение по границам
        if (x < 0) x = 0;
        if (x > 2) x = 2;

        if (x == 0 && menu_key == Key::Enter) {
            sym = Tab::Exit;
            break;
        }
        if (x == 1 && menu_key == Key::Enter) {
            sym = Tab::Play;
            break;
        }
        if (x == 2 && menu_key == Key::Enter) {
            sym = Tab::LeaderBoard;
            break;
        }
        if (x == 0) str_menu = "\n\t\033[32mExit\033[0m Play LeaderBoard";
        if (x == 1) str_menu = "\n\tExit \033[32mPlay\033[0m LeaderBoard";
        if (x == 2) str_menu = "\n\tExit Play \033[32mLeaderBoard\033[0m";

        system("cls");

    } while (sym == Tab::None);
    return sym;
}

Tab get_dif() {
    Tab sym = Tab::None;
    string str_menu = "\n\tBack \033[32mEasy\033[0m Normal Hard";
    GameInput input;
    Key menu_key;
    int x = 1;
    
    do {
        cout << "Choose a difficult:\n";
        cout << str_menu;

        menu_key = input.GetInput();
        switch (menu_key) {
        case Key::Left: x--; break;
        case Key::Right: x++; break;
            //case Key::Enter: std::cout << "Escape pressed" << std::endl; break;
        case Key::Unknown: break; // Просто игнорируем неизвестные нажатия
        }

        // Ограничение по границам
        if (x < 0) x = 0;
        if (x > 3) x = 3;

        if (x == 0 && menu_key == Key::Enter) {
            sym = Tab::Back;
            break;
        }
        if (x == 1 && menu_key == Key::Enter) {
            sym = Tab::Easy;
            break;
        }
        if (x == 2 && menu_key == Key::Enter) {
            sym = Tab::Normal;
            break;
        }
        if (x == 3 && menu_key == Key::Enter) {
            sym = Tab::Hard;
            break;
        }
        if (x == 0) str_menu = "\n\t\033[32mBack\033[0m Easy Normal Hard";
        if (x == 1) str_menu = "\n\tBack \033[32mEasy\033[0m Normal Hard";
        if (x == 2) str_menu = "\n\tBack Easy \033[32mNormal\033[0m Hard";
        if (x == 3) str_menu = "\n\tBack Easy Normal \033[32mHard\033[0m";

        system("cls");
    } while (sym == Tab::None);
    return sym;
}


Point Game::find_unit(unsigned board_size) {
    int flag = 0;
    Point pt;
    int num_row, num_col;
    while (flag == 0) {
        num_row = rand() % board_size;
        num_col = rand() % board_size;
        if (Game::board[num_row][num_col] != '*') {
            pt.row = std::to_string(num_row);
            pt.col = std::to_string(num_col);
            Game::board[num_row][num_col] = open_unit(Game::board, pt, board_size);
            flag = 1;
        }
    }

    return pt;
}

void win() {

    cout << "Nahh, you win.\n";
}

char open_unit(std::vector<std::vector<char>> board, Point pt, unsigned board_size) {
    int num_row = stoi(pt.row);
    int num_col = stoi(pt.col);
    if (board[num_row][num_col] == '*') {
        return -1;
    }

    int mines_around = 48;//because code page Windows-1251 0 == 48 & 9 == 57
    for (unsigned int i = num_row - 1; i <= num_row + 1; i++) {
        for (unsigned int j = num_col - 1; j <= num_col + 1; j++) {
            if (i != '*' && i < board_size && j != '*' && j < board_size) {
                if (board[i][j] == '*') mines_around++;
            }
        }
    }
    board[num_row][num_col] = (char)mines_around;
    return (char)mines_around;
}

bool Game::open_cell(int row, int col) {
    if (board[row][col] == '*') {
        game_over = true;
        reveal_mines();
        return false;
    }
    int mines = count_mines(row, col);
    board[row][col] = (mines > 0) ? '0' + mines : ' ';
    opened_count++;

    if (mines == 0) {
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                open_cell(row + i, col + j);
    }
    return true;
}

void Game::show_board(unsigned board_size, bool show_mines) {
    cout << "\t\t  ";
    for (unsigned int i = 0; i < board_size; i++) {
        cout << " " << i;
    }
    cout << endl;

    for (unsigned int i = 0; i < board_size; i++) {
        cout << "\t\t";
        if (i < 10) cout << " ";
        cout << i;
        for (unsigned int j = 0; j < board_size; j++) {
            cout << " ";

            if ((Game::board[i][j] == '*' || Game::board[i][j] == '#') && show_mines == false) {
                cout << "?";
            }
            //check bombs and free units
            /*if (show_mines == true) {
                if (Game::board[i][j] == '*')cout << "*";
                else if (Game::board[i][j] == '#')cout << "#";
            }*/
            cout << Game::board[i][j];
            if (j > 9) cout << " ";
        }
        cout << endl;
    }
}

string check(string str, unsigned int board_size) {
    if (str == "F") return str;
    int num_str = stoi(str);
    while (num_str < 0 || num_str > board_size) {
        cout << "Please enter number again\n";
        cin >> str;
        num_str = stoi(str);
    }
    return str;
}

int read_int(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

void lose() {
    cout << endl; //ascii art - BOOM
    cout << "\
                                 .          ...    ..:=+:                  \n\
                                .%*+=:. .:=+**#-:=***=:-%.                \n\
                                .@..:*#=#+:.  .*+:      *+.               \n\
                                :@:::::::::::-==========+@+.              \n\
                                .*+-:::::::::.    .-*%%%%%#*-:..      ...        ... \n\
                ......           .......         :@:        .=@+.     -@%       :@@: \n\
              .@=....-%@=     .%%-.....-%%-    .#+.           .:@.   :@.-%     *#.-#.\n\
              .@      ..:@..*%-..       ..:*%::%-.             .:@: :@. .-%.  +#. .+#.\n\
              .@   .%:.-=.@+:.             ..*@:    .:.          .%=@:   .=#.#+.   .-%. \n\
              =#  .:#@+-. *+      +-   :-:   .*+    .-#=..:*+     :@-     .*#+.      **.\n\
             .%-  :=::=   *+      .=#-+#=:.   .%:     .+%#+:.     .#-      .:.       .#=.\n\
             .@.     .:-+*+.        :@%:       *+     .#*#.       .#-                 .%-\n\
             -%+++**#%@#+:.        :%-:#=.     ++    -#-.-%.      .#-          .:.     :%.\n\
             +*      ..:-+#+.     .%:  .#+.   .%-    -.  .+#.     :%: .-**    .##*:     *=\n\
            .#-   ....-    -%.    ..    .-.  .*+.               .-%:.:**.@:  .%=.:#+.   :%.\n\
            .@.  .+##@-     %:             ..**..            ..:+#:=#*.  :@.:%:    +#:.  *+. ....::.\n\
          .:=#    .@:*%.   :@:           ..=%:-##+-:.......:-#@*=%*.      +#%.   ...:@-::-@+*#%%#+=.\n\
          .@@:           ..@%%-:::::::::+%+.       :+++++++:.:::.  ..........:-+#%@%#+##::*#.\n\
          -@%...........-@#.                         .........:--+#%@@@%#+:. \n\
          .....:::::::::...     ..::-=*#%@@@%#*===----:::...............\n\
               .:=++***####*=---:::::........ \n\
     .**#*++==--:::........  \n\
     .....                                                                                          \n";

    cout << "Well, you lose...\n";
}