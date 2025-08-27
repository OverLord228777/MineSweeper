#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "func.h"
#include "LeaderBoard.h"

using std::cin;
using std::cout;
using std::string;
using std::endl;

int main(void) {
    std::srand(std::time(0));
    bool _game = true;
    string name;
    
    while (_game) {
        system("cls");
        Tab menu = intro();
        system("cls");

        if (menu == Tab::Exit) {
            _game = false;
            cout << "Thanks for play my game! Goodbye:)\n";
                return 0;
        }

        if (menu == Tab::Play) {
            Tab sym = get_dif();
            string dif;
            if (sym == Tab::Back) continue;

            //std::vector<std::vector<char>> board;
            unsigned mine_count;
            unsigned board_size;

            if (sym == Tab::Easy) {
                mine_count = easy::e_mine_count;
                board_size = easy::e_board_size;
                dif = "Easy";
            }
            else
                if (sym == Tab::Normal) {
                    mine_count = norm::n_mine_count;
                    board_size = norm::n_board_size;
                    dif = "Normal";
                }
                else
                    if (sym == Tab::Hard) {
                        mine_count = hard::h_mine_count;
                        board_size = hard::h_board_size;
                        dif = "Hard";
                    }
                    else {
                        _game = false;
                        cout << "Someyhing wrong with game.";
                        return 1;
                    }

            system("cls");
            cout << "Enter your name: ";
            cin >> name; //Поменять name в char из string хотя не
            cout << endl;

            Game game{ board_size, mine_count };
            unsigned rest_unit = board_size * board_size - mine_count;

            //game.place_mines(mine_count, board_size);
            cout << "Lets im take a first move by you? (y/n) ";
            char cho;
            cin >> cho;
            if (cho == 'y') {
                game.find_unit(board_size);
                rest_unit--;
            }
            game.show_board(board_size);
            cout << "Enter your first move(two integer, like 'row column': '2 3'): ";
            Point pt;
            clock_t start = clock();
            int row, col;

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
            string prompt = "Enter correct number\n";
            row = stoi(pt.row);
            col = stoi(pt.col);

            int lost = game.open_cell(row, col);
            if (lost == false) {
                lose();

            }
            rest_unit--;
            game.show_board(board_size);

            while (lost != false && rest_unit > 0) {
                cout << "Enter your next move(two integer, like 'row column': '2 3'): ";
                Point pt2;

                cin >> pt2.row;
                pt2.row = check(pt2.row, board_size);
                if (pt2.row == "F") {
                    _game = false;
                    break;
                }

                cin >> pt2.col;
                pt2.col = check(pt2.col, board_size);
                cout << endl;
                if (pt2.row == "F" || pt2.col == "F") {
                    _game = false;
                    break;
                }

                row = stoi(pt2.row);
                col = stoi(pt2.col);

                lost = game.open_cell(row, col);
                if (lost == false && game.is_game_over()) {
                    lose();
                    break;
                }
                rest_unit = game.is_win();
                game.show_board(board_size);
            }

            clock_t end = clock();
            int time_spent = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            
            if (_game == false) break;
            if (rest_unit == 0)
                win();

            put_record(name, rest_unit, time_spent, dif);

            cout << "Do you want to play again? (y/n) ";
            char cont;
            cin >> cont;
            cout << endl;
            if (cont != 'y') _game = false;
        }

        if (menu == Tab::LeaderBoard) {
            while (menu == Tab::LeaderBoard) {
                menu = ldbd();
            }
        }
    }
    
    return 0;
}