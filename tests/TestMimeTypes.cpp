#include "TestMimeTypes.h"

#include <core/MimeTypes.h>

#include <QTest>

QTEST_GUILESS_MAIN(TestMimeType)

void TestMimeType::testMimePlantText()
{
    const std::array TextMimeTypes = {
        "text/plain", // Plain text
        "text/html", // HTML documents
        "text/css", // CSS stylesheets
        "text/javascript", // JavaScript files
        "text/markdown", // Markdown documents
        "text/xml", // XML documents
        "text/rtf", // Rich Text Format
        "text/vcard", // vCard files
        "text/tab-separated-values", // Tab-separated values
        "application/json", // JSON data
        "application/xml", // XML data
        "application/soap+xml", // SOAP messages
        "application/x-yaml", // YAML data
        "application/protobuf", // Protocol Buffers
    };

    for (const auto& mime : TextMimeTypes) {
        QCOMPARE(core::toMimeType(mime), core::MimeType::PlantText);
    }
}

void TestMimeType::testMimeImage()
{
    constexpr static std::array ImageMimeTypes = {
        "image/jpeg", // JPEG images
        "image/png", // PNG images
        "image/gif", // GIF images
        "image/bmp", // BMP images
        "image/webp", // WEBP images
        "image/svg+xml" // SVG images
    };

    for (const auto& mime : ImageMimeTypes) {
        QCOMPARE(core::toMimeType(mime), core::MimeType::Image);
    }
}

void TestMimeType::testMimeUnknown()
{
    constexpr static std::array UnknownMimeTypes = {
        "audio/mpeg", // MPEG audio files
        "video/mp4", // MP4 video files
        "application/pdf", // PDF documents
        "application/zip", // ZIP archives
        "application/x-tar", // TAR archives
        "application/x-rar-compressed", // RAR archives
        "application/x-7z-compressed", // 7z archives
        "application/x-shockwave-flash", // Adobe Flash files
        "application/vnd.ms-excel", // Microsoft Excel files
        "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", // Microsoft Excel (OpenXML) files
        "application/vnd.ms-powerpoint", // Microsoft PowerPoint files
        "application/vnd.openxmlformats-officedocument.presentationml.presentation", // Microsoft PowerPoint (OpenXML)
                                                                                     // files
        "application/msword", // Microsoft Word files
        "application/vnd.openxmlformats-officedocument.wordprocessingml.document", // Microsoft Word (OpenXML) files
        "application/vnd.oasis.opendocument.text", // OpenDocument Text
        "application/vnd.oasis.opendocument.spreadsheet", // OpenDocument Spreadsheet
        "application/vnd.oasis.opendocument.presentation", // OpenDocument Presentation
        "application/x-httpd-php", // PHP files
        "application/x-perl", // Perl scripts
        "application/x-python", // Python scripts
        "application/x-ruby", // Ruby scripts
        "application/x-shellscript", // Shell scripts
    };

    for (const auto& mime : UnknownMimeTypes) {
        QCOMPARE(core::toMimeType(mime), core::MimeType::Unknown);
    }
}
