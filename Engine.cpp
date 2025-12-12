#include "Engine.h"
#include <ctime>

// create the window
Engine::Engine()
{
    m_Window.create(sf::VideoMode::getDesktopMode(),
                    "Particle Fireworks");
    std::srand((unsigned)std::time(nullptr));
}

// main game loop
void Engine::run()
{
    sf::Clock clock;

    std::cout << "Starting Particle unit tests...\n";
    Particle p(
        m_Window,
        4,
        { (int)m_Window.getSize().x / 2,
          (int)m_Window.getSize().y / 2 });
    p.unitTests();
    std::cout << "Unit tests complete.  Starting engine...\n";

    while (m_Window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        input();
        update(dt);
        draw();
    }
}

// handle input
void Engine::input()
{
    sf::Event e;

    while (m_Window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
            m_Window.close();

        if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Escape)
            m_Window.close();

        // spawn particles on left click
        if (e.type == sf::Event::MouseButtonPressed &&
            e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i pos(e.mouseButton.x, e.mouseButton.y);

            for (int i = 0; i < 5; ++i)
            {
                int pts = 25 + (std::rand() % 26); // 25–50
                m_particles.emplace_back(m_Window, pts, pos);
            }
        }
    }
}

// update particles
void Engine::update(float dt)
{
    auto it = m_particles.begin();

    while (it != m_particles.end())
    {
        if (it->getTTL() > 0.f)
        {
            it->update(dt);
            ++it;
        }
        else
        {
            it = m_particles.erase(it);
        }
    }
}

// draw everything
void Engine::draw()
{
    m_Window.clear();

    for (auto& p : m_particles)
        m_Window.draw(p);

    m_Window.display();
}
