#pragma once

#include "common/data.h"
#include "common/exception.h"

#include <string.h>

namespace love
{
    class GlyphData : public Data
    {
        public:
            static love::Type type;

            struct GlyphMetrics
            {
                int height;
                int width;
                int advance;
                int bearingX;
                int bearingY;
            };

            GlyphData(uint32_t glyph, GlyphMetrics metrics);

            GlyphData(const GlyphData & c);

            virtual ~GlyphData();

            GlyphData * Clone() const;

            size_t GetPixelSize() const;

            void * GetData() const;

            size_t GetSize() const;

            void * GetData(int x, int y) const;

            int GetHeight() const;

            int GetWidth() const;

            uint32_t GetGlyph() const;

            std::string GetGlyphString() const;

            int GetAdvance() const;

            int GetBearingX() const;

            int GetBearingY() const;

            int GetMinX() const;

            int GetMinY() const;

            int GetMaxX() const;

            int GetMaxY() const;

        private:
            uint32_t glyph;

            GlyphMetrics metrics;

            uint8_t * data;

    };
}