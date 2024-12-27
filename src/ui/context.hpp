#pragma once

#include <QImage>
#include <QObject>
#include <QQmlEngine>

#include "ui/opaque.hpp"  // IWYU pragma: keep

namespace verre::ui {

struct Context : QObject {
    Q_PROPERTY(verre::ui::opaque::QImage image MEMBER image_ NOTIFY imageChanged)

    Context();

signals:
    void imageChanged();

private:
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    QImage image_;
};

}  // namespace verre::ui
