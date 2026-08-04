#include "unocards.hpp"
#include "cursealias.hpp"

int main() {
    // here come the tornado of shit code

    // no i have to sleep // my mom told me to sleep here ok
    // ... this is awkward i updated the wrong file and i got confused on why my code changes arent working.

    // i wonder how much of these comments can be taken out of context
    
    int ch = 0;
    srand(time(0));
    std::random_device rd;
    std::mt19937 g(rd());

    // go commit die
    // ur so mean waaaaa

    std::vector<uno::player> players = {};
    std::vector<uno::card> deck = {};
    std::vector<uno::card> pile = {};

    players.push_back({"noName", 0, {}}); // what the fuck?

    // somebodys gonna get confused by this so...
    for (int x = 0 ; x < 4 ; x++) { // this cycles color
        deck.push_back({0, static_cast<uno::colors>(x), uno::types::none});
        for (int z = 0 ; z < 2 ; z++) { // this generates the card 2 times
            for (int y = 0 ; y < 9 ; y++) { // this for generates number cards above 0
                deck.push_back({(y + 1), static_cast<uno::colors>(x), uno::types::none});
            }

            deck.push_back({42, static_cast<uno::colors>(x), uno::types::draw});
            deck.push_back({42, static_cast<uno::colors>(x), uno::types::reverse});
            deck.push_back({42, static_cast<uno::colors>(x), uno::types::skip});
        }
    }

    for (int i = 0 ; i < 4 ; i++) {
        deck.push_back({69, static_cast<uno::colors>(4), uno::types::wild});
        deck.push_back({69, static_cast<uno::colors>(4), uno::types::wilddraw});
    }

    // end of hell code

    std::shuffle(deck.begin(), deck.end(), g);

    std::string colorArray[4] = { // do we need this?
        // oh wait i dont have contributers no ones gonna answer this.
        "Red",
        "Blue",
        "Green",
        "Yellow"
    };

    uno::card lastCard = {3, uno::colors::green, uno::types::none};

    int selection = 0;
    int displayedLine = 0;
    int displayOffset = 0;
    int result = 0;
    int currentPlayer = 0;
    int threshold = 50;
    int tmpVal = 0; // This does not mean the value is temporary,
    int y;
    int x;
    getmaxyx(stdscr, y ,x);
    bool notOOB = 0;
    bool alreadyDidIt = 0;
    bool notClose = 1;
    // fuck you C89

    for (int i = 0 ; i < 15 ; i++) {
        players.at(currentPlayer).hand.push_back(deck.front());
        deck.erase(deck.begin());
    }

    while (1) {
        if (deck.at(tmpVal).type == uno::types::none && deck.at(tmpVal).color != uno::colors::wild) {
            transferVectorCard(deck, tmpVal, pile);
            break;
        } else {
            tmpVal++;
        }
    }

    // start of pdcurses
    // OUUU SHI VS CODE RECOGNIZES IT
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, 1);
    nodelay(stdscr, 1);

    if (has_colors() == 0) {
        printw("Your terminal doesn't support color.");
        refresh();
    } else {
        start_color();
        use_default_colors();
    }

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    // staircase :face_holding_back_tears:

    WINDOW* cardWindow = newwin(14,14,1,1);

    WINDOW* pileWindow = newwin(4,49,16,1);

    WINDOW* playerWindow = newwin(14,34,1,16);

	WINDOW* playerSetupWindow = newwin(19,49,1,1);
    winTitle(playerSetupWindow, " Player Setup ");

    addRefreshList(cardWindow);
    addRefreshList(pileWindow);
    addRefreshList(playerWindow);

	// Player setup
    while (notClose) {
        mvwprintw(playerSetupWindow,1,1,"W.I.P, Press enter to close.");
		refresh();
		wrefresh(playerSetupWindow);
        ch = getch();
        switch (ch) {
            case KEY_DOWN:
                // selection--;
                break;
            case KEY_UP:
                // selection++;
                break;
            case '\n':
                notClose = 0;
                break;
        }
	}

	notClose = 1

    //////////////////////////////////////////////////////////////////////////

    winTitle(playerWindow, " Player ");
    winTitle(cardWindow, " Hand ");
    winTitle(pileWindow, " Discard Pile ");

    while (notClose) {
        io.poll();
        mvwprintw(playerWindow, 12, 1, "                                ");
        mvwprintw(playerWindow, 12, 1, printFNCard(players.at(currentPlayer).hand.at(selection + displayOffset)).c_str());
        for (int i = 0 ; i < 10 ; i++) {
            if ((i + displayOffset) >= players.at(currentPlayer).hand.size()) {
                mvwprintw(cardWindow, (11 - i), 1, "  ");
            } else {
                if (i == selection) {
                    wattron(cardWindow, A_REVERSE);
                }
                wattron(cardWindow, COLOR_PAIR(getCardColorAsANSI(players.at(currentPlayer).hand.at(i + displayOffset))) | A_BOLD);
                mvwprintw(cardWindow, (11 - i), 1, printTLCard(players.at(currentPlayer).hand.at((i + displayOffset))).c_str());
                wattroff(cardWindow, COLOR_PAIR(getCardColorAsANSI(players.at(currentPlayer).hand.at(i + displayOffset)) | A_BOLD));
                wattroff(cardWindow, A_REVERSE);
            }
        }

        wattron(pileWindow, COLOR_PAIR(getCardColorAsANSI(pile.at(pile.size() - 1))) | A_BOLD);
        mvwprintw(pileWindow, 1, 1, printTLCard(pile.at(pile.size() - 1)).c_str());
        wattroff(pileWindow, COLOR_PAIR(getCardColorAsANSI(pile.at(pile.size() - 1))) | A_BOLD);

        mvwprintw(pileWindow, 2, 1, "%d", static_cast<int>(pile.size()));
        mvwprintw(pileWindow, 2, 5, "/");
        mvwprintw(pileWindow, 2, 7, "%d", threshold);

        refreshAll();

        mvwprintw(playerWindow, 1, 1, "                      ");
	    mvwprintw(playerWindow, 2, 1, "                      ");
        mvwprintw(playerWindow, 1, 1, "Player %d", currentPlayer + 1);
	    mvwprintw(playerWindow, 2, 1, "Cards: %d", players.at(currentPlayer).hand.size());

        mvwprintw(cardWindow, 12, 1, "PageDown");
        mvwprintw(cardWindow, 1, 1, "PageUp");

        // THIS BLOCK IS NOTHING, YOU SERVE ZERO PURPOSE, YOU SHOULD DELETE THIS CODE. NOW.
        wattron(playerWindow, COLOR_PAIR(getCardColorAsANSI(lastCard)));
        wattroff(playerWindow, COLOR_PAIR(getCardColorAsANSI(lastCard)));

        ch = getch();
        switch (ch) {
            case KEY_DOWN:
                selection--;
                break;
            case KEY_UP:
                selection++;
                break;
            case '\n':
                result = discardCard(players.at(currentPlayer).hand.at(selection + displayOffset), lastCard, 1);
                if (result) {
                    mvwprintw(playerWindow, 11, 1, "                                ");
                    mvwprintw(playerWindow, 11, 1, "Discard Result: %d", result);
                    transferVectorCard(players.at(currentPlayer).hand, selection + displayOffset, pile);
                    // pile.push_back(players.at(currentPlayer).hand.at(selection + displayOffset));
                    // players.at(currentPlayer).hand.erase(players.at(currentPlayer).hand.begin() + selection + displayOffset);
                } else { 
                    mvwprintw(playerWindow, 11, 1, "You cannot play this card.");
                }
                break;
        }
        if (selection > 9) {
            selection = 9;
            displayOffset++;
        } else if (selection < 0) {
            selection = 0;
            displayOffset--;
        } else if (selection > players.at(currentPlayer).hand.size() - 1) {
            selection = players.at(currentPlayer).hand.size() - 1;
        }

        if (displayOffset < 0) {
            displayOffset = 0;
        } else if (displayOffset > players.at(currentPlayer).hand.size() - 10) {
            displayOffset = players.at(currentPlayer).hand.size() - 10;
        }
    }

    endwin();
    printf("List was emptied or some other function broke the loop.\n");
    return 0;
}
