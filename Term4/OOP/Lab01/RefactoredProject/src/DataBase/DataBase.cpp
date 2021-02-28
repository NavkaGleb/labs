#include "DataBase.hpp"

namespace RefactoredProject {

    DataBase_Impl::~DataBase_Impl() noexcept {
        for (auto& [hash, entities] : m_Entities) {
            for (auto& entity : entities) {
                entity.reset();
            }

        }
        std::cout << "DataBase_Impl dtor" << std::endl;
    }

} // namespace RefactoredProject