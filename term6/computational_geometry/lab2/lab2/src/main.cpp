#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include <SFML/Graphics.hpp>

#include "fortune_algorithm.hpp"

std::vector<cg_lab2::Vector2> GeneratePoints(std::uint32_t pointCount)
{
    static std::default_random_engine generator(static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()));

    std::uniform_real_distribution<double> distribution{ 0.0, 1.0 };

    std::vector<cg_lab2::Vector2> points;
    for (int i = 0; i < pointCount; ++i)
    {
        points.push_back(cg_lab2::Vector2{ distribution(generator), distribution(generator) });
    }

    return points;
}

void DrawPoints(sf::RenderWindow& window, cg_lab2::VoronoiDiagram& diagram)
{
    static const auto drawPoint = [&window](const cg_lab2::Vector2& point, const sf::Color& color)
    {
        const float radius{ 0.005f };

        sf::CircleShape shape(radius);
        shape.setPosition(sf::Vector2f(point.x - radius, 1 - point.y - radius));
        shape.setFillColor(color);

        window.draw(shape);
    };

    for (std::size_t i = 0; i < diagram.GetSiteCount(); ++i)
    {
        drawPoint(diagram.GetSite(i)->point, sf::Color{ 250, 200, 50 });
    }
}

void DrawDiagram(sf::RenderWindow& window, cg_lab2::VoronoiDiagram& diagram)
{
    static const auto& drawEdge = [&window](const cg_lab2::Vector2& from, const cg_lab2::Vector2& to, const sf::Color& color)
    {
        const std::array<sf::Vertex, 2> vertices
        {
            sf::Vertex{ sf::Vector2f{ static_cast<float>(from.x), 1.0f - static_cast<float>(from.y) }, color },
            sf::Vertex{ sf::Vector2f{ static_cast<float>(to.x), 1.0f - static_cast<float>(to.y) }, color }
        };

        window.draw(vertices.data(), vertices.size(), sf::Lines);
    };

    for (std::size_t i = 0; i < diagram.GetSiteCount(); ++i)
    {
        const cg_lab2::VoronoiDiagram::Site* site{ diagram.GetSite(i) };
        const cg_lab2::Vector2 center{ site->point };

        const cg_lab2::VoronoiDiagram::Face* face{ site->face };
        const cg_lab2::VoronoiDiagram::HalfEdge* halfEdge{ face->outerComponent };

        if (!halfEdge)
        {
            continue;
        }

        while (halfEdge->prev)
        {
            halfEdge = halfEdge->prev;

            if (halfEdge == face->outerComponent)
            {
                break;
            }
        }

        const cg_lab2::VoronoiDiagram::HalfEdge* start{ halfEdge };

        while (halfEdge)
        {
            if (halfEdge->origin && halfEdge->destination)
            {
                const cg_lab2::Vector2 origin{ (halfEdge->origin->point - center) * 1.0f + center };
                const cg_lab2::Vector2 destination{ (halfEdge->destination->point - center) * 1.0f + center };

                drawEdge(origin, destination, sf::Color{ 50, 100, 250 });
            }

            halfEdge = halfEdge->next;

            if (halfEdge == start)
            {
                break;
            }
        }
    }
}

cg_lab2::VoronoiDiagram GenerateRandomDiagram(std::uint32_t pointCount)
{
    std::vector<cg_lab2::Vector2> points{ GeneratePoints(pointCount) };

    cg_lab2::FortuneAlgorithm algorithm(points);
    algorithm.Construct();
    algorithm.Bound(cg_lab2::Box{ -0.05, -0.05, 1.05, 1.05 });

    cg_lab2::VoronoiDiagram diagram{ algorithm.GetVoronoiDiagram() };

    if (!diagram.intersect(cg_lab2::Box{ 0.0, 0.0, 1.0, 1.0 }))
    {
        throw std::runtime_error("An error occured in the box intersection algorithm");
    }

    return diagram;
}

int main()
{
    // Init window
    const std::uint32_t windowWidth{ 1000 };
    const std::uint32_t windowHeight{ 1000 };
    const std::string_view windowTitle{ "Voronoi Diagram" };

    const std::uint32_t style{ sf::Style::Close };

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle.data(), style, settings);
    window.setView(sf::View(sf::FloatRect{ -0.01f, -0.01f, 1.02f, 1.02f }));

    // Init VoronoiDiagram
    const std::uint32_t pointCount{ 50 };
    cg_lab2::VoronoiDiagram diagram{ GenerateRandomDiagram(pointCount) };

    // Draw
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::N)
            {
                diagram = GenerateRandomDiagram(pointCount);
            }
        }

        window.clear(sf::Color{ 30, 30, 30 });

        DrawPoints(window, diagram);
        DrawDiagram(window, diagram);

        window.display();
    }

    return 0;
}
