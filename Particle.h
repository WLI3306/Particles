#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>
#include "Matrices.h"

using namespace Matrices;

// short comment: particle object
class Particle : public sf::Drawable
{
private:
    float m_ttl;
    int   m_numPoints;
    Matrix m_A;
    float m_radiansPerSec;
    float m_vx, m_vy;
    sf::Vector2f m_centerCoordinate;
    sf::View m_cartesianPlane;
    sf::Color m_color1, m_color2;

public:
    Particle(sf::RenderTarget& target, int numPoints, sf::Vector2i mouseClick);

    float getTTL() const
    {
        return m_ttl;
    }

    void update(float dt);

    void translate(double x, double y);
    void rotate(double theta);
    void scale(double c);

    bool almostEqual(double a, double b, double eps = 0.001);
    void unitTests();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

// constants
const float TTL   = 5.0f;
const float SCALE = 0.999f;
const float G     = 300.0f;

#endif 
