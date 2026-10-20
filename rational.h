#pragma once

#include <numeric>
#include <iostream>
#include <compare>

class Rational {
public:
    // Конструкторы
    Rational() : numerator_(0), denominator_(1) {}

    Rational(int numerator) : numerator_(numerator), denominator_(1) {}

    Rational(int numerator, int denominator) : numerator_(numerator), denominator_(denominator) {
        Reduction();
    }

    Rational(const Rational& other) : numerator_(other.numerator_), denominator_(other.denominator_) {}

    // Операция присваивания
    Rational& operator=(const Rational& other) {
        if (this != &other) {
            numerator_ = other.numerator_;
            denominator_ = other.denominator_;
        }
        return *this;
    }

    // Геттеры
    int GetNumerator() const { return numerator_; }
    int GetDenominator() const { return denominator_; }

    // Метод Inv (инверсия дроби)
    Rational Inv() const {
        return Rational(denominator_, numerator_);
    }

    // Унарные операции
    Rational operator+() const {
        return *this;
    }

    Rational operator-() const {
        return Rational(-numerator_, denominator_);
    }

    // Присваивающие арифметические операции
    Rational& operator+=(const Rational& other) {
        numerator_ = numerator_ * other.denominator_ + other.numerator_ * denominator_;
        denominator_ *= other.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator-=(const Rational& other) {
        numerator_ = numerator_ * other.denominator_ - other.numerator_ * denominator_;
        denominator_ *= other.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator*=(const Rational& other) {
        numerator_ *= other.numerator_;
        denominator_ *= other.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator/=(const Rational& other) {
        numerator_ *= other.denominator_;
        denominator_ *= other.numerator_;
        Reduction();
        return *this;
    }

    // Бинарные арифметические операции
    friend Rational operator+(Rational lhs, const Rational& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Rational operator-(Rational lhs, const Rational& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Rational operator*(Rational lhs, const Rational& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend Rational operator/(Rational lhs, const Rational& rhs) {
        lhs /= rhs;
        return lhs;
    }

    // Сравнения (C++20 оператор <=> и ==)
    bool operator==(const Rational& other) const = default;

    std::strong_ordering operator<=>(const Rational& other) const {
        // Так как дроби всегда хранятся в несократимом виде и с положительным знаменателем,
        // для корректного сравнения приводим к общему знаменателю:
        int64_t lhs_val = static_cast<int64_t>(numerator_) * other.denominator_;
        int64_t rhs_val = static_cast<int64_t>(other.numerator_) * denominator_;
        return lhs_val <=> rhs_val;
    }

    // Ввод и вывод в поток
    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        if (r.denominator_ == 1) {
            os << r.numerator_;
        } else {
            os << r.numerator_ << " / " << r.denominator_;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Rational& r) {
        int num = 0;
        if (!(is >> num)) {
            return is;
        }

        // Пытаемся прочитать следующий символ (пропуская пробелы)
        is >> std::ws;
        if (is.peek() == '/') {
            char slash;
            is >> slash; // Считываем сам символ '/'

            int den = 0;
            if (is >> den && den != 0) {
                r = Rational(num, den);
            } else {
                is.setstate(std::ios_base::failbit); // Ошибка: деление на ноль или не число
            }
        } else {
            // Если '/' нет, значит ввели обычное целое число
            r = Rational(num, 1);
        }
        return is;
    }

private:
    int numerator_;
    int denominator_;

    void Reduction() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }
};
