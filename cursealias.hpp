#include "unocards.hpp"

namespace {
    static std::vector<WINDOW*> windowList;
    static asio::io_context io;
    static asio::steady_timer blink_timer(io);
}

void Sblink(WINDOW *window, int yPos, int xPos, std::string string, int repeats, int delay) { // an alias for blinking text :3
    for (int i = 1 ; i < repeats ; i++) {
        wattron(window, A_REVERSE);
        mvwprintw(window, yPos, xPos, string.c_str());
        refresh();
        wrefresh(window);

        napms(delay);

        wattroff(window, A_REVERSE);
        mvwprintw(window, yPos, xPos, string.c_str());
        refresh();
		wrefresh(window);

        napms(delay);
    }
}

void winTitle(WINDOW *window, const char* string) { // an alias for printing titles
    box(window, 0, 0);
    wattron(window, A_REVERSE);
    mvwprintw(window, 0, 2, string);
    wattroff(window, A_REVERSE);
}

void addRefreshList(WINDOW *window) {
    windowList.push_back(window);
}

void refreshAll() {
    refresh();
    for (int flflfr = 0 ; flflfr < windowList.size() ; flflfr++) {
        wrefresh(windowList.at(flflfr));
    }
}