#pragma once

#include <utility>

#include <QImage>
#include <QObject>
#include <QQmlEngine>

namespace verre::ui::opaque {

#define MAKE(T)                            \
    struct T {                             \
        using Value = ::T;                 \
                                           \
        T() = default;                     \
                                           \
        T(Value value) :                   \
                value_{std::move(value)} { \
        }                                  \
                                           \
        operator Value() const& {          \
            return value_;                 \
        }                                  \
        operator Value() && {              \
            return std::move(value_);      \
        }                                  \
                                           \
        const Value& operator*() const {   \
            return value_;                 \
        }                                  \
        Value& operator*() {               \
            return value_;                 \
        }                                  \
                                           \
    private:                               \
        Value value_;                      \
                                           \
        Q_GADGET                           \
        QML_ANONYMOUS                      \
    };


MAKE(QImage);


#undef MAKE

}  // namespace verre::ui::opaque
