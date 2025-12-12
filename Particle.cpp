#include "Particle.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;
using namespace Matrices;

// compare doubles
bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

// unit tests (same idea as handout)
void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4.0);
    if (r.getRows() == 2 && r.getCols() == 2 &&
        almostEqual(r(0,0), cos(theta)) &&
        almostEqual(r(0,1), -sin(theta)) &&
        almostEqual(r(1,0), sin(theta)) &&
        almostEqual(r(1,1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2 &&
        almostEqual(s(0,0), 1.5) &&
        almostEqual(s(0,1), 0.0) &&
        almostEqual(s(1,0), 0.0) &&
        almostEqual(s(1,1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3 &&
        almostEqual(t(0,0), 5)   && almostEqual(t(1,0), -5) &&
        almostEqual(t(0,1), 5)   && almostEqual(t(1,1), -5) &&
        almostEqual(t(0,2), 5)   && almostEqual(t(1,2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing Particles..." << endl;

    // center particle on screen, should map to (0,0)
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (!almostEqual(m_centerCoordinate.x, 0.0) ||
        !almostEqual(m_centerCoordinate.y, 0.0))
    {
        cout << "Failed.  Expected (0,0).  Received: ("
             << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")"
             << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        double xOld = initialCoords(0, j);
        double yOld = initialCoords(1, j);

        double xNew = m_A(0, j);
        double yNew = m_A(1, j);

        if (!almostEqual(xNew, -yOld) || !almostEqual(yNew, xOld))
        {
            cout << "Failed mapping: (" << xOld << ", " << yOld
                 << ") ==> (" << xNew << ", " << yNew << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        double xOld = initialCoords(0, j);
        double yOld = initialCoords(1, j);

        double xNew = m_A(0, j);
        double yNew = m_A(1, j);

        if (!almostEqual(xNew, 0.5 * xOld) ||
            !almostEqual(yNew, 0.5 * yOld))
        {
            cout << "Failed mapping: (" << xOld << ", " << yOld
                 << ") ==> (" << xNew << ", " << yNew << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        double xOld = initialCoords(0, j);
        double yOld = initialCoords(1, j);

        double xNew = m_A(0, j);
        double yNew = m_A(1, j);

        if (!almostEqual(xNew, 10 + xOld) ||
            !almostEqual(yNew, 5 + yOld))
        {
            cout << "Failed mapping: (" << xOld << ", " << yOld
                 << ") ==> (" << xNew << ", " << yNew << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

// constructor: make one particle
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_ttl(TTL),
      m_numPoints(numPoints),
      m_centerCoordinate(0.f, 0.f),
      m_radiansPerSec(0.f),
      m_vx(0.f),
      m_vy(0.f),
      m_cartesianPlane(),
      m_color1(),
      m_color2(),
      m_A(2, numPoints)
{
    // random spin [0, PI]
    float frac = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    m_radiansPerSec = frac * static_cast<float>(M_PI);

    // set view so (0,0) is center, y up
    m_cartesianPlane.setCenter(0.f, 0.f);
    m_cartesianPlane.setSize(
        static_cast<float>(target.getSize().x),
        -1.0f * static_cast<float>(target.getSize().y));

    // click → Cartesian center
    m_centerCoordinate =
        target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    // random vx in [100,500] with random sign
    float baseVx = 100.f + static_cast<float>(rand() % 401);
    float sign = (rand() % 2 == 0) ? -1.f : 1.f;
    m_vx = baseVx * sign;

    // random vy in [100,500]
    m_vy = 100.f + static_cast<float>(rand() % 401);

    // colors
    m_color1 = Color::White;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

    // build random star shape
    float theta = (static_cast<float>(rand()) / RAND_MAX) *
                  static_cast<float>(M_PI / 2.0);
    float dTheta = 2.f * static_cast<float>(M_PI) /
                   static_cast<float>(numPoints - 1);

    for (int j = 0; j < numPoints; ++j)
    {
        float r = 20.f + static_cast<float>(rand() % 61); // [20, 80]
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;

        theta += dTheta;
    }
}

// draw one particle
void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray fan(TriangleFan, m_numPoints + 1);

    // center
    Vector2i centerPix =
        target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
    fan[0].position = Vector2f(static_cast<float>(centerPix.x),
                               static_cast<float>(centerPix.y));
    fan[0].color = m_color1;

    // outer vertices
    for (int j = 0; j < m_numPoints; ++j)
    {
        int idx = j + 1;

        Vector2f world(static_cast<float>(m_A(0, j)),
                       static_cast<float>(m_A(1, j)));

        Vector2i pix =
            target.mapCoordsToPixel(world, m_cartesianPlane);

        fan[idx].position = Vector2f(static_cast<float>(pix.x),
                                     static_cast<float>(pix.y));
        fan[idx].color = m_color2;
    }

    target.draw(fan, states);
}

// update motion + life
void Particle::update(float dt)
{
    m_ttl -= dt;
    if (m_ttl <= 0.f)
    {
        return;
    }

    // spin + shrink
    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    // move with gravity
    float dx = m_vx * dt;
    m_vy -= G * dt;
    float dy = m_vy * dt;

    translate(dx, dy);
}

// move by (x,y)
void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, m_A.getCols());
    m_A = T + m_A;

    m_centerCoordinate.x += static_cast<float>(xShift);
    m_centerCoordinate.y += static_cast<float>(yShift);
}

// rotate around center
void Particle::rotate(double theta)
{
    Vector2f temp = m_centerCoordinate;

    // move to origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    RotationMatrix R(theta);
    m_A = R * m_A;

    // move back
    translate(temp.x, temp.y);
}

// scale from center
void Particle::scale(double c)
{
    Vector2f temp = m_centerCoordinate;

    // move to origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    ScalingMatrix S(c);
    m_A = S * m_A;

    // move back
    translate(temp.x, temp.y);
}
