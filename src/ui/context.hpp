#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QString>

namespace verre::ui {

struct Context : QObject {
public:
    Q_INVOKABLE QString hello() const;

private:
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
};

}  // namespace verre::ui
