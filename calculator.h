#pragma once

#include <string>
#include <optional>
#include <cmath>
#include <type_traits>
#include "rational.h"
#include "pow.h"

using Error = std::string;

template <class Number>
class Calculator {
public:
    void Set(Number n) {
        current_value_ = n;
    }

    Number GetNumber() const {
        return current_value_;
    }

    std::optional<Error> Add(Number n) {
        current_value_ = current_value_ + n;
        return std::nullopt;
    }

    std::optional<Error> Sub(Number n) {
        current_value_ = current_value_ - n;
        return std::nullopt;
    }

    std::optional<Error> Mul(Number n) {
        current_value_ = current_value_ * n;
        return std::nullopt;
    }

    std::optional<Error> Div(Number n) {
        if constexpr (std::is_floating_point_v<Number>) {
            current_value_ = current_value_ / n;
            return std::nullopt;
        } else {
            if (n == Number{0}) {
                return Error{"Division by zero"};
            }
            current_value_ = current_value_ / n;
            return std::nullopt;
        }
    }

    std::optional<Error> Pow(Number n) {
        if constexpr (std::is_floating_point_v<Number>) {
            // Всегда успешно для float/double (возможны NaN)
            current_value_ = std::pow(current_value_, n);
            return std::nullopt;
        } else if constexpr (std::is_same_v<Number, Rational>) {
            if (current_value_ == Rational{0} && n == Rational{0}) {
                return Error{"Zero power to zero"};
            }
            if (n.GetDenominator() != 1) {
                return Error{"Fractional power is not supported"};
            }
            current_value_ = ::Pow(current_value_, n);
            return std::nullopt;
        } else {
            // Целочисленные типы
            if (current_value_ == Number{0} && n == Number{0}) {
                return Error{"Zero power to zero"};
            }
            if (n < Number{0}) {
                return Error{"Integer negative power"};
            }
            current_value_ = IntegerPow(current_value_, n);
            return std::nullopt;
        }
    }

    void Save() {
        memory_ = current_value_;
    }

    void Load() {
        if (memory_.has_value()) {
            current_value_ = memory_.value();
        }
    }

    bool GetHasMem() const {
        return memory_.has_value();
    }

    std::string GetNumberRepr() const {
        return {};
    }

private:
    Number current_value_{};
    std::optional<Number> memory_;
};