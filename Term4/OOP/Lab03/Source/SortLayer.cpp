#include "SortLayer.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>

#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application.hpp"
#include "Algorithm.hpp"

namespace Lab03 {

    namespace Internal {

        static void Sleep(uintmax_t ms) {
            auto end = clock() + ms * CLOCKS_PER_SEC / 1000;
            while (clock() < end);
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
        , m_BeginColor(254.0f / 255.0f, 212.0f / 255.0f, 123.0f / 255.0f, 1.0f)
        , m_EndColor(254.0f / 255.0f, 109.0f / 255.0f, 41.0f / 255.0f, 1.0f)
        , m_IsSorted(true)
        , m_DelayTime(1) {}

    SortLayer::~SortLayer() noexcept {
        if (m_SortFuture.valid())
            m_SortFuture.wait();
    }

    void SortLayer::OnAttach() {
        Ziben::FrameBufferSpecification specification;

        specification.Attachments = {
            { Ziben::FrameBufferTextureFormat::RGBA8      },
            { Ziben::FrameBufferTextureFormat::Depth      }
        };

        specification.Width  = 1280;
        specification.Height = 720;

        m_FrameBuffer = Ziben::FrameBuffer::Create(std::move(specification));

        InitQuads();
        UpdateQuads();
    }

    void SortLayer::OnDetach() {

    }

    void SortLayer::OnEvent(Ziben::Event& event) {
        Ziben::EventDispatcher dispatcher(event);

        dispatcher.Dispatch<Ziben::WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
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
        }
    }

    void SortLayer::OnRender() {
        Ziben::FrameBuffer::Bind(m_FrameBuffer);
        Ziben::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Ziben::RenderCommand::Clear();

        Ziben::Renderer2D::BeginScene(m_Camera);
        {
            for (const auto& quad : m_Quads)
                Ziben::Renderer2D::DrawQuad(quad.Position, quad.Size, quad.Color);
        }
        Ziben::Renderer2D::EndScene();
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
            }
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::Begin("Sorting");
            {
                ImVec2 buttonSize = { ImGui::GetContentRegionAvailWidth(), 0.0f };

                if (ImGui::Button("Shuffle", buttonSize))
                    ShuffleQuads();

                if (ImGui::Button("BubbleSort", buttonSize))
                    Sort<&SortLayer::BubbleSortQuads>();

                if (ImGui::Button("SelectionSort", buttonSize))
                    Sort<&SortLayer::SelectionSortQuads>();

                if (ImGui::Button("InsertionSort", buttonSize))
                    Sort<&SortLayer::InsertionSortQuads>();

                if (ImGui::Button("ShellSort", buttonSize))
                    Sort<&SortLayer::ShellSortQuads>();

                if (ImGui::Button("QuickSort", buttonSize))
                    Sort<&SortLayer::QuickSortQuads>();

                if (ImGui::Button("MergeSort", buttonSize))
                    Sort<&SortLayer::MergeSortQuads>();
            }
            ImGui::End();

            ImGui::Begin("Settings");
            {
                ImGui::Text("Sorted: %s", m_IsSorted ? "True" : "False");
                ImGui::Text("QuadCount: %llu", m_QuadCount);

                ImGui::DragInt("Delay Time (us)", &m_DelayTime, 0.3f, 1, 1000);

                if (ImGui::ColorEdit4("BeginColor", glm::value_ptr(m_BeginColor)))
                    UpdateQuads();

                if (ImGui::ColorEdit4("EndColor", glm::value_ptr(m_EndColor)))
                    UpdateQuads();

                if (m_IsSorted && ImGui::DragInt("QuadCount", reinterpret_cast<int*>(&m_QuadCount), 0.3f, 2, 400))
                    UpdateQuads();
            }
            ImGui::End();
        }
        ImGui::End();
    }

    bool SortLayer::OnWindowResized(Ziben::WindowResizedEvent& event) {
        m_Camera.SetProjection(
            0,
            static_cast<float>(Application::Get().GetWindow().GetWidth()),
            static_cast<float>(Application::Get().GetWindow().GetHeight()),
            0
        );

        UpdateQuads();

        return true;
    }

    void SortLayer::InitQuads() {
        std::size_t prevCount = m_Quads.size();

        m_Quads.resize(m_QuadCount);

        for (std::size_t i = prevCount; i < m_Quads.size(); ++i)
            m_Quads[i].Index = i;
    }

    void SortLayer::ShuffleQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            m_IsSorted = false;

            for (auto i = m_Quads.size() - 1; i > 0; --i) {
                SwapQuads(m_Quads[i], m_Quads[Ziben::Random::GetFromRange<decltype(i)>(0, i)]);
                Internal::Sleep(m_DelayTime);
            }
        });
    }

    void SortLayer::UpdateQuads() {
        if (m_QuadCount != m_Quads.size())
           InitQuads();

        auto width   = static_cast<float>(Application::Get().GetWindow().GetWidth());
        auto height  = static_cast<float>(Application::Get().GetWindow().GetHeight());

        float scaleX = width  / static_cast<float>(m_QuadCount);
        float scaleY = height / static_cast<float>(m_QuadCount);

        for (std::size_t i = 0; i < m_Quads.size(); ++i) {
            m_Quads[i].Size     = { scaleX, scaleY * static_cast<float>(m_Quads[i].Index + 1) };
            m_Quads[i].Position = { scaleX * static_cast<float>(i + 1) - m_Quads[i].Size.x / 2.0f, height - m_Quads[i].Size.y / 2.0f };
            m_Quads[i].Color    = glm::lerp(m_EndColor, m_BeginColor, m_Quads[i].Size.y / height);
        }
    }

    void SortLayer::SwapQuads(Quad& lhs, Quad& rhs) {
        std::swap(lhs.Index, rhs.Index);
        UpdateQuads();
    }

    void SortLayer::BubbleSortQuads(QuadIterator begin, QuadIterator end) {
        for (auto [i, isSwapped] = std::make_pair(begin, false); i != end - 1; ++i) {
            isSwapped = false;

            for (auto j = begin; j != end - std::distance(begin, i) - 1; ++j) {
                if (*(j + 1) < *j) {
                    isSwapped = true;

                    SwapQuads(*(j + 1), *j);
                    Internal::Sleep(m_DelayTime);
                }
            }

            if (!isSwapped)
                break;
        }
    }

    void SortLayer::SelectionSortQuads(QuadIterator begin, QuadIterator end) {
        for (auto i = begin, min = QuadIterator(); i != end - 1; ++i) {
            min = i;

            for (auto j = i; j < end; ++j)
                if (*j < *min)
                    min = j;

            SwapQuads(*i, *min);
            Internal::Sleep(m_DelayTime);
        }
    }

    void SortLayer::InsertionSortQuads(QuadIterator begin, QuadIterator end) {
        for (auto i = begin + 1; i != end; ++i) {
            for (auto j = i; j != begin && *j < *(j - 1); --j) {
                SwapQuads(*j, *(j - 1));
                Internal::Sleep(m_DelayTime);
            }
        }
    }

    void SortLayer::ShellSortQuads(QuadIterator begin, QuadIterator end) {
        std::iterator_traits<QuadIterator>::difference_type h = 1;

        while (h < (end - begin) / 3)
            h = 3 * h + 1;

        while (h >= 1) {
            for (auto i = begin + h; i != end; ++i) {
                for (auto j = i; j >= begin + h && *j < *(j - h); j -= h) {
                    SwapQuads(*j, *(j - h));
                    Internal::Sleep(m_DelayTime);
                }
            }

            h = h / 3;
        }
    }

    void SortLayer::QuickSortQuads(QuadIterator begin, QuadIterator end) {
        static const auto Partition = [&](QuadIterator begin, QuadIterator end) {
            auto i = begin - 1;
            auto j = end;

            while (true) {
                while (*(++i) < *end)
                    if (i == end)
                        break;

                while (*end < *(--j))
                    if (j == begin)
                        break;

                if (j <= i)
                    break;

                SwapQuads(*i, *j);
                Internal::Sleep(m_DelayTime);
            }

            SwapQuads(*end, *i);

            return i;
        };

        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return InsertionSortQuads(begin, end);

        auto partition = Partition(begin, end - 1);

        QuickSortQuads(begin,         partition);
        QuickSortQuads(partition + 1, end);
    }

    void SortLayer::ParallelQuickSortQuads(QuadIterator begin, QuadIterator end) {

    }

    void SortLayer::MergeSortQuads(QuadIterator begin, QuadIterator end) {
        static const auto Merge = [&](QuadIterator begin, QuadIterator middle, QuadIterator end) {
            std::vector<Quad> temp(std::distance(begin, end));

            auto i = begin;
            auto j = middle;

            for (auto& k : temp) {
                if ((*i < *j && i < middle) || j >= end)
                    k = *(i++);
                else if ((*j < *i && j < end) || i >= middle)
                    k = *(j++);

                Internal::Sleep(m_DelayTime);
            }

            for (auto k = temp.begin(); k != temp.end(); ++k)
                *(begin + std::distance(temp.begin(), k)) = *k;

            UpdateQuads();
        };

        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return InsertionSortQuads(begin, end);

        auto middle = begin + std::distance(begin, end) / 2;

        MergeSortQuads(begin,  middle);
        MergeSortQuads(middle, end);

        Merge(begin, middle, end);
    }

} // namespace Lab03