#include "Application/Application.hpp"

int main() {
    RefactoredProject::Application::Create().Run();
    RefactoredProject::Application::Destroy();
}
