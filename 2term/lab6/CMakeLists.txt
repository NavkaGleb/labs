cmake_minimum_required(VERSION 3.16)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    lab6
    main.cpp
    Containers/List.hpp
    Containers/Container.hpp
    OrderedLists/OrderedList.hpp
    OrderedLists/LOrderedList.hpp
    OrderedLists/AOrderedList.hpp
    Containers/Array.hpp
    Random/rand.cpp Random/rand.hpp
    Trees/Tree.hpp
    Trees/BST.hpp
    OrderedLists/BSTOrderedList.hpp
    Trees/AVLTree.hpp
    OrderedLists/AVLOrderedList.hpp
    Trees/TwoThreeTree.hpp)