#pragma once

#include <future>

#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCamera.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>

namespace Lab03 {

    class SortLayer : public Ziben::Layer {
    public:
        SortLayer();
        ~SortLayer() noexcept override;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Ziben::Event& event) override;
        void OnUpdate(const Ziben::TimeStep& ts) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        struct Quad {
            std::size_t Index    = 0;
            glm::vec2   Position = glm::vec2(0.0f);
            glm::vec2   Size     = glm::vec2(1.0f);
            glm::vec4   Color    = glm::vec4(1.0f);

            inline bool operator <(const Quad& other) const { return Index < other.Index; }
        };

    private:
        using QuadContainer = std::vector<Quad>;
        using QuadIterator  = QuadContainer::iterator;

        using SortFunction  = void (SortLayer::*)(QuadIterator, QuadIterator);

    private:
        bool OnWindowResized(Ziben::WindowResizedEvent& event);

        void InitQuads();
        void ShuffleQuads();
        void UpdateQuads();
        void SwapQuads(Quad& lhs, Quad& rhs);

        template <SortFunction SortFunction>
        void Sort();

        void BubbleSortQuads(QuadIterator begin, QuadIterator end);
        void SelectionSortQuads(QuadIterator begin, QuadIterator end);
        void InsertionSortQuads(QuadIterator begin, QuadIterator end);
        void ShellSortQuads(QuadIterator begin, QuadIterator end);
        void QuickSortQuads(QuadIterator begin, QuadIterator end);
        void ParallelQuickSortQuads(QuadIterator begin, QuadIterator end);
        void MergeSortQuads(QuadIterator begin, QuadIterator end);
        void BottomUpMergeSortQuads(QuadIterator begin, QuadIterator end);
        void ParallelMergeSortQuads(QuadIterator begin, QuadIterator end);

    private:
        Ziben::OrthographicCamera      m_Camera;
        Ziben::Ref<Ziben::FrameBuffer> m_FrameBuffer;
        glm::vec<2, uint32_t>          m_ViewportSize;

        std::size_t                    m_QuadCount;
        QuadContainer                  m_Quads;
        glm::vec4                      m_BeginColor;
        glm::vec4                      m_EndColor;

        bool                           m_IsSorted;

        std::future<void>              m_SortFuture;
        int                            m_DelayTime;

    }; // class SortLayer

    template <SortLayer::SortFunction SortFunction>
    void SortLayer::Sort() {
        m_SortFuture = std::async(std::launch::async, [&] {
            (this->*SortFunction)(m_Quads.begin(), m_Quads.end());
            m_IsSorted = true;
        });
    }

} // namespace Lab03