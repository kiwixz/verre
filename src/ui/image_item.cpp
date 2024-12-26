#include "ui/image_item.hpp"

#include <exception>

#include <QImage>
#include <QSGImageNode>
#include <QSGNode>

#include "utils/exception.hpp"

namespace verre::ui {

ImageItem::ImageItem() {
    setFlag(Flag::ItemHasContents);
}

QSGNode* ImageItem::updatePaintNode(QSGNode* node_, UpdatePaintNodeData* /*data*/) {
    QSGImageNode* node = static_cast<QSGImageNode*>(node_);
    try {
        if (!node) {
            node = window()->createImageNode();
            if (!node)
                throw Exception{"Could not create image node"};
            node->setOwnsTexture(true);
        }

        node->setRect(boundingRect());

        auto texture = window()->createTextureFromImage(image_);
        if (!texture)
            throw Exception{"Could not create texture"};
        node->setTexture(texture);

        return node;
    }
    catch (const std::exception& ex) {
        if (node != node_)
            delete node;
        throw;
    }
}

void ImageItem::update_image(QImage image) {
    image_ = std::move(image);
    update();
}

}  // namespace verre::ui
