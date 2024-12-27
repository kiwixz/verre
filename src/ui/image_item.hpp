#pragma once

#include <QImage>
#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QSGNode>

#include "ui/opaque.hpp"  // IWYU pragma: keep

namespace verre::ui {

struct ImageItem : QQuickItem {
    Q_PROPERTY(verre::ui::opaque::QImage image MEMBER image_ WRITE update_image REQUIRED)

    ImageItem();

    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData* /*data*/) override;

    void update_image(QImage image);

private:
    QImage image_;

    Q_OBJECT
    QML_ELEMENT
};

}  // namespace verre::ui
