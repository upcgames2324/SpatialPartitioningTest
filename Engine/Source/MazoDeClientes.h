#ifndef _H
#define _H
#include <vector>


namespace TavernaBoardGame {
    class CartaDeCliente;
class MazoDeClientes {
private:
    std::vector<CartaDeCliente*> deck;        // Main deck of cards
    std::vector<CartaDeCliente*> revealed;    // Revealed cards (4 cards max)

public:
    // Constructor and Destructor
    MazoDeClientes();
    ~MazoDeClientes();

    // Shuffle the deck
    void shuffle();

    // Add a card to the deck
    void add_card(CartaDeCliente* card);

    // Reveal cards (ensures there are always 4 revealed)
    void reveal_cards();

    // Getters
    const std::vector<CartaDeCliente*>& get_revealed_cards() const;
    CartaDeCliente* draw_from_revealed(int index);

    // Manage cards at the end of the turn
    void refill_revealed();

};

}

#endif // _H
