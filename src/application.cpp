#include "application.h"
#include <iostream>
#include <ncurses.h>
#include <clocale>

bool Application::initialize() {
    setlocale(LC_ALL, "");
    initscr();
    getmaxyx(stdscr, stdsrc_height_, stdsrc_width_);
    keypad(stdscr, true);
    cbreak();
    return true;
};

bool Application::run() {
    while(!should_close_) {

        if(getch() == KEY_UP) {
            this->current_program_.push_back("a⠃l");
        }
        for(int index{}; index < this->current_program_.size(); ++index) {
            printw(this->current_program_[index]);
            refresh();
        }
    }
    return true;
};

void Application::shutdown() {
    endwin();
    std::cout << "shutting down..." << "\n";
};
