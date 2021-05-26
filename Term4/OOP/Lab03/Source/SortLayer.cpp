#include "SortLayer.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>

#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application.hpp"

namespace Lab03 {

    namespace Internal {

        static void Sleep(uintmax_t ms) {
            auto end = clock() + ms * CLOCKS_PER_SEC / 1000;
            while (clock() < end);
//            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

    } // namespace Internal

    SortLayer::SortLayer()
        : Ziben::Layer("SortLayer")
        , m_ViewportSize(0)
        , m_Camera(
            0,
            static_cast<float>(Application::Get().GetWindow().GetWidth()),
            static_cast<float>(Application::Get().GetWindow().GetHeight()),
            0
        )
        , m_BeginColor(222.0f / 255.0f, 222.0f / 255.0f, 222.0f / 255.0f, 1.0f)
        , m_EndColor(90.0f / 255.0f, 67.0f / 255.0f, 177.0f / 255.0f, 1.0f)
        , m_IsRunning(false)
        , m_IsSorted(true)
        , m_AsyncTasks(1)
        , m_DelayTime(1)
        , m_ApplicationState(ApplicationState::Init) {}

    SortLayer::~SortLayer() noexcept {

    }

    void SortLayer::OnAttach() {
        // Init FrameBuffer
        Ziben::FrameBufferSpecification specification;

        specification.Attachments = {
            { Ziben::FrameBufferTextureFormat::RGBA8 },
            { Ziben::FrameBufferTextureFormat::Depth }
        };

        specification.Width  = 1280;
        specification.Height = 720;

        m_FrameBuffer = Ziben::FrameBuffer::Create(std::move(specification));

        // Init ParticleSystem
        m_ParticleSystem.SetMaxParticleCount(3'000);

        // Init Particle
        m_Particle.ColorBegin        = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
        m_Particle.ColorEnd          = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
        m_Particle.SizeBegin         = 30.0f,
        m_Particle.SizeVariation     = 2.0f,
        m_Particle.SizeEnd           = 0.0f;
        m_Particle.LifeTime          = 3.5f;
        m_Particle.Velocity          = { 0.0f,   0.0f };
        m_Particle.VelocityVariation = { 50.0f, 50.0f };
        m_Particle.Position          = { 0.0f,   0.0f };

        // Init ShuffleAlgorithm
        m_ShuffleAlgorithms[ShuffleType::RandomShuffle] = CreateShuffleAlgorithm(ShuffleType::RandomShuffle);
        m_ShuffleAlgorithms[ShuffleType::Reverse]       = CreateShuffleAlgorithm(ShuffleType::Reverse);

        // Init SortAlgorithm
        m_SortAlgorithms[SortType::BubbleSort]        = CreateSortAlgorithm(SortType::BubbleSort);
        m_SortAlgorithms[SortType::SelectionSort]     = CreateSortAlgorithm(SortType::SelectionSort);
        m_SortAlgorithms[SortType::InsertionSort]     = CreateSortAlgorithm(SortType::InsertionSort);
        m_SortAlgorithms[SortType::ShellSort]         = CreateSortAlgorithm(SortType::ShellSort);
        m_SortAlgorithms[SortType::QuickSort]         = CreateSortAlgorithm(SortType::QuickSort);
        m_SortAlgorithms[SortType::ParallelQuickSort] = CreateSortAlgorithm(SortType::ParallelQuickSort);
        m_SortAlgorithms[SortType::MergeSort]         = CreateSortAlgorithm(SortType::MergeSort);
        m_SortAlgorithms[SortType::BottomUpMergeSort] = CreateSortAlgorithm(SortType::BottomUpMergeSort);
        m_SortAlgorithms[SortType::ParallelMergeSort] = CreateSortAlgorithm(SortType::ParallelMergeSort);

        // Register Observers
        for (const auto& [type, algorithm] : m_ShuffleAlgorithms)
            RegisterObserver(algorithm);

        for (const auto& [type, algorithm] : m_SortAlgorithms)
            RegisterObserver(algorithm);

        // Init Quads
        InitQuads(50);
        UpdateQuads();

        // Init ApplicationState
        m_ApplicationState = ApplicationState::Running;

        // Notify All Observers
        NotifyObservers(reinterpret_cast<const void*>(&m_ApplicationState));
    }

    void SortLayer::OnDetach() {
        if (m_SortFuture.valid())
            m_SortFuture.wait();

        for (auto& [type, algorithm] : m_SortAlgorithms)
            delete algorithm;
    }

    void SortLayer::OnEvent(Ziben::Event& event) {
        Ziben::EventDispatcher dispatcher(event);

        dispatcher.Dispatch<Ziben::WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
        dispatcher.Dispatch<Ziben::WindowClosedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowClosed));
    }

