#include "MainMenuState.hpp"

#include <iostream>
#include <functional>
#include <vector>

#include "MandelbrotState.hpp"

namespace ng {

    // constructor / destructor
    MainMenuState::MainMenuState(sf::RenderWindow& window)
        : m_window(window) {

        loadFonts();

        initButtons();
    }

    // public methods
    void MainMenuState::mouseButtonPressed(const sf::Event& event) {
        if (event.mouseButton.button == sf::Mouse::Left && m_buttons["Mandelbrot Set"]->isHovered()) {
            std::cout << "a" << std::endl;
            State::getStateStack().push(new MandelbrotState);
        }
    }

    void MainMenuState::update(const float& ftime) {
        for (auto&& [name, button] : m_buttons)
            button->update(sf::Mouse::getPosition(m_window));
    }

    void MainMenuState::render(sf::RenderTarget& target) {
        for (const auto& [name, button] : m_buttons)
            target.draw(*button);
    }

    // member methods
    void MainMenuState::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("MainMenuState::loadFonts: failed to load 'Baloo2-Medium.ttf'");
    }

    void MainMenuState::initButtons() {
        sf::Color backgroundIdleColor = sf::Color(30, 50, 150, 200);
        sf::Color backgroundHoverColor = sf::Color(30, 40, 170, 220);
        sf::Color backgroundActiveColor = sf::Color(30, 40, 200, 230);
        std::vector<std::string> buttonText(3);

        buttonText[0] = "Mandelbrot Set";
        buttonText[1] = "Fluid Simulation";
        buttonText[2] = "Double-Pendulum Simulation";

        for (std::size_t i = 0; i < buttonText.size(); ++i) {
            auto button = std::make_unique<gui::Button>();

            button->setFont(m_font);
            button->setCharacterSize(25);
            button->setText(buttonText[i]);
            button->setTextHorizontalAlign(gui::Button::HorizontalAlign::Center);
            button->setTextVerticalAlign(gui::Button::VerticalAlign::Center);
            button->setTextColor(sf::Color::White);
            button->setSize(sf::Vector2f(400.0f, 50.0f));
            button->setPosition(
                (m_window.getSize().x - button->getSize().x) / 2.0f,
                static_cast<float>(i + 1) * 80.0f + 200.0f
            );
            button->setBackgroundIdleColor(backgroundIdleColor);
            button->setBackgroundHoverColor(backgroundHoverColor);
            button->setBackgroundActiveColor(backgroundActiveColor);
            m_buttons[button->getText()] = std::move(button);
        }
    }

} // namespace ng