#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>

#include "../EventSystem/Event.hpp"
#include "../Exportables/Exportable.hxx"
#include "../Scripting/Script.hpp"
#include "../Game.hpp"
#include "../Math/Vector2.hpp"

namespace OGP::Scripting {
	class Script;
}

namespace OGP {
	class Game;
}

namespace OGP::SceneManagement {

	// TODO: Implement proper transformation matrices, and transformation operations
	class Node {
	public:

		static constexpr Node* NullParent = nullptr;
		OGP::EventSystem::Event<const std::shared_ptr<Node>&> OnChildAdded;
		OGP::EventSystem::Event<const std::shared_ptr<Node>&> OnChildRemoved;
		OGP::EventSystem::Event<const std::shared_ptr<OGP::Scripting::Script>&> OnScriptAdded;
		OGP::EventSystem::Event<const std::shared_ptr<OGP::Scripting::Script>&> OnScriptRemoved;

		OGP_API Node(Node* parent);
		OGP_API Node(Node* parent, const std::string& name);
		OGP_API Node(Node* parent, std::string&& name) noexcept;
		OGP_API ~Node();

		Node(const Node&) = delete;
		Node(Node&&) noexcept = delete;

		OGP_API Node* GetParent() const noexcept;
		OGP_API bool IsLocallyEnabled() const noexcept;
		OGP_API bool IsEnabled() const noexcept;
		OGP_API void Enable();
		OGP_API void Disable();
		OGP_API bool IsMarkedForDeletion() const noexcept;
		OGP_API const std::string& GetName() const noexcept;
		OGP_API std::string& GetName(std::string& result) const;
		OGP_API void SetName(const std::string& name);
		OGP_API void SetName(std::string&& name) noexcept;
		OGP_API const OGP::Math::Vector2<float>& GetLocalPosition() const noexcept;
		OGP_API void SetLocalPosition(const OGP::Math::Vector2<float>& localPosition) noexcept;
		OGP_API OGP::Math::Vector2<float> GetPosition() const noexcept;
		//OGP_API void SetPosition(const OGP::Math::Vector2<float>& position) noexcept;
		OGP_API float GetLocalRotation() const noexcept;
		OGP_API void SetLocalRotation(float localRotation) noexcept;
		OGP_API float GetRotation() const noexcept;
		OGP_API void SetRotation(float rotation) noexcept;
		OGP_API const OGP::Math::Vector2<float>& GetLocalScale() const noexcept;
		OGP_API void SetLocalScale(const OGP::Math::Vector2<float>& localScale) noexcept;
		OGP_API OGP::Math::Vector2<float> GetScale() const noexcept;
		OGP_API void SetScale(const OGP::Math::Vector2<float>& scale) noexcept;
		OGP_API const std::vector<std::shared_ptr<Node>>& GetChildren() const noexcept;
		OGP_API std::vector<std::shared_ptr<Node>>& GetChildren(std::vector<std::shared_ptr<Node>>& result) const;
		OGP_API std::shared_ptr<Node> CreateNewChild();
		OGP_API std::shared_ptr<Node> CreateNewChild(const std::string& name);
		OGP_API std::shared_ptr<Node> CreateNewChild(std::string&& name);
		OGP_API std::shared_ptr<Node> CreateNewAndInsertChild(std::size_t index);
		OGP_API std::shared_ptr<Node> CreateNewAndInsertChild(std::size_t index, const std::string& name);
		OGP_API std::shared_ptr<Node> CreateNewAndInsertChild(std::size_t index, std::string&& name);
		OGP_API bool RemoveChild(std::size_t index);
		OGP_API bool RemoveChild(std::shared_ptr<Node> child);
		OGP_API void RemoveAllChildren() noexcept;
		OGP_API const std::vector<std::shared_ptr<OGP::Scripting::Script>>& GetScripts() const noexcept;
		OGP_API std::vector<std::shared_ptr<OGP::Scripting::Script>>& GetScripts(
			std::vector<std::shared_ptr<OGP::Scripting::Script>>& result
		) const;
		template <typename TScript>
		constexpr inline std::shared_ptr<TScript> AddScript() {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			std::shared_ptr<TScript> ret(std::make_shared<TScript>(this));
			scripts.push_back(ret);
			OnScriptAdded(ret);
			return ret;
		}
		template <typename TScript>
		constexpr inline std::shared_ptr<TScript> EnsureScript() {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			std::shared_ptr<TScript> ret;
			if (!TryGettingScript<TScript>(ret)) {
				ret = AddScript<TScript>();
			}
			return ret;
		}
		template <typename TScript>
		constexpr bool IsScriptAvailable() const noexcept {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			bool ret(false);
			for (const auto& script : scripts) {
				if (dynamic_cast<TScript*>(script.get())) {
					ret = true;
					break;
				}
			}
			return ret;
		}
		template <typename TScript>
		constexpr bool TryGettingScript(const std::shared_ptr<TScript>& result) const noexcept {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			bool ret(false);
			for (const auto& script : scripts) {
				result = std::dynamic_pointer_cast<TScript>(script);
				if (result) {
					ret = true;
					break;
				}
			}
			return ret;
		}
		template <typename TScript>
		constexpr bool TryGettingScript(std::shared_ptr<TScript>& result) noexcept {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			bool ret(false);
			for (auto& script : scripts) {
				result = std::dynamic_pointer_cast<TScript>(script);
				if (result) {
					ret = true;
					break;
				}
			}
			return ret;
		}
		template <typename TScript>
		constexpr std::vector<std::shared_ptr<TScript>>& GetScripts(std::vector<std::shared_ptr<TScript>>& result) const {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			result.clear();
			for (const auto& script : scripts) {
				if (dynamic_cast<TScript*>(script.get())) {
					result.push_back(script);
				}
			}
			return result;
		}
		template <typename TScript>
		constexpr bool RemoveScript(const std::shared_ptr<TScript>& script) {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			bool ret(false);
			if (script) {
				const auto& it(std::find(scripts.begin(), scripts.end(), script));
				if (it != scripts.end()) {
					OnScriptRemoved(script);
					script->Deinitialize();
					scripts.erase(it);
					ret = true;
				}
			}
			return ret;
		}
		template <typename TScript>
		constexpr std::size_t RemoveScripts() {
			static_assert(
				std::is_base_of<OGP::Scripting::Script, TScript>::value,
				"Specified type is not derived from OGP::Scripting::Script.");
			std::size_t ret(0U);
			std::vector<std::shared_ptr<TScript>> scripts;
			for (const auto& script : GetScripts<TScript>(scripts)) {
				if (RemoveScript<TScript>(script)) {
					++ret;
				}
			}
			return ret;
		}
		OGP_API void RemoveAllScripts();
		OGP_API void GameTickScripts(Game& game, std::chrono::high_resolution_clock::duration deltaTime);
		OGP_API void BeforeFrameRenderScripts(Game& game, std::chrono::high_resolution_clock::duration deltaTime);
		OGP_API void FrameRenderScripts(Game& game, std::chrono::high_resolution_clock::duration deltaTime);
		OGP_API void Destroy();

		Node& operator =(const Node&) = delete;
		Node& operator =(Node&&) noexcept = delete;

	private:

		Node* parent;
		bool isLocallyEnabled;
		bool isMarkedForDeletion;
		std::string name;
		OGP::Math::Vector2<float> localPosition;
		float localRotation;
		OGP::Math::Vector2<float> localScale;
		std::vector<std::shared_ptr<Node>> children;
		std::vector<std::shared_ptr<OGP::Scripting::Script>> scripts;
		std::vector<std::shared_ptr<OGP::Scripting::Script>> temporaryScripts;
	};
}
