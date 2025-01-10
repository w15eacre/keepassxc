#include "MimeTypes.h"

#include <array>

namespace
{

    bool isTextCompatibleFormat(const QString& format)
    {
        constexpr static std::array Formats = {
            "text/",
            "application/json",
            "application/xml",
            "application/soap+xml",
            "application/x-yaml",
            "application/protobuf",
        };

        return std::any_of(
            std::cbegin(Formats), std::cend(Formats), [&format](const auto& f) { return format.startsWith(f); });
    }

    bool isImageCompatibleFormat(const QString& format)
    {
        constexpr static std::array Formats = {"image/"};

        return std::any_of(
            std::cbegin(Formats), std::cend(Formats), [&format](const auto& f) { return format.startsWith(f); });
    }

} // namespace

namespace core
{
    MimeType toMimeType(const QString& mimeName)
    {
        if (isImageCompatibleFormat(mimeName)) {
            return MimeType::Image;
        } else if (isTextCompatibleFormat(mimeName)) {
            return MimeType::PlantText;
        }

        return MimeType::Unknown;
    }

} // namespace core
