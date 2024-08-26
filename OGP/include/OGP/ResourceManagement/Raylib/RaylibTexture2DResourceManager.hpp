#pragma once

#include <string>

#include <raylib.h>

#include "../../Exportables/Exportable.hxx"
#include "../AResourceManager.hpp"
#include "../ResourceID.hpp"

namespace OGP::ResourceManagement::Raylib {
	class RaylibTexture2DResourceManager : public OGP::ResourceManagement::AResourceManager<Texture2D> {
	public:

		OGP_API RaylibTexture2DResourceManager();
		OGP_API virtual ~RaylibTexture2DResourceManager();

		OGP_API void Initialize();
		OGP_API void Deinitialize();

	protected:

		OGP_API virtual bool TryLoadingResource(const ResourceID& resourceID, Texture2D& result) override;
		OGP_API virtual void UnloadResource(const Texture2D& resource) override;

	private:

		Texture2D invalidTexture2D;
	};
}
