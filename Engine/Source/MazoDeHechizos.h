#ifndef MAZODEHECHIZOS_H
#define MAZODEHECHIZOS_H

#include <vector>
#include <algorithm>
#include <random>
#include "CartaDeHechizo.h"

namespace TavernaBoardGame {

    class MazoDeHechizos {
    private:
        std::vector<CartaDeHechizo*> deck; // Main deck of spell cards

    public:
        // Constructor and Destructor
        MazoDeHechizos();
        ~MazoDeHechizos();

        // Methods
        void shuffle();                          // Shuffle the deck
        void add_card(CartaDeHechizo* card);     // Add a card to the deck
        CartaDeHechizo* draw_card();             // Draw a card from the top of the deck

        // Check if the deck is empty
        bool is_empty() const;
    };

} // namespace TavernaBoardGame

#endif // MAZODEHECHIZOS_H
