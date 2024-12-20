#pragma once

#include "layers/Layer.hpp"

namespace ENGINE 
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
		
		void OnEvent(Event& e) override;
		void BlockEvents(bool block) { m_BlockEvents = block; }

        void Begin();
        void End();

		void SetThemeColors();
		void SetTheme2();
		void SetDefaultTheme();

	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = true;
	};
}