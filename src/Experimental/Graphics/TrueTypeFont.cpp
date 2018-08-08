// Copyright (c) 2013-2018 mogemimi. Distributed under the MIT license.

#include "Pomdog/Experimental/Graphics/TrueTypeFont.hpp"
#include "Pomdog/Experimental/Graphics/FontGlyph.hpp"
#include "Pomdog/Experimental/Graphics/SpriteFont.hpp"
#include "Pomdog/Content/AssetManager.hpp"
#include "Pomdog/Content/Utility/BinaryReader.hpp"
#include "Pomdog/Math/Point2D.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"
#include <utility>
#include <vector>
#include <locale>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcomma"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

namespace Pomdog {

class TrueTypeFont::Impl final {
public:
    std::vector<std::uint8_t> ttfBinary;
    stbtt_fontinfo fontInfo;

    void Reset();

    void LoadFont(const AssetManager& assets, const std::string& fontPath);
};

void TrueTypeFont::Impl::Reset()
{
    if (!ttfBinary.empty()) {
        ttfBinary.clear();
    }
}

void TrueTypeFont::Impl::LoadFont(const AssetManager& assets, const std::string& fontPath)
{
    auto binaryFile = assets.OpenStream(fontPath);
    if (!binaryFile.Stream) {
        POMDOG_THROW_EXCEPTION(std::runtime_error, "Failed to open file");
    }

    if (binaryFile.SizeInBytes <= 0) {
        POMDOG_THROW_EXCEPTION(std::runtime_error, "The file is too small");
    }

    Reset();

    using Pomdog::Detail::BinaryReader;
    ttfBinary = BinaryReader::ReadArray<std::uint8_t>(binaryFile.Stream, binaryFile.SizeInBytes);

    const auto offset = stbtt_GetFontOffsetForIndex(ttfBinary.data(), 0);
    if (!stbtt_InitFont(&fontInfo, ttfBinary.data(), offset)) {
        ttfBinary.clear();

        // FUS RO DAH
        POMDOG_THROW_EXCEPTION(std::runtime_error,
            "Failed to initialize truetype font");
    }
}

TrueTypeFont::TrueTypeFont(const AssetManager& assets, const std::string& fontPath)
    : impl(std::make_unique<Impl>())
{
    POMDOG_ASSERT(impl);
    impl->LoadFont(assets, fontPath);
}

TrueTypeFont::~TrueTypeFont()
{
    POMDOG_ASSERT(impl);
    impl->Reset();
}

Optional<FontGlyph> TrueTypeFont::RasterizeGlyph(
    char32_t codePoint,
    float pixelHeight,
    int textureWidth,
    const std::function<void(int width, int height, Point2D & point, std::uint8_t* & output)>& callback)
{
    if (impl->ttfBinary.empty()) {
        // error
        return Pomdog::NullOpt;
    }

    const auto & f = impl->fontInfo;

    const float scale = stbtt_ScaleForPixelHeight(&f, pixelHeight);

    const int g = stbtt_FindGlyphIndex(&f, codePoint);

    std::locale defaultLocale;
    const bool isSpace = (std::isspace<char>(codePoint, defaultLocale) != 0) && (codePoint != '\n');

    if (g <= 0 && !isSpace) {
        // error: not found
        return Pomdog::NullOpt;
    }

    int advance, lsb, x0, y0, x1, y1;
    stbtt_GetGlyphHMetrics(&f, g, &advance, &lsb);
    stbtt_GetGlyphBitmapBox(&f, g, scale, scale, &x0, &y0, &x1, &y1);
    const int glyphWidth = x1 - x0;
    const int glyphHeight = y1 - y0;

    std::uint8_t* pixels = nullptr;
    Point2D point = {1, 1};

    POMDOG_ASSERT(callback);
    callback(glyphWidth, glyphHeight, point, pixels);

    POMDOG_ASSERT(pixels != nullptr);
    POMDOG_ASSERT(point.X >= 0);
    POMDOG_ASSERT(point.Y >= 0);
    POMDOG_ASSERT(textureWidth > 0);

    if (!isSpace) {
        stbtt_MakeGlyphBitmap(&f, pixels + point.X + point.Y * textureWidth,
            glyphWidth, glyphHeight, textureWidth, scale, scale, g);
    }

    FontGlyph glyph;
    glyph.Subrect.X = point.X;
    glyph.Subrect.Y = point.Y;
    glyph.Subrect.Width = glyphWidth;
    glyph.Subrect.Height = glyphHeight;
    glyph.TexturePage = 0;
    glyph.XAdvance = scale * advance;
    glyph.XOffset = x0;
    glyph.YOffset = y0;
    glyph.Character = codePoint;
    return std::move(glyph);
}

//int TrueTypeFont::GetBaseline(float fontSize) const
//{
//    POMDOG_ASSERT(!impl->ttfBinary.empty());
//    POMDOG_ASSERT(fontSize > 0);
//
//    if (impl->ttfBinary.empty()) {
//        // error
//        return 0;
//    }
//
//    const float scale = stbtt_ScaleForPixelHeight(&impl->fontInfo, fontSize);
//
//    int ascent = 0;
//    stbtt_GetFontVMetrics(&impl->fontInfo, &ascent, 0, 0);
//    int baseline = ascent * scale;
//    return baseline;
//}

//std::vector<Glyph> TrueTypeFont::BakeFontBitmap(float fontSize, int width, int height, std::uint8_t* output)
//{
//    POMDOG_ASSERT(!impl->ttfBinary.empty());
//    POMDOG_ASSERT(fontSize > 0);
//
//    ///@todo ASCII codes [32, 127)
//    std::array<stbtt_bakedchar, 96> characters;
//
//    char32_t startCodePoint = 32;
//
//    stbtt_BakeFontBitmap(impl->ttfBinary.data(),
//        0, fontSize, output, width, height,
//        startCodePoint, characters.size(), characters.data());
//
//    std::vector<Glyph> glyphs;
//    glyphs.reserve(characters.size());
//
//    char32_t codePoint = startCodePoint;
//    for (auto c: characters) {
//        Glyph glyph;
//        glyph.Subrect.X = c.x0;
//        glyph.Subrect.Y = c.y0;
//        glyph.Subrect.Width = c.x1 - c.x0;
//        glyph.Subrect.Height = c.y1 - c.y0;
//        glyph.TexturePage = 0;
//        glyph.XAdvance = c.xadvance;
//        glyph.XOffset = c.xoff;
//        glyph.YOffset = c.yoff;
//        glyph.Character = codePoint;
//        glyphs.push_back(std::move(glyph));
//        ++codePoint;
//    }
//    return std::move(glyphs);
//}

} // namespace Pomdog