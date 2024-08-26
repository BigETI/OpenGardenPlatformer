#pragma once

#include <chrono>
#include <memory>

#include "../EventSystem/Event.hpp"
#include "../Exportables/Exportable.hxx"
#include "../Game.hpp"
#include "../SceneManagement/Node.hpp"

namespace OGP {
	class Game;
}

namespace OGP::SceneManagement {
	class Node;
}

namespace OGP::Scripting {
	class Script {
	public:

		OGP::EventSystem::Event<OGP::Game&> OnInitialized;
		OGP::EventSystem::Event<OGP::Game&> OnDeinitialized;
		OGP::EventSystem::Event<OGP::Game&> OnEnabled;
		OGP::EventSystem::Event<OGP::Game&> OnDisabled;
		OGP::EventSystem::Event<OGP::Game&, std::chrono::high_resolution_clock::duration> OnGameTicked;
		OGP::EventSystem::Event<OGP::Game&, std::chrono::high_resolution_clock::duration> OnFrameRendered;

		OGP_API Script(OGP::SceneManagement::Node* node);

		Script(const Script&) = delete;
		Script(Script&&) = delete;

		OGP_API virtual ~Script() noexcept;

		OGP_API bool IsInitialized() const noexcept;
		OGP_API bool IsEnabled() const noexcept;
		OGP_API void SetEnabledState(bool enabledState) noexcept;
		OGP_API const OGP::SceneManagement::Node& GetNode() const noexcept;
		OGP_API OGP::SceneManagement::Node& GetNode() noexcept;
		OGP_API void InitializeOrGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime);
		OGP_API void InitializeOrFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime);
		OGP_API void Deinitialize();
		OGP_API void Enable();
		OGP_API void Disable();

		Script& operator =(const Script&) = delete;
		Script& operator =(Script&&) = delete;

	protected:

		OGP_API virtual void OnInitialize(OGP::Game& game);
		OGP_API virtual void OnDeinitialize(OGP::Game& game);
		OGP_API virtual void OnEnable(OGP::Game& game);
		OGP_API virtual void OnDisable(OGP::Game& game);
		OGP_API virtual void OnGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime);
		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime);

	private:

		bool isInitialized;
		bool isEnabled;
		OGP::SceneManagement::Node* node;
		OGP::Game* game;
	};
}
