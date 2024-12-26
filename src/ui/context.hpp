#pragma once

#include <QImage>
#include <QObject>
#include <QQmlEngine>

namespace verre::ui {

struct Context : QObject {
public:
    Q_PROPERTY(QImage image MEMBER image_ NOTIFY imageChanged)

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
