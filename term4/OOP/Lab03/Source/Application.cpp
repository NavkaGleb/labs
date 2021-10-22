#include "Application.hpp"

#include "SortLayer.hpp"

namespace Lab03 {

    Application::Application()
        : Ziben::Application("Lab03", 1600, 700) {

        PushLayer(new SortLayer);
    }

} // namespace Lab03