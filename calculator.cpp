#include "calculator.h"

#include <cmath> // Для использования функции std::pow

void Calculator::Set(Number n) {
    current_value_ = n;
}

Number Calculator::GetNumber() const {
    return current_value_;
}

void Calculator::Add(Number n) {
    current_value_ += n;
}

void Calculator::Sub(Number n) {
    current_value_ -= n;
}

void Calculator::Div(Number n) {
    current_value_ /= n;
}

void Calculator::Mul(Number n) {
    current_value_ *= n;
}

void Calculator::Pow(Number n) {
    current_value_ = std::pow(current_value_, n);
}

void Calculator::Save() {
    memory_value_ = current_value_;
    has_memory_ = true;
}

void Calculator::Load() {
    if (has_memory_) {
        current_value_ = memory_value_;
    }
}

bool Calculator::HasMem() const {
    return has_memory_;
}

std::string Calculator::GetNumberRepr() const {
    return std::to_string(current_value_);
}