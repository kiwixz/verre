#pragma once

#include <stop_token>
#include <thread>

#include <QImage>
#include <QObject>
#include <QQmlEngine>

#include "core/renderer.hpp"
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
    core::Renderer renderer_;
    std::jthread render_thread_;

    void render_thread(const std::stop_token& stop_token) noexcept;
};

}  // namespace verre::ui
