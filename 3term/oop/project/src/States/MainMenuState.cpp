#include "MainMenuState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    MainMenuState::MainMenuState(sf::RenderWindow& window) : m_window(window) {
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
        button->setBackgroundIdleColor(sf::Color(30, 50, 150, 200));
        button->setBackgroundHoverColor(sf::Color(30, 40, 170, 220));
        button->setBackgroundActiveColor(sf::Color(30, 40, 200, 230));

        m_buttons["Mandelbrot Set"] = std::move(button);
    }

    // public methods
    void MainMenuState::update(const float& ftime) {
        for (auto&& [name, button] : m_buttons)
            button->update(sf::Mouse::getPosition(m_window));
    }

    void MainMenuState::render(sf::RenderTarget& target) {
        for (const auto& [name, button] : m_buttons)
            target.draw(*button);
    }

} // namespace ng