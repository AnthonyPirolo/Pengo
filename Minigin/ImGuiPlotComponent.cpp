#include "ImGuiPlotComponent.h"
#include "imgui_plot.h"
#include "imgui_plot.cpp"
#include <math.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <chrono>

namespace dae
{
	ImGuiPlotComponent::ImGuiPlotComponent(GameObject* owner):
		BaseComponent(owner)
	{
	}

	void ImGuiPlotComponent::FixedUpdate(float deltaTime)
	{
		(void)deltaTime;
	}
	void ImGuiPlotComponent::Update()
	{
        GenerateData();
	}

	void ImGuiPlotComponent::LateUpdate()
	{

	}

	void ImGuiPlotComponent::Render() const
	{
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Exercise 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            if (!m_DrawExercise1)
            {
                if (ImGui::Button("Draw Exercise 1"))
                {
                    m_DrawExercise1 = true;
                }
            }
            else DrawExercise1();

            ImGui::Begin("Exercise 2", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            if (!m_DrawExercise2)
            {
                if (ImGui::Button("Draw GameObject"))
                {
                    m_DrawExercise2 = true;
                }
            }
            else DrawExercise2();
            if (!m_DrawExercise2Alt)
            {
                if (ImGui::Button("Draw GameObjectAlt"))
                {
                    m_DrawExercise2Alt = true;
                }
            }
            else DrawExercise2Alt();
            ImGui::End();
            ImGui::End();
        ImGui::Render();
	}

    void ImGuiPlotComponent::GenerateData() 
    {

        //Exercise 1
        if (m_DrawExercise1 && !m_Exercise1Generated)
        {
            const int size = 1 << 26;
            std::vector<int> arr(size, 1);
            int count = 0;

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
            {
                auto start = std::chrono::high_resolution_clock::now();

                for (int i = 0; i < size; i += stepsize)
                {
                    arr[i] *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                m_XData[count] = float(stepsize);
                m_YData1[count] = float(total);
                ++count;
            }
            m_Exercise1Generated = true;
        }

        //Exercise 2
        if (m_DrawExercise2 && !m_Exercise2Generated)
        {
            const int size = 1 << 26;
            std::vector<GameObject3D> arr(size);
            int count = 0;

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
            {
                auto start = std::chrono::high_resolution_clock::now();

                for (int i = 0; i < size; i += stepsize)
                {
                    arr[i].ID *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                m_XData[count] = float(stepsize);
                m_YData2[count] = float(total);
                ++count;
            }
            m_Exercise2Generated = true;
        }

        //Exercise 2 Alt
        if (m_DrawExercise2Alt && !m_Exercise2AltGenerated)
        {
            const int size = 1 << 26;
            std::vector<GameObject3DAlt> arr(size);
            int count = 0;

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
            {
                auto start = std::chrono::high_resolution_clock::now();

                for (int i = 0; i < size; i += stepsize)
                {
                    arr[i].ID *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                m_XData[count] = float(stepsize);
                m_YData3[count] = float(total);
                ++count;
            }
            m_Exercise2AltGenerated = true;
        }

    }

    void ImGuiPlotComponent::DrawExercise1() const
    {
        static const float* y_data[] = { m_YData1 };
        static uint32_t selection_start = 0, selection_length = 0;
        static const ImU32 colors[3]{ ImColor(255, 0, 0), ImColor(0, 0, 0), ImColor(0, 0, 0) };
        // Draw first plot with multiple sources
        ImGui::PlotConfig conf;
        conf.values.xs = m_XData;
        conf.values.count = 12;
        conf.values.ys = m_YData1;
        conf.values.ys_count = 1;
        conf.values.colors = colors;
        conf.scale.min = 0;
        conf.scale.max = m_YData1[0];
        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_x.size = 64;
        conf.grid_x.subticks = 1;
        conf.grid_y.show = true;
        conf.grid_y.size = 0.5f;
        conf.grid_y.subticks = 1;
        conf.selection.show = true;
        conf.selection.start = &selection_start;
        conf.selection.length = &selection_length;
        conf.frame_size = ImVec2(100, 100);
        ImGui::Plot("plot1", conf);

    }

    void ImGuiPlotComponent::DrawExercise2() const
    {
        static const float* y_data[] = { m_YData2 };
        static uint32_t selection_start = 0, selection_length = 0;
        static const ImU32 colors[3]{ ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
        // Draw first plot with multiple sources
        ImGui::PlotConfig conf;
        conf.values.xs = m_XData;
        conf.values.count = 12;
        conf.values.ys = m_YData2;
        conf.values.ys_count = 1;
        conf.values.colors = colors;
        conf.scale.min = 0;
        conf.scale.max = m_YData2[0];
        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_x.size = 64;
        conf.grid_x.subticks = 1;
        conf.grid_y.show = true;
        conf.grid_y.size = 0.5f;
        conf.grid_y.subticks = 1;
        conf.selection.show = true;
        conf.selection.start = &selection_start;
        conf.selection.length = &selection_length;
        conf.frame_size = ImVec2(100, 100);
        ImGui::Plot("plot2", conf);

    }

    void ImGuiPlotComponent::DrawExercise2Alt() const
    {
        static const float* y_data[] = { m_YData2, m_YData3 };
        static uint32_t selection_start = 0, selection_length = 0;
        static const ImU32 colors[3]{ ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };

        ImGui::PlotConfig conf;
        conf.values.xs = m_XData;
        conf.values.count = 12;
        conf.values.ys = m_YData3;
        conf.values.ys_count = 1;
        conf.values.colors = colors;
        conf.scale.min = 0;
        conf.scale.max = m_YData3[0];
        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_x.size = 64;
        conf.grid_x.subticks = 1;
        conf.grid_y.show = true;
        conf.grid_y.size = 0.5f;
        conf.grid_y.subticks = 1;
        conf.selection.show = true;
        conf.selection.start = &selection_start;
        conf.selection.length = &selection_length;
        conf.frame_size = ImVec2(100, 100);
        ImGui::Plot("plot3", conf);

        if (m_YData2 != nullptr)
        {
            conf.values.ys = nullptr;
            conf.selection.show = false;
            conf.scale.max = m_YData2[0];
            conf.values.ys_list = y_data;
            conf.values.ys_count = 2;

            ImGui::Plot("plot2&3", conf);
        }
    }
}