#include <chrono>
#include <exception>
#include <filesystem>

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QTimer>

#include "utils/log.hpp"

int main(int argc, char** argv) {
    using namespace verre;

    try {
        qInstallMessageHandler([](QtMsgType level_qt, const QMessageLogContext& /*from*/, const QString& message) {
            log::Level level = [&] {
                switch (level_qt) {
                case QtDebugMsg: return log::Level::debug;
                case QtInfoMsg: return log::Level::info;
                case QtWarningMsg: return log::Level::warning;
                case QtCriticalMsg:
                case QtFatalMsg: return log::Level::error;
                }
                assert(false);
                return log::Level::debug;
            }();
            log::log(level, "Qt: {}", message.toStdString());
        });

        const char* hot_reload = std::getenv("HOTRELOAD");

        QGuiApplication app{argc, argv};
        QQuickView window;

        window.setResizeMode(QQuickView::SizeRootObjectToView);
        window.resize(1280, 720);

        std::string qml_folder = hot_reload ? hot_reload : "qrc:qt/qml/verre";
        QUrl main_qml = QString::fromStdString(fmt::format("{}/main.qml", qml_folder));
        window.setSource(main_qml);
        if (window.status() != QQuickView::Ready)
            return 1;
        window.show();

        QTimer timer;
        if (hot_reload) {
            log::info("Enabling hot reload");
            timer.callOnTimeout([&,
                                 hot_reload = std::filesystem::path{hot_reload},
                                 last_reload = std::chrono::file_clock::now()]() mutable noexcept {
                try {
                    std::chrono::file_clock::time_point now = std::chrono::file_clock::now();
                    if (std::ranges::none_of(std::filesystem::recursive_directory_iterator{hot_reload},
                                             [&](const std::filesystem::directory_entry& entry) { return entry.last_write_time() > last_reload; }))
                    {
                        return;
                    }

                    log::info("Reloading QML");
                    window.setSource({});
                    window.engine()->clearComponentCache();
                    window.setSource(main_qml);
                    last_reload = now;
                }
                catch (const std::exception& ex) {
                    log::error("{}", ex.what());
                    app.exit(1);
                }
            });
            timer.setInterval(200);
            timer.start();
        }

        return app.exec();
    }
    catch (const std::exception& ex) {
        log::error("{}", ex.what());
        return 1;
    }
}
