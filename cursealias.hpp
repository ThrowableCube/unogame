#include "unocards.hpp"

namespace {
    std::vector<WINDOW*> windowList;
}

void Sblink(WINDOW *window, int yPos, int xPos, std::string string) { // an alias for blinking text :3
    wattron(window, A_REVERSE);
    mvwprintw(window, yPos, xPos, string.c_str());
    refresh();
    wrefresh(window);

    blink_timer.expires_after(std::chrono::milliseconds(250));

    blink_timer.async_wait([window, yPos, xPos, string](const asio::error_code& ec) {
        if (!ec) {
            wattroff(window, A_REVERSE);
            mvwprintw(window, yPos, xPos, string.c_str());
            wrefresh(window);
        }
    });
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