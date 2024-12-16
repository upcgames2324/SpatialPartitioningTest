#ifndef MAZODEFAVORESREALES_H
#define MAZODEFAVORESREALES_H

#include <vector>
#include <algorithm>
#include <random>
#include "CartaDeFavorReal.h"

namespace TavernaBoardGame {

    class MazoDeFavoresReales {
    private:
        std::vector<CartaDeFavorReal*> deck;     // Main deck of royal favor cards
        std::vector<CartaDeFavorReal*> revealed; // Revealed cards (5 cards max)

    public:
        // Constructor and Destructor
        MazoDeFavoresReales();
        ~MazoDeFavoresReales();

        // Shuffle the deck
        void shuffle();

        // Add a card to the deck
        void add_card(CartaDeFavorReal* card);

        // Reveal cards (ensures there are always 5 revealed)
        void reveal_cards();

        // Getters
        const std::vector<CartaDeFavorReal*>& get_revealed_cards() const;
        CartaDeFavorReal* draw_from_revealed(int index);

        // Refill revealed zone after drawing
        void refill_revealed();
    };

} // namespace TavernaBoardGame

#endif // MAZODEFAVORESREALES_H
