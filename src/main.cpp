#include <iostream>
#include <chrono>
#include <iomanip>

#include "bitboards.h"
#include "position.h"
#include "move.h"
#include "bitfish.h"

using namespace std::chrono;


int main() {
    Bitboards::init();

    std::string line;
    while (true) {
        std::cout << "\nEnter FEN (or 'q' to quit, empty = startpos): ";
        std::getline(std::cin, line);

        if (line == "q" || line == "quit") {
            std::cout << "ByeBye!!!!!\n";
            break;
        }

        std::string fen = line.empty() ? STARTING_POS_FEN : line;

        BitFish::position(fen);
        

        std::cout << "Searching...\n";
        BitFish::go(15, 100000);
    }

    return 0;
}
