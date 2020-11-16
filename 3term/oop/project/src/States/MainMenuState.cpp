#include "MainMenuState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    MainMenuState::MainMenuState() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            std::cout << "fuck this shit" << std::endl;

        std::unique_ptr<gui::Button> button = std::make_unique<gui::Button>();

        button->setFont(m_font);
        button->setCharacterSize(20);
        button->setText("Mandelbrot Set");
        button->setTextHorizontalAlign(gui::Button::HorizontalAlign::Center);
        button->setTextVerticalAlign(gui::Button::VerticalAlign::Center);
        button->setTextColor(sf::Color::White);
        button->setSize(sf::Vector2f(200.0f, 40.0f));
        button->setPosition(400.0f, 400.0f);
        button->setBackgroundColor(sf::Color(30, 50, 150, 200));

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