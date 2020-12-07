#include "FluidState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    FluidState::FluidState()
        : m_fluid(State::getContext().window->getSize().x / 2, State::getContext().window->getSize().y / 2, 2),
          m_isPressed(false),
          m_isPaused(false),
          m_showSliders(true) {

        loadFonts();
        initSliders();
    }

    // public methods
    void FluidState::mouseMoved(const sf::Event& event) {
        int x = std::min<int>(State::getContext().window->getSize().x, std::max(0, event.mouseMove.x));
        int y = std::min<int>(State::getContext().window->getSize().y, std::max(0, event.mouseMove.y));

        if (m_showSliders) {
            if (m_sliders[SliderRole::Pressure].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::Pressure].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().pressure
                );
            } else if (m_sliders[SliderRole::Vorticity].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::Vorticity].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().vorticity
                );
            } else if (m_sliders[SliderRole::VelocityDiffusion].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::VelocityDiffusion].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().velocityDiffusion
                );
            } else if (m_sliders[SliderRole::ColorDiffusion].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::ColorDiffusion].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().colorDiffusion
                );
            } else if (m_sliders[SliderRole::DensityDiffusion].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::DensityDiffusion].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().densityDiffusion
                );
            } else if (m_sliders[SliderRole::ForceScale].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::ForceScale].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().forceScale
                );
            } else if (m_sliders[SliderRole::BloomIntense].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::BloomIntense].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().bloomIntense
                );
            } else if (m_sliders[SliderRole::Radius].slider->isPressed()) {
                updateSliderValue(
                    m_sliders[SliderRole::Radius].slider,
                    static_cast<float>(x),
                    m_fluid.getConfig().radius
                );
            }
        }

        std::swap(m_pos1, m_pos2);
        m_pos2 = { static_cast<int>(x / m_fluid.getScale()), static_cast<int>(y / m_fluid.getScale()) };
    }

    void FluidState::mouseButtonPressed(const sf::Event& event) {
        if (!isSliderHovered() && event.mouseButton.button == sf::Mouse::Left) {
            m_pos1 = {
                static_cast<int>(event.mouseButton.x / m_fluid.getScale()),
                static_cast<int>(event.mouseButton.y / m_fluid.getScale())
            };
            m_isPressed = true;
        }

        if (event.mouseButton.button == sf::Mouse::Right)
            m_isPaused = !m_isPaused;
    }

    void FluidState::mouseButtonReleased(const sf::Event& event) {
        m_isPressed = false;
    }

    void FluidState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();

        if (event.key.code == sf::Keyboard::S)
            m_showSliders = !m_showSliders;
    }

    void FluidState::update(const float& dt) {
        updateSliders();
        updateMousePosition();
        updateFluid(dt);
    }

    void FluidState::render(sf::RenderTarget& target) const {
        target.draw(m_fluid);

        if (m_showSliders) {
            for (const auto& [role, item] : m_sliders) {
                target.draw(item.text);
                target.draw(*item.slider);
            }
        }
    }

    // member methods
    void FluidState::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("FluidState::loadFonts: failed to load font");
    }

    void FluidState::initSliders() {
        m_sliders[SliderRole::Pressure].slider          = std::make_unique<gui::Slider>(0.0f,  1.8f,     0.1f);
        m_sliders[SliderRole::Vorticity].slider         = std::make_unique<gui::Slider>(0.0f,  150.0f,   10.0f);
        m_sliders[SliderRole::VelocityDiffusion].slider = std::make_unique<gui::Slider>(0.0f,  4.0f,     0.1f);
        m_sliders[SliderRole::ColorDiffusion].slider    = std::make_unique<gui::Slider>(0.0f,  2.0f,     0.1f);
        m_sliders[SliderRole::DensityDiffusion].slider  = std::make_unique<gui::Slider>(0.0f,  3.0f,     0.1f);
        m_sliders[SliderRole::ForceScale].slider        = std::make_unique<gui::Slider>(0.0f,  10000.0f, 500.0f);
        m_sliders[SliderRole::BloomIntense].slider      = std::make_unique<gui::Slider>(0.0f,  10.0f,     0.5f);
        m_sliders[SliderRole::Radius].slider            = std::make_unique<gui::Slider>(50.0f, 400.0f,   10.0f);

        auto sliderIterator = m_sliders.begin();

        for (std::size_t i = 0; i < m_sliders.size(); ++i) {
            auto& [role, item] = *(sliderIterator);

            switch (role) {
                case SliderRole::Pressure:
                    item.text.setString("Pressure");
                    item.slider->setValue(m_fluid.getConfig().pressure);
                    break;

                case SliderRole::Vorticity:
                    item.text.setString("Vorticity");
                    item.slider->setValue(m_fluid.getConfig().vorticity);
                    break;

                case SliderRole::VelocityDiffusion:
                    item.text.setString("VelocityDiffusion");
                    item.slider->setValue(m_fluid.getConfig().velocityDiffusion);
                    break;

                case SliderRole::ColorDiffusion:
                    item.text.setString("ColorDiffusion");
                    item.slider->setValue(m_fluid.getConfig().colorDiffusion);
                    break;

                case SliderRole::DensityDiffusion:
                    item.text.setString("DensityDiffusion");
                    item.slider->setValue(m_fluid.getConfig().densityDiffusion);
                    break;

                case SliderRole::ForceScale:
                    item.text.setString("ForceScale");
                    item.slider->setValue(m_fluid.getConfig().forceScale);
                    break;

                case SliderRole::BloomIntense:
                    item.text.setString("BloomIntense");
                    item.slider->setValue(m_fluid.getConfig().bloomIntense);
                    break;

                case SliderRole::Radius:
                    item.text.setString("Radius");
                    item.slider->setValue(static_cast<float>(m_fluid.getConfig().radius));
                    break;
            }

            // text
            item.text.setFont(m_font);
            item.text.setCharacterSize(15);
            item.text.setFillColor(sf::Color(79, 125, 232, 255));
            item.text.setPosition(350.0f, std::floor(i * 30.0f + 20.f));
            
            // slider
            item.slider->setPosition(500.0f, i * 30.0f + 30.0f);
            item.slider->setSize(sf::Vector2f(250.0f, 5.0f));
            item.slider->setPointerRadius(10.0f);
            item.slider->setBackgroundColor(sf::Color(122, 122, 122, 255));
            item.slider->setForegroundColor(sf::Color(98, 94, 213));
            item.slider->setPointerIdleColor(sf::Color(150, 150, 150, 255));
            item.slider->setPointerHoverColor(sf::Color(200, 200, 200, 255));
            item.slider->setPointerActiveColor(sf::Color(250, 250, 250, 255));

            ++sliderIterator;
        }
    }

    bool FluidState::isSliderHovered() const {
        return std::ranges::any_of(m_sliders.begin(), m_sliders.end(), [](const auto& p) {
            return p.second.slider->isHovered();
        });
    }

    bool FluidState::isSliderPressed() const {
        return std::ranges::any_of(m_sliders.begin(), m_sliders.end(), [](const auto& p) {
            return p.second.slider->isPressed();
        });
    }

    void FluidState::updateMousePosition() {
        if (std::abs(m_pos1.x - m_pos2.x) == 1 || std::abs(m_pos1.y - m_pos2.y) == 1)
            m_pos2 = m_pos1;
    }

    void FluidState::updateSliders() {
        if (!m_isPressed)
            for (auto& [role, item] : m_sliders)
                item.slider->update(sf::Mouse::getPosition(*State::getContext().window));
    }

    void FluidState::updateFluid(float dt) {
        if (!m_isPaused)
            m_fluid.update(dt, m_pos1, m_pos2, m_isPressed);
    }

} // namespace ng