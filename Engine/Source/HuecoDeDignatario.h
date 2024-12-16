#ifndef HUECODEDIGNATARIO_H
#define HUECODEDIGNATARIO_H

#include <memory> // For smart pointers
#include "Dignatario.h"

namespace TavernaBoardGame {

    class HuecoDeDignatario {
    private:
        std::shared_ptr<Dignatario> dignatario; // Pointer to the dignitary occupying this spot
        float radius;                           // Radius of the cylindrical bounding box
        float height;                           // Height of the cylindrical bounding box

    public:
        // Constructor and Destructor
        HuecoDeDignatario(float radius, float height);
        ~HuecoDeDignatario();

        // Check if the space is occupied
        bool is_occupied() const;

        // Get the occupying dignitary
        std::shared_ptr<Dignatario> get_dignatario() const;

        // Set the occupying dignitary
        void set_dignatario(std::shared_ptr<Dignatario> dignatario);

        // Get bounding box properties
        float get_radius() const;
        float get_height() const;
    };

} // namespace TavernaBoardGame

#endif // HUECODEDIGNATARIO_H
