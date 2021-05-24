#pragma once

#include <future>
#include <atomic>

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

        using ShuffleFunction = void (SortLayer::*)();
        using SortFunction    = void (SortLayer::*)(QuadIterator, QuadIterator);

    private:
        bool OnWindowResized(Ziben::WindowResizedEvent& event);

        void InitQuads();
        void UpdateQuads();
        void SwapQuads(Quad& lhs, Quad& rhs);

        void RandomShuffleQuads();
        void ReverseQuads();

        template <typename Function, typename... Args>
        decltype(auto) AsyncRun(Function&& function, Args&&... args);

        template <ShuffleFunction ShuffleFunction>
        void Shuffle();

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

        auto PartitionQuads(QuadIterator begin, QuadIterator end) -> QuadIterator;
        void MergeQuads(QuadIterator begin, QuadIterator middle, QuadIterator end);

    private:
        Ziben::OrthographicCamera      m_Camera;
        Ziben::Ref<Ziben::FrameBuffer> m_FrameBuffer;
        glm::vec<2, uint32_t>          m_ViewportSize;

        std::size_t                    m_QuadCount;
        QuadContainer                  m_Quads;
        glm::vec4                      m_BeginColor;
        glm::vec4                      m_EndColor;

        std::future<void>              m_SortFuture;
        std::atomic_bool               m_IsRunning;
        std::atomic_bool               m_IsSorted;
        std::atomic_uint32_t           m_AsyncTasks;
        uint32_t                       m_DelayTime;

    }; // class SortLayer

    template <typename Function, typename... Args>
    decltype(auto) SortLayer::AsyncRun(Function&& function, Args&&... args) {
        return std::async(std::launch::async, [
            &,
            function = std::forward<Function>(function),
            ...args  = std::forward<Args>(args)
        ] {
            ++m_AsyncTasks;
            function(std::forward<Args>(args)...);
            --m_AsyncTasks;
        });
    }

    template <SortLayer::ShuffleFunction ShuffleFunction>
    void SortLayer::Shuffle() {
        if (m_IsRunning)
            return;

        m_SortFuture = AsyncRun([&] {
            m_IsRunning = true;
            m_IsSorted  = false;

            (this->*ShuffleFunction)();

            m_IsRunning = false;
        });
    }

    template <SortLayer::SortFunction SortFunction>
    void SortLayer::Sort() {
        if (m_IsSorted || m_IsRunning)
            return;

        m_SortFuture = AsyncRun([&] {
            m_IsRunning = true;

            (this->*SortFunction)(m_Quads.begin(), m_Quads.end());

            m_IsSorted  = true;
            m_IsRunning = false;
        });
    }

} // namespace Lab03