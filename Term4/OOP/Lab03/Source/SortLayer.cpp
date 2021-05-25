#include "SortLayer.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>

#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application.hpp"
#include "Algorithm.hpp"

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
        , m_QuadCount(50)
        , m_BeginColor(222.0f / 255.0f, 222.0f / 255.0f, 222.0f / 255.0f, 1.0f)
        , m_EndColor(90.0f / 255.0f, 67.0f / 255.0f, 177.0f / 255.0f, 1.0f)
        , m_IsRunning(false)
        , m_IsSorted(true)
        , m_AsyncTasks(1)
        , m_DelayTime(1) {}

    SortLayer::~SortLayer() noexcept {
        if (m_SortFuture.valid())
            m_SortFuture.wait();
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
        m_ParticleSystem.SetMaxParticleCount(10'000);

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

        // Init Sort
//        m_BubbleSortQuads = BubbleSortQuadsFunctor;

        // Subscribe
        RegisterObserver(&m_BubbleSortQuads);

        // Init Quads();
        UpdateQuads();
    }

    void SortLayer::OnDetach() {

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
                    Shuffle<&SortLayer::RandomShuffleQuads>();

                if (ImGui::Button("Reverse", buttonSize))
                    Shuffle<&SortLayer::ReverseQuads>();

                ImGui::Separator();

                if (ImGui::Button("BubbleSort", buttonSize)) {
                    m_SortFuture = AsyncRun([&] {
                        m_BubbleSortQuads(m_Quads.begin(), m_Quads.end(), [&](Quad& lhs, Quad& rhs) {
                            SwapQuads(lhs, rhs);
                        });
                    });
                }

//                    Sort<&SortLayer::BubbleSortQuads>();

                if (ImGui::Button("SelectionSort", buttonSize))
                    Sort<&SortLayer::SelectionSortQuads>();

                if (ImGui::Button("InsertionSort", buttonSize))
                    Sort<&SortLayer::InsertionSortQuads>();

                if (ImGui::Button("ShellSort", buttonSize))
                    Sort<&SortLayer::ShellSortQuads>();

                if (ImGui::Button("QuickSort", buttonSize))
                    Sort<&SortLayer::QuickSortQuads>();

                if (ImGui::Button("ParallelQuickSort", buttonSize))
                    Sort<&SortLayer::ParallelQuickSortQuads>();

                if (ImGui::Button("MergeSort", buttonSize))
                    Sort<&SortLayer::MergeSortQuads>();

                if (ImGui::Button("BottomUpMergeSort", buttonSize))
                    Sort<&SortLayer::BottomUpMergeSortQuads>();

                if (ImGui::Button("ParallelMergeSort", buttonSize))
                    Sort<&SortLayer::ParallelMergeSortQuads>();
            }
            ImGui::End();

            ImGui::Begin("SortProps");
            {
                ImGui::Text("AsyncTasks: %d", (uint32_t)m_AsyncTasks);
                ImGui::Text("IsRunning: %d", (bool)m_IsRunning);
                ImGui::Text("IsSorted: %s", m_IsSorted ? "True" : "False");
                ImGui::Text("QuadCount: %llu", m_QuadCount);

                ImGui::DragInt("Delay Time", reinterpret_cast<int*>(&m_DelayTime), 0.3f, 1, 1000);

                if (ImGui::ColorEdit4("BeginColor", glm::value_ptr(m_BeginColor)))
                    UpdateQuads();

                if (ImGui::ColorEdit4("EndColor", glm::value_ptr(m_EndColor)))
                    UpdateQuads();

                if (m_IsSorted && ImGui::DragInt("QuadCount", reinterpret_cast<int*>(&m_QuadCount), 0.3f, 2, 2000))
                    UpdateQuads();
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
        NotifyObservers(reinterpret_cast<const void*>(m_QuadCount));

        return true;
    }

    void SortLayer::InitQuads() {
        std::size_t prevCount = m_Quads.size();

        m_Quads.resize(m_QuadCount);

        for (std::size_t i = prevCount; i < m_Quads.size(); ++i)
            m_Quads[i].Index = i;
    }

    void SortLayer::UpdateQuads() {
        if (m_QuadCount != m_Quads.size())
           InitQuads();

        auto width   = static_cast<float>(m_ViewportSize.x);
        auto height  = static_cast<float>(m_ViewportSize.y);

        float scaleX = width  / static_cast<float>(m_QuadCount);
        float scaleY = height / static_cast<float>(m_QuadCount);

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

    void SortLayer::RandomShuffleQuads() {
        for (std::size_t i = m_Quads.size() - 1; i > 0; --i) {
            SwapQuads(m_Quads[i], m_Quads[Ziben::Random::GetFromRange<decltype(i)>(0, i)]);
            Internal::Sleep(m_DelayTime);
        }
    }

    void SortLayer::ReverseQuads() {
        for (std::size_t i = 0; i < m_QuadCount / 2; ++i) {
            SwapQuads(m_Quads[i], m_Quads[m_QuadCount - i - 1]);
            Internal::Sleep(m_DelayTime);
        }
    }

    void SortLayer::SelectionSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::InsertionSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::ShellSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::QuickSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::ParallelQuickSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::MergeSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::BottomUpMergeSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::ParallelMergeSortQuads(QuadIterator begin, QuadIterator end) {

    }

    auto SortLayer::PartitionQuads(QuadIterator begin, QuadIterator end) -> QuadIterator {

    }

    void SortLayer::MergeQuads(QuadIterator begin, QuadIterator middle, QuadIterator end) {

    }

} // namespace Lab03