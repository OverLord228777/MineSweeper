#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "LeaderBoard.h"
#include "func.h"

using std::string;

int show_ldbd() {
    std::ifstream file("Records.csv"); // ��������� ����
    std::vector<std::vector<std::string>> data;
    std::string line;

    while (std::getline(file, line)) { // ������ ���������
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(ss, cell, ';')) { // ��������� �������
            row.push_back(cell);
        }
        data.push_back(row);
    }

    // ������ ������ ������ �� �������
    for (const auto& row : data) {
        std::cout << "\t";
        for (const auto& cell : row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }

    file.close();
    return 0;
}

void put_record(string name, unsigned mines, int time, string dif) {
    std::ofstream outputFile("Records.csv", std::ios::app);

    if (outputFile.is_open()) {
        // ���������
        //outputFile << "Name;Count_of_mines;Time;Difficult; " << '\n';

        int min = time / 60;
        int sec = time % 60;


        // ���������� ������ ������ ������
        outputFile << name << ";" << mines << ";" << min << ":" << sec << ";" << dif << ";" << '\n';

        // ��������� ����
        outputFile.close();
        std::cout << "Data was saved" << std::endl;
    }
    else {
        std::cerr << "Error with file opening" << std::endl;
    }

}

void clear_ldbd() {
    std::string file("Records.csv");
    std::ofstream del_file(file, std::ios::out);
    if (del_file.is_open()) {
        // ���� ���� ������� ��������, �� ��������� ���.
        // ��� ���� �������� � �������� � ������ "w" �������� ����.
        del_file.close();
    }
    else {
        std::cerr << "Error with file opening" << std::endl;
    }

    std::ofstream h_file("Records.csv");

    if (h_file.is_open()) {
        h_file << "Name;Recent_cells;Time;Difficult;" << '\n';
        h_file << "OverLord;15;42:42;Hard;" << '\n';
        h_file.close();
    }
    else {
        std::cerr << "Error with file opening" << std::endl;
    }
}

Tab ldbd() {
    Tab but = Tab::None;
    string str_menu = "\n\t\033[32mBack\033[0m\t\tClear";
    GameInput input;
    Key menu_key;
    int x = 0;

    do {
        show_ldbd();
        cout << str_menu;

        menu_key = input.GetInput();
        switch (menu_key) {
        case Key::Left: x--; break;
        case Key::Right: x++; break;
        case Key::Escape: but = Tab::Exit; break;
        case Key::Unknown: break; // ������ ���������� ����������� �������
        }

        // ����������� �� ��������
        if (x < 0) x = 0;
        if (x > 1) x = 1;

        if (x == 0 && menu_key == Key::Enter) {
            but = Tab::Back;
            break;
        }
        if (x == 1 && menu_key == Key::Enter) {
            but = Tab::Clear;
            clear_ldbd();
        }
        
        if (x == 0) str_menu = "\n\t\033[32mBack\033[0m\t\tClear";
        if (x == 1) str_menu = "\n\tBack\t\t\033[32mClear\033[0m";
        

        system("cls");

    } while (but == Tab::None || but == Tab::Clear);
    return but;
}