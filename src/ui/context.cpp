#include "ui/context.hpp"

#include <QImage>

namespace verre::ui {

Context::Context() :
        image_{100, 100, QImage::Format_RGB32} {
    for (int y = 0; y < image_.height(); ++y) {
        for (int x = 0; x < image_.width(); ++x) {
            QColor color{0,
                         static_cast<int>(std::lround(x / static_cast<double>(image_.width() - 1) * 255)),
                         static_cast<int>(std::lround(y / static_cast<double>(image_.height() - 1) * 255))};
            image_.setPixelColor(x, y, color);
        }
    }
}

}  // namespace verre::ui
