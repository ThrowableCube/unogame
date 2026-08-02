#pragma once
#include <cstdlib>
#include <ctime>
#include <vector>
#include <curses.h>
#include <asio.hpp>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <random>

static asio::io_context io;
static asio::steady_timer blink_timer(io);

class uno {
    public:
        enum class colors {
            red = 0,
            blue,
            green,
            yellow,
            wild
        };
        enum class types {
            none = 0,
            draw,
            reverse,
            skip,
            wild,
            wilddraw
        };

        struct card {
            int number;
            colors color;
            types type;
        };

        struct player {
            std::string name;
            bool skipped;
            std::vector<card> hand;
        };
};

void transferVectorCard(std::vector<uno::card>& from, int index, std::vector<uno::card>& to) { // hi
    try {
        to.push_back(from.at(index));
        from.erase(from.begin() + index);
    } catch (...) {
        mvprintw(0,0,"transferVectorCard: Exception Caught, please restart the game.");
        refresh();
    }
}

std::string printCardFN(uno::card card) {
    std::string rs;
    std::vector<std::string> letterColor = {"Red ", "Blue ", "Green ", "Yellow "};
    if (card.type == uno::types::none) {
        rs = letterColor.at(static_cast<int>(card.color));
        rs += std::to_string(card.number);
    } else {
        switch (static_cast<int>(card.type)) {
            case 1:
                rs = letterColor.at(static_cast<int>(card.color));
                rs += "+2";
                break;
            case 2:
                rs = letterColor.at(static_cast<int>(card.color));
                rs += "Reverse";
                break;
            case 3:
                rs = letterColor.at(static_cast<int>(card.color));
                rs += "Skip";
                break;
            case 4:
                rs = "Wild";
                break;
            case 5:
                rs = "Wild +4";
                break;
            default:
                rs = "Error...";
                break;
        }
    }
    return rs; // changed back to std string
}

std::string printTLCard(uno::card card) {
    std::string rs;
    std::vector<std::string> letterColor = {"R", "B", "G", "Y"};
    if (card.type == uno::types::none) {
        rs = letterColor.at(static_cast<int>(card.color));
        rs += std::to_string(card.number);
    } else {
        switch (static_cast<int>(card.type)) {
            case 1:
                rs = letterColor.at(static_cast<int>(card.color));
                rs += "+";
                break;
            case 2:
                rs = letterColor.at(static_cast<int>(card.color));
                rs += "R";
                break;
            case 3:
                rs = letterColor.at(static_cast<int>(card.color));
                rs += "S";
                break;
            case 4:
                rs = "WC";
                break;
            case 5:
                rs = "W+";
                break;
            default:
                rs = "!E";
                break;
        }
    }
    return rs; // changed back to std string
}

int getCardColorAsANSI(uno::card card) { // Better, but still revise if possible.
    switch (card.color) {
        case uno::colors::red:
            return 1;
        case uno::colors::blue:
            return 2;
        case uno::colors::green:
            return 3;
        case uno::colors::yellow:
            return 4;
        default:
            return 5;
    }
}

int discardCard(uno::card discardingCard, uno::card &lastCard, bool mutate = 0) { // might need a rewrite.
    // HOLY SHIT I REMOVED THE PRINT AND PAUSE CODE AND IT LOOKS INFINITELY SMALLER.
    // Yeah about that...

    // handling special cards
    if (discardingCard.type == uno::types::wild) {
        return 2; // YES DO IT HERE NOT THERE // yes sir ;-;
    } else if (discardingCard.type == uno::types::wilddraw) {
        return 3;
    }
    
    // handling special colored cards
    else if (discardingCard.type == uno::types::draw && discardingCard.color == lastCard.color) {
        return 4;
    } else if (discardingCard.type == uno::types::reverse && discardingCard.color == lastCard.color) {
        return 5; // function here...
    } else if (discardingCard.type == uno::types::skip && discardingCard.color == lastCard.color) {
        return 6;
    }

    // handling normal cards
    else if (discardingCard.color == lastCard.color) { // checks for matching color or wildcard
        return 1; // if it's two open the color picker! :D // FUCK NO. // you're so mean :'(
    } else if (discardingCard.number == lastCard.number) {
        if (mutate) {
            lastCard.color = discardingCard.color;
	    }
        return 1;
    }
    return 0; // if none of the ifs ran
}

