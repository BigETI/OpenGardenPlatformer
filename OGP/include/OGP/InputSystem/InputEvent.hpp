#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>

#include "../Exportables/Exportable.hxx"
#include "../Hashing/StringHash.hpp"
#include "../Math/Vector2.hpp"
#include "EInputEventValueType.hpp"

namespace OGP::InputSystem {
	class InputEvent {
	public:

		OGP_API InputEvent();
		OGP_API InputEvent(const std::string& name);
		OGP_API InputEvent(std::string&& name);
		OGP_API InputEvent(const std::string& name, bool isPressing);
		OGP_API InputEvent(std::string&& name, bool isPressing) noexcept;
		OGP_API InputEvent(const std::string& name, float pressValue);
		OGP_API InputEvent(std::string&& name, float pressValue) noexcept;
		OGP_API InputEvent(const std::string& name, const OGP::Math::Vector2<float>& pressValue2D);
		OGP_API InputEvent(std::string&& name, OGP::Math::Vector2<float>&& pressValue2D) noexcept;
		OGP_API InputEvent(const OGP::Hashing::StringHash& nameHash);
		OGP_API InputEvent(OGP::Hashing::StringHash&& nameHash);
		OGP_API InputEvent(const OGP::Hashing::StringHash& nameHash, bool isPressing);
		OGP_API InputEvent(OGP::Hashing::StringHash&& nameHash, bool isPressing) noexcept;
		OGP_API InputEvent(const OGP::Hashing::StringHash& nameHash, float pressValue);
		OGP_API InputEvent(OGP::Hashing::StringHash&& nameHash, float pressValue) noexcept;
		OGP_API InputEvent(const OGP::Hashing::StringHash& nameHash, const OGP::Math::Vector2<float>& pressValue2D);
		OGP_API InputEvent(OGP::Hashing::StringHash&& nameHash, OGP::Math::Vector2<float>&& pressValue2D) noexcept;
		OGP_API InputEvent(const InputEvent& inputEvent);
		OGP_API InputEvent(InputEvent&& inputEvent) noexcept;
		OGP_API InputEvent(const InputEvent& inputEvent, bool isPressing);
		OGP_API InputEvent(InputEvent&& inputEvent, bool isPressing) noexcept;
		OGP_API InputEvent(const InputEvent& inputEvent, float pressValue);
		OGP_API InputEvent(InputEvent&& inputEvent, float pressValue) noexcept;
		OGP_API InputEvent(const InputEvent& inputEvent, const OGP::Math::Vector2<float>& pressValue2D);
		OGP_API InputEvent(InputEvent&& inputEvent, OGP::Math::Vector2<float>&& pressValue2D) noexcept;

		OGP_API static const InputEvent& GetCloseWindowInputEvent() noexcept;

		OGP_API const OGP::Hashing::StringHash& GetNameHash() const noexcept;
		OGP_API OGP::Hashing::StringHash& GetNameHash(OGP::Hashing::StringHash& result) const;
		OGP_API EInputEventValueType GetInputEventValueType() const noexcept;
		OGP_API bool IsPressing() const;
		OGP_API float GetPressValue() const;
		OGP_API OGP::Math::Vector2<float> GetPressValue2D() const;

		OGP_API InputEvent& operator =(const InputEvent& inputEvent);
		OGP_API InputEvent& operator =(InputEvent&& inputEvent) noexcept;
		OGP_API InputEvent& operator =(bool isPressing);
		OGP_API InputEvent& operator =(float pressValue);
		OGP_API InputEvent& operator =(const OGP::Math::Vector2<float>& pressValue2D);
		OGP_API InputEvent& operator =(OGP::Math::Vector2<float>&& pressValue2D);

	private:

		static const InputEvent closeWindowInputEvent;

		OGP::Hashing::StringHash nameHash;
		EInputEventValueType inputEventValueType;
		std::optional<bool> isPressing;
		std::optional<float> pressValue;
		std::optional<OGP::Math::Vector2<float>> pressValue2D;
	};
}
