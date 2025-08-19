#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::cin;
using std::cout;
using std::string;
using std::endl;

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

struct Point {
    string row;
    string col;
};

class Game {
public:
    
};

void intro();
char get_dif();
void place_mines(char** board, unsigned mine_count, unsigned board_size);
Point find_unit(char** board, unsigned board_size);
char open_unit(char** board, Point pt, unsigned board_size);
void show_board(char** board, unsigned board_size);
void lose();
void win();
string check(string str, unsigned int board_size);

int main(void) {
    std::srand(std::time(0));
    intro();
    bool _game = true;

    while (_game) {
        char sym = get_dif();
        char** board;
        unsigned mine_count;
        unsigned board_size;

        if (sym == 'e') {
            mine_count = easy::e_mine_count;
            board_size = easy::e_board_size;
        }
        else
            if (sym == 'n') {
                mine_count = norm::n_mine_count;
                board_size = norm::n_board_size;
            }
            else
                if (sym == 'h') {
                    mine_count = hard::h_mine_count;
                    board_size = hard::h_board_size;
                }
                else {
                    _game = false;
                    break;
                }

        board = new char* [board_size];
        for (int i = 0; i < board_size; ++i) {
            board[i] = new char[board_size];
        }
        unsigned rest_unit = board_size * board_size - mine_count;

        place_mines(board, mine_count, board_size);
        cout << "Lets im take a first move by you? (y/n) ";
        char cho;
        cin >> cho;
        if (cho == 'y') {
            find_unit(board, board_size);
            rest_unit--;           
        }
        show_board(board, board_size);
        cout << "Enter your first move(two integer, like 'row column': '2 3'): ";
        Point pt;
        
        cin >> pt.row;
        pt.row = check(pt.row, board_size);
        if (pt.row == "F") {
            _game = false;
            break;
        }
        cin >> pt.col;
        pt.col = check(pt.col, board_size);
        cout << endl;
        if (pt.row == "F" || pt.col == "F") {
            _game = false;
            break;
        }
        
        int lost = open_unit(board, pt, board_size);
        if (lost == -1) {
            lose();
            
        }
        rest_unit--;
        show_board(board, board_size);

        while (lost != -1 && rest_unit > 0) {
            cout << "Enter your next move(two integer, like 'row column': '2 3'): ";
            Point pt2;
            cin >> pt2.row;
            pt2.row = check(pt2.row, board_size);
            if (pt2.row == "F") {
                _game = false;
                break;
            }
            cin>> pt2.col;
            pt2.col = check(pt2.col, board_size);
            cout << endl;
            if (pt2.row == "F" || pt2.col == "F") {
                _game = false;
                break;
            }
            lost = open_unit(board, pt2, board_size);
            if (lost == -1) {
                lose();
                break;
            }
            rest_unit--;
            show_board(board, board_size);
        }
        
        for (int i = 0; i < board_size; ++i) {
            delete[] board[i];
        }
        delete[] board;
        if (_game == false) break;
        win();

        cout << "Do you want to play again? (y/n) ";
        char cont;
        cin >> cont;
        cout << endl;
        if (cont != 'y') _game = false;
    }

    return 0;
}

void intro() {
    std::cout << "Welcome to Minesweeper\n";
    std::cout << "Enter 'F' to end game\n";
}

char get_dif() {
    char sym;
    do {
        cout << "Choose a difficult:\n";
        cout << "";
        cin >> sym;
    } while (strchr("enhF", sym) == NULL);
    return sym;
}

void place_mines(char** board, unsigned mine_count, unsigned board_size) {
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

Point find_unit(char** board, unsigned board_size) {
    int flag = 0;
    Point pt;
    int num_row, num_col;
    while (flag == 0) {
        num_row = rand() % board_size;
        num_col = rand() % board_size;
        if (board[num_row][num_col] != '*') {
            pt.row = std::to_string(num_row);
            pt.col = std::to_string(num_col);
            board[num_row][num_col] = open_unit(board, pt, board_size);
            flag = 1;
        }
    }
    
    return pt;
}

void win() {

    cout << "Well, you win.\n";
}

char open_unit(char** board, Point pt, unsigned board_size) {
    int num_row = stoi(pt.row);
    int num_col = stoi(pt.col);
    if (board[num_row][num_col] == '*') {
        return -1;
    }
    
    int mines_around=48;//because code page Windows-1251 0 == 48 & 9 == 57
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

void show_board(char** board, unsigned board_size) {
    cout << "\t  ";
    for (unsigned int i = 0; i < board_size; i++) {
        cout << " " << i;
    }
    cout << endl;
    for(unsigned int i = 0; i < board_size; i++) {
        cout << "\t";
        if (i < 10) cout << " ";
        cout << i;
        for (unsigned int j = 0; j < board_size; j++) {
            cout << " ";
            
            if (board[i][j] == '*' || board[i][j] == '#') {
                cout << "?";
            }
            //check bombs and free units
            /*if (board[i][j] == '*')cout << "*";
            else if (board[i][j] == '#')cout << "#";*/
            else cout << board[i][j];
            if (j > 9) cout << " ";
        }
        cout << endl;
    }
}

string check(string str, unsigned int board_size) {
    if (str == "F") return str;
    int num_str = stoi(str);
    while (num_str < 0 || num_str > board_size ) {
        cout << "Please enter number again\n";
        cin >> str;
        num_str = stoi(str);
    }
    return str;
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