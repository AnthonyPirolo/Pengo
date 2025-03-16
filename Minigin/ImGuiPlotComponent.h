//#pragma once
//#include "BaseComponent.h"
//
//namespace dae
//{
//	class ImGuiPlotComponent final : public BaseComponent
//	{
//	public:
//		//---------------------------------
//		//Game Related Functions
//		//---------------------------------
//		void FixedUpdate(float deltaTime) override;
//		void Update() override;
//		void LateUpdate() override;
//		void Render() const override;
//
//		//---------------------------------
//		//Constructor & Destructor
//		//---------------------------------
//		ImGuiPlotComponent(GameObject* owner);
//		virtual ~ImGuiPlotComponent() override = default;
//		ImGuiPlotComponent(const ImGuiPlotComponent& other) = delete;
//		ImGuiPlotComponent(ImGuiPlotComponent&& other) = delete;
//		ImGuiPlotComponent& operator=(const ImGuiPlotComponent& other) = delete;
//		ImGuiPlotComponent& operator=(ImGuiPlotComponent&& other) = delete;
//	private:
//
//		void DrawExercise1() const;
//		void DrawExercise2() const;
//		void DrawExercise2Alt() const;
//		void GenerateData();
//
//		//---------------------------------
//		//Plot related variables
//		//---------------------------------
//		float m_XData[11]{};
//
//		struct Transform
//		{
//			float matrix[16] = {
//				1,0,0,0,
//				0,1,0,0,
//				0,0,1,0,
//				0,0,0,1 };
//		};
//
//		class GameObject3D
//		{
//		public:
//			Transform transform;
//			int ID;
//		};
//
//		class GameObject3DAlt
//		{
//		public:
//			Transform* transform;
//			int ID;
//		};
//
//		//Exercise 1
//		float m_YData1[11]{}; 
//		mutable bool m_DrawExercise1 = false;
//		bool m_Exercise1Generated = false;
//
//		//Exercise 2
//		float m_YData2[11]{}; 
//		float m_YData3[11]{};
//		mutable bool m_DrawExercise2 = false;
//		mutable bool m_DrawExercise2Alt = false;
//		bool m_Exercise2Generated = false;
//		bool m_Exercise2AltGenerated = false;
//	};
//}
