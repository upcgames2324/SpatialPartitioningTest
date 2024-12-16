#ifndef _H
#define _H

#include "GlobalsTaverna.h"

namespace TavernaBoardGame {

class HuecoDeMesa {
private:
    Races m_race;
    bool occupied;          // Whether the spot is currently occupied

public:
    HuecoDeMesa();  // Constructor
    ~HuecoDeMesa();  // Destructor

    // Add public methods and attributes here
    Races get_race() const;            // Get the race of the spot
    void set_race(Races race);         // Set the race of the spot

    bool is_occupied() const;          // Check if the spot is occupied
    void set_occupied(bool state);     // Set the occupied state

};

}

#endif // _H
