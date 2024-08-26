#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <span>
#include <string>

#include "EventSystem/Event.hpp"
#include "Exportables/Exportable.hxx"
#include "InputSystem/IInputHandler.hpp"
#include "Math/Ratio.hpp"
#include "Rendering/IRenderer.hpp"
#include "Rendering/RenderingContext.hpp"
#include "SceneManagement/Node.hpp"

namespace OGP::SceneManagement {
	class Node;
}

namespace OGP {
	class Game {
	public:

		OGP::EventSystem::Event<std::chrono::high_resolution_clock::time_point> OnGameStarted;
		OGP::EventSystem::Event<std::chrono::high_resolution_clock::duration> OnFrameRendered;
		OGP::EventSystem::Event<std::chrono::high_resolution_clock::duration> OnGameTicked;
		OGP::EventSystem::Event<int> OnGameStopped;

		OGP_API Game(const std::span<const std::string> commandLineArguments);
		OGP_API Game(const std::span<const std::string> commandLineArguments, const std::filesystem::path& configurationFilePath);

		Game(const Game&) = delete;
		Game(Game&&) = delete;

		OGP_API ~Game();

		OGP_API bool IsGameRunning() const noexcept;
		OGP_API std::chrono::high_resolution_clock::time_point GetGameStartTimePoint() const noexcept;
		OGP_API const std::vector<OGP::InputSystem::InputEvent>& GetCurrentInputEvents() const noexcept;
		OGP_API std::vector<OGP::InputSystem::InputEvent>& GetCurrentInputEvents(
			std::vector<OGP::InputSystem::InputEvent>& result
		) const;
		OGP_API const std::vector<std::shared_ptr<OGP::SceneManagement::Node>>& GetScenes() const noexcept;
		OGP_API std::vector<std::shared_ptr<OGP::SceneManagement::Node>>& GetScenes(
			std::vector<std::shared_ptr<OGP::SceneManagement::Node>>& result
		) const;
		OGP_API const std::vector<OGP::Rendering::RenderingContext>& GetRenderingContexts() const noexcept;
		OGP_API std::vector<OGP::Rendering::RenderingContext>& GetRenderingContexts() noexcept;
		OGP_API int GetExitCode();
		OGP_API int Start();
		OGP_API void Stop();
		OGP_API void Stop(int exitCode);

		OGP_API std::shared_ptr<OGP::SceneManagement::Node> CreateNewEmptyScene();
		template <typename TNode>
		constexpr inline std::shared_ptr<TNode> CreateNewScene() {
			std::shared_ptr<TNode> ret(std::make_shared<TNode>(TNode::NullParent));
			scenes.push_back(ret);
			return ret;
		}
		OGP_API bool RemoveScene(const std::shared_ptr<OGP::SceneManagement::Node>& scene);
		OGP_API void ClearScenes();

		OGP_API bool AddRenderer(const std::shared_ptr<OGP::Rendering::IRenderer>& renderer);
		OGP_API bool RemoveRenderer(const std::shared_ptr<OGP::Rendering::IRenderer>& renderer);
		OGP_API void ClearRenderers() noexcept;
		
		OGP_API bool AddInputHandler(const std::shared_ptr<OGP::InputSystem::IInputHandler>& inputHandler);
		OGP_API bool RemoveInputHandler(const std::shared_ptr<OGP::InputSystem::IInputHandler>& inputHandler);
		OGP_API void ClearInputHandlers() noexcept;
		
		Game& operator =(const Game&) = delete;
		Game& operator =(Game&&) = delete;

	private:

		OGP::Math::Ratio<unsigned int> targetGameTickRate;
		OGP::Math::Ratio<unsigned int> targetFrameRate;
		std::atomic_bool isGameRunning;
		std::chrono::high_resolution_clock::time_point gameStartTimePoint;
		std::vector<OGP::InputSystem::InputEvent> currentInputEvents;
		std::vector<std::shared_ptr<OGP::SceneManagement::Node>> scenes;
		std::vector<OGP::Rendering::RenderingContext> renderingContexts;
		std::vector<std::shared_ptr<OGP::Rendering::IRenderer>> renderers;
		std::vector<std::shared_ptr<OGP::InputSystem::IInputHandler>> inputHandlers;
		std::atomic_int exitCode;
	};
}
