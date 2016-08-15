// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Basic/Export.hpp"
#include "Pomdog/Graphics/detail/ForwardDeclarations.hpp"
#include "Pomdog/Content/Utility/BinaryFileStream.hpp"
#include <memory>
#include <string>

namespace Pomdog {
namespace Detail {

class POMDOG_EXPORT AssetLoaderContext final {
public:
    std::string RootDirectory;
    std::weak_ptr<Pomdog::GraphicsDevice> GraphicsDevice;

    BinaryFileStream OpenStream(const std::string& assetName) const;
};

} // namespace Detail
} // namespace Pomdog
