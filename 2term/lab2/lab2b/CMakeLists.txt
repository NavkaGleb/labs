cmake_minimum_required(VERSION 3.15)
project(lab2b)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5 COMPONENTS system window graphics)

add_executable(lab2b main.cpp Modes/Core.cpp Modes/Core.hpp Entities/Player.cpp Entities/Player.hpp Entities/Entity.cpp Entities/Entity.hpp States/GameState.cpp States/GameState.hpp States/State.cpp States/State.hpp Components/MovementComponent.cpp Components/MovementComponent.hpp Map/Map.cpp Map/Map.hpp Entities/Enemy.cpp Entities/Enemy.hpp Weapons/Bullet.cpp Weapons/Bullet.hpp Components/AnimationComponent.cpp Components/AnimationComponent.hpp Gui/EntityGui.cpp Gui/EntityGui.hpp Entities/Box.cpp Entities/Box.hpp Inventory/LifeCircle.cpp Inventory/LifeCircle.hpp Gui/MessageBox.cpp Gui/MessageBox.hpp)
target_link_libraries(lab2b sfml-system sfml-window sfml-graphics)