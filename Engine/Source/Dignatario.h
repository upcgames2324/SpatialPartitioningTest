#pragma once

namespace TavernaBoardGame {
    class HuecoDeDignatario;

class Dignatario {
public:
    Dignatario();  // Constructor
    ~Dignatario();  // Destructor

    virtual void activate() = 0;

    HuecoDeDignatario* GetPosition() const { return m_position; }
    // Add public methods and attributes here
private:
    HuecoDeDignatario* m_position;
};

}
