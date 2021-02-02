#ifndef ENTITYGUI_HPP
#define ENTITYGUI_HPP

#include "MessageBox.hpp"

namespace gui {

	class Bar {
	private:
		// variables
		int mvalue;																	// max value
		float mwidth;																// max width

		sf::RectangleShape backBar;													// back bar
		sf::RectangleShape innerBar;												// inner bar

	public:
		// constructor / destructor
		Bar(const int& mvalue, const sf::Color& fillColor, const float& x, const float& y,
			const float& width, const float& height);
		virtual ~Bar();

		// methods
		void update(const int& value);
		void render(sf::RenderTarget& target);

	};

}

#endif // ENTITYGUI_HPP