    void SortLayer::OnUpdate(const Ziben::TimeStep& ts) {
        // Resize
        if (auto specification = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0 &&
            m_ViewportSize.y > 0 && (
                specification.Width  != m_ViewportSize.x ||
                specification.Height != m_ViewportSize.y
            )
        ) {
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

            // Update CameraProjection
            m_Camera.SetProjection(0, static_cast<float>(m_ViewportSize.x), static_cast<float>(m_ViewportSize.y), 0);

            // Update Quads
            UpdateQuads();
        }

        m_ParticleSystem.OnUpdate(ts);
    }

    void SortLayer::OnRender() {
        Ziben::Renderer2D::ResetStatistics();

        Ziben::FrameBuffer::Bind(m_FrameBuffer);
        {
            Ziben::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            Ziben::RenderCommand::Clear();

            Ziben::Renderer2D::BeginScene(m_Camera);
            {
                for (const auto& quad : m_Quads)
                    Ziben::Renderer2D::DrawQuad(quad.Position, quad.Size, quad.Color);
            }
            Ziben::Renderer2D::EndScene();

            // ParticleSystem Render
            m_ParticleSystem.OnRender(m_Camera);
        }
        Ziben::FrameBuffer::Unbind();
    }

    void SortLayer::OnImGuiRender() {
        static bool               dockspaceOpen   = true;
        static bool               fullscreen      = true;
        static bool               padding         = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags windowFlags              = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        if (!padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);
        {
            if (!padding)
                ImGui::PopStyleVar();

            if (fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO&    io    = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();

            float windowMinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            style.WindowMinSize.x = windowMinSizeX;

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Exit"))
                        Application::Get().Close();

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
            ImGui::Begin("Viewport");
            {
                auto viewportPanelSize = ImGui::GetContentRegionAvail();
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                ImGui::Image(
                    reinterpret_cast<void*>(m_FrameBuffer->GetColorAttachmentHandle()),
                    viewportPanelSize,
                    ImVec2(0, 1),
                    ImVec2(1, 0)
                );

                // ParticleSystem Update
                if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
                    auto viewportOffset    = ImGui::GetWindowPos();

                    ImVec2 windowPosition  = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };

                    auto positionX = ImGui::GetMousePos().x - windowPosition.x;
                    auto positionY = ImGui::GetMousePos().y - windowPosition.y;

                    m_Particle.Position = { positionX, positionY };

                    for (int i = 0; i < 13; ++i)
                        m_ParticleSystem.Push(m_Particle);
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::Begin("Sorting");
            {
                ImVec2 buttonSize = { ImGui::GetContentRegionAvailWidth(), 0.0f };

                if (ImGui::Button("RandomShuffle", buttonSize))
                    Shuffle<ShuffleType::RandomShuffle>();

                if (ImGui::Button("Reverse", buttonSize))
                    Shuffle<ShuffleType::Reverse>();

                ImGui::Separator();

                if (ImGui::Button("BubbleSort", buttonSize))
                    Sort<SortType::BubbleSort>();

                if (ImGui::Button("SelectionSort", buttonSize))
                    Sort<SortType::SelectionSort>();

                if (ImGui::Button("InsertionSort", buttonSize))
                    Sort<SortType::InsertionSort>();

                if (ImGui::Button("ShellSort", buttonSize))
                    Sort<SortType::ShellSort>();

                if (ImGui::Button("QuickSort", buttonSize))
                    Sort<SortType::QuickSort>();

                if (ImGui::Button("ParallelQuickSort", buttonSize))
                    Sort<SortType::ParallelQuickSort>();

                if (ImGui::Button("MergeSort", buttonSize))
                    Sort<SortType::MergeSort>();

                if (ImGui::Button("BottomUpMergeSort", buttonSize))
                    Sort<SortType::BottomUpMergeSort>();

                if (ImGui::Button("ParallelMergeSort", buttonSize))
                    Sort<SortType::ParallelMergeSort>();
            }
            ImGui::End();

            ImGui::Begin("SortProps");
            {
                ImGui::Text("AsyncTasks: %d", (uint32_t)m_AsyncTasks);
                ImGui::Text("IsRunning: %d", (bool)m_IsRunning);
                ImGui::Text("IsSorted: %s", m_IsSorted ? "True" : "False");
                ImGui::Text("QuadCount: %llu", m_Quads.size());

                ImGui::DragInt("Delay Time", reinterpret_cast<int*>(&m_DelayTime), 0.3f, 1, 1000);

                if (ImGui::ColorEdit4("BeginColor", glm::value_ptr(m_BeginColor)))
                    UpdateQuads();

                if (ImGui::ColorEdit4("EndColor", glm::value_ptr(m_EndColor)))
                    UpdateQuads();

                if (std::size_t quadCount = m_Quads.size();
                    m_IsSorted && ImGui::DragInt("QuadCount", reinterpret_cast<int*>(&quadCount), 0.3f, 2, 2000)
                ) {
                    UpdateQuads(quadCount);
                }
            }
            ImGui::End();

            ImGui::Begin("ParticleProps");
            {
                ImGui::ColorEdit4("ColorBegin", glm::value_ptr(m_Particle.ColorBegin));
                ImGui::ColorEdit4("ColorEnd", glm::value_ptr(m_Particle.ColorEnd));

                ImGui::DragFloat("SizeBegin", &m_Particle.SizeBegin, 0.2f, 1.0f, 100.0f);
                ImGui::DragFloat("LifeTime", &m_Particle.LifeTime, 0.2f, 0.1f, 6.0f);
                ImGui::DragFloat2("VelocityVariation", glm::value_ptr(m_Particle.VelocityVariation), 0.1f, 0.0f, 100.0f);
            }
            ImGui::End();

            ImGui::Begin("Stats");
            {
                const auto& statistics = Ziben::Renderer2D::GetStatistics();

                ImGui::Text("Renderer2D Statistics");
                ImGui::Text("Draw Calls: %d",   statistics.DrawCalls);
                ImGui::Text("Quad Count: %d",   statistics.QuadCount);
                ImGui::Text("Vertex Count: %d", statistics.QuadCount * 4);
                ImGui::Text("Index Count: %d",  statistics.QuadCount * 6);

                ImGui::Separator();

                ImGui::Text("Application");
                ImGui::Text("FrameTime: %0.3f", 1000.0f / ImGui::GetIO().Framerate);
                ImGui::Text("FrameRate: %0.1f", ImGui::GetIO().Framerate);
            }
            ImGui::End();
        }
        ImGui::End();
    }

    bool SortLayer::OnWindowResized(Ziben::WindowResizedEvent& event) {
        m_Camera.SetProjection(
            0,
            static_cast<float>(m_ViewportSize.x),
            static_cast<float>(m_ViewportSize.y),
            0
        );

        UpdateQuads();

        return true;
    }

    bool SortLayer::OnWindowClosed(Ziben::WindowClosedEvent& event) {
        NotifyObservers(reinterpret_cast<const void*>(&(m_ApplicationState = ApplicationState::Shutdown)));

        return true;
    }

    ControllableAlgorithm<QuadIterator>* SortLayer::CreateShuffleAlgorithm(ShuffleType type) {
        auto Swap  = [&](Quad& lhs, Quad& rhs) { SwapQuads(lhs, rhs); };

        switch (type) {
            case ShuffleType::RandomShuffle: return new RandomShuffleAlgorithm<QuadIterator>(Swap);
            case ShuffleType::Reverse:       return new ReverseAlgorithm<QuadIterator>(Swap);

            default: break;
        }

        throw std::invalid_argument("Lab03::SortLayer::CreateShuffleAlgorithm: Unknown shuffle type");
    }

    ControllableAlgorithm<QuadIterator>* SortLayer::CreateSortAlgorithm(SortType type) {
        auto Swap  = [&](Quad& lhs, Quad& rhs) { SwapQuads(lhs, rhs); };
        auto Async = [&](const std::function<void()>& function) { return AsyncRun(function); };

        switch (type) {
            case SortType::BubbleSort:        return new BubbleSortAlgorithm<QuadIterator>(Swap);
            case SortType::SelectionSort:     return new SelectionSortAlgorithm<QuadIterator>(Swap);
            case SortType::InsertionSort:     return new InsertionSortAlgorithm<QuadIterator>(Swap);
            case SortType::ShellSort:         return new ShellSortAlgorithm<QuadIterator>(Swap);
            case SortType::QuickSort:         return new QuickSortAlgorithm<QuadIterator>(Swap, m_SortAlgorithms[SortType::InsertionSort]);
            case SortType::ParallelQuickSort: return new ParallelQuickSortAlgorithm<QuadIterator, decltype(Async)>(Swap, Async, m_SortAlgorithms[SortType::InsertionSort]);
            case SortType::MergeSort:         return new MergeSortAlgorithm<QuadIterator>(Swap, m_SortAlgorithms[SortType::InsertionSort]);
            case SortType::BottomUpMergeSort: return new BottomUpMergeSortAlgorithm<QuadIterator>(Swap, m_SortAlgorithms[SortType::InsertionSort]);
            case SortType::ParallelMergeSort: return new ParallelMergeSortAlgorithm<QuadIterator, decltype(Async)>(Swap, Async, m_SortAlgorithms[SortType::InsertionSort]);

            default: break;
        }

        throw std::invalid_argument("Lab03::SortLayer::CreateSortAlgorithm: Unknown sort type");
    }

    void SortLayer::InitQuads(std::size_t count) {
        std::size_t prevCount = m_Quads.size();

        m_Quads.resize(count);

        for (std::size_t i = prevCount; i < m_Quads.size(); ++i)
            m_Quads[i].Index = i;
    }

    void SortLayer::UpdateQuads(std::size_t count) {
        if (count != m_Quads.size() && count != 0)
           InitQuads(count);

        auto width   = static_cast<float>(m_ViewportSize.x);
        auto height  = static_cast<float>(m_ViewportSize.y);

        float scaleX = width  / static_cast<float>(m_Quads.size());
        float scaleY = height / static_cast<float>(m_Quads.size());

        for (std::size_t i = 0; i < m_Quads.size(); ++i) {
            m_Quads[i].Size     = { scaleX, scaleY * static_cast<float>(m_Quads[i].Index + 1) };
            m_Quads[i].Position = { scaleX * static_cast<float>(i + 1) - m_Quads[i].Size.x / 2.0f, height - m_Quads[i].Size.y / 2.0f };
            m_Quads[i].Color    = glm::lerp(m_BeginColor, m_EndColor, m_Quads[i].Size.y / height);
        }
    }

    void SortLayer::SwapQuads(Quad& lhs, Quad& rhs) {
        std::swap(lhs.Index, rhs.Index);
        UpdateQuads();
        Internal::Sleep(m_DelayTime);
    }

} // namespace Lab03