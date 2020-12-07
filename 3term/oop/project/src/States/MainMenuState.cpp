#include "MainMenuState.hpp"

#include <iostream>
#include <functional>
#include <vector>

#include "MandelbrotState.hpp"
#include "FluidState.hpp"

namespace ng {

    // constructor / destructor
    MainMenuState::MainMenuState() {
        loadFonts();
        initButtons();
    }

    // public methods
    void MainMenuState::mouseButtonPressed(const sf::Event& event) {

    }

    void MainMenuState::mouseButtonReleased(const sf::Event& event) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (m_buttons[ButtonRole::MandelbrotSet]->isPressed())
                State::getStateStack().push(std::make_unique<MandelbrotState>());

            if (m_buttons[ButtonRole::FluidSimulation]->isPressed())
                State::getStateStack().push(std::make_unique<FluidState>());
        }
    }

    void MainMenuState::update(const float& dt) {
        for (auto& [name, button] : m_buttons)
            button->update(sf::Mouse::getPosition(*State::getContext().window));
    }

    void MainMenuState::render(sf::RenderTarget& target) const {
        for (const auto& [name, button] : m_buttons)
            target.draw(*button);
    }

    // member methods
    void MainMenuState::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("MainMenuState::loadFonts: failed to load 'Baloo2-Medium.ttf'");
    }

    void MainMenuState::initButtons() {
        sf::Color backgroundIdleColor   = sf::Color(30, 50, 150, 200);
        sf::Color backgroundHoverColor  = sf::Color(30, 40, 170, 220);
        sf::Color backgroundActiveColor = sf::Color(30, 40, 200, 230);

        std::cout << "init" << std::endl;

        m_buttons[ButtonRole::MandelbrotSet]   = std::make_unique<gui::Button>();
        m_buttons[ButtonRole::FluidSimulation] = std::make_unique<gui::Button>();

        auto buttonIterator = m_buttons.begin();

        for (std::size_t i = 0; i < m_buttons.size(); ++i) {
            auto& [role, button] = *buttonIterator;

            button->setFont(m_font);
            button->setCharacterSize(25);

            button->setTextHorizontalAlign(gui::Button::HorizontalAlign::Center);
            button->setTextVerticalAlign(gui::Button::VerticalAlign::Center);
            button->setTextColor(sf::Color::White);
            button->setSize(sf::Vector2f(400.0f, 50.0f));
            button->setPosition(
                (State::getContext().window->getSize().x - button->getSize().x) / 2.0f,
                static_cast<float>(i + 1) * 80.0f + 100.0f
            );
            button->setBackgroundIdleColor(backgroundIdleColor);
            button->setBackgroundHoverColor(backgroundHoverColor);
            button->setBackgroundActiveColor(backgroundActiveColor);

            switch (role) {
                case ButtonRole::MandelbrotSet:
                    button->setText("Mandelbrot Set");
                    break;

                case ButtonRole::FluidSimulation:
                    button->setText("Fluid Simulation");
                    break;
            }

            ++buttonIterator;
        }


    }

} // namespace ng