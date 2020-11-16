#include "MainMenuState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    MainMenuState::MainMenuState() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            std::cout << "fuck this shit" << std::endl;

        std::unique_ptr<gui::Button> button = std::make_unique<gui::Button>();

        button->setFont(m_font);
        button->setCharacterSize(12);
        button->setText("Mandelbrot set");
        button->setSize(sf::Vector2f(100.0f, 40.0f));
        button->setPosition(40.0f, 40.0f);
        button->setBackgroundColor(sf::Color::Magenta);

        m_buttons["Mandelbrot Set"] = std::move(button);
    }

    // public methods
    void MainMenuState::update(const float& ftime) {

    }

    void MainMenuState::render(sf::RenderTarget& target) {
        for (const auto& [name, button] : m_buttons)
            target.draw(*button);
    }

} // namespace ng