#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Particle.h"

class Engine
{
private:
    sf::RenderWindow m_Window;       // game window
    std::vector<Particle> m_particles; // active particles

public:
    Engine();  // make window

    void run();  // main loop

private:
    void input();        // handle keys/mouse
    void update(float dt); // update particles
    void draw();         // draw everything
};

#endif
