#pragma once

#include <string>

// Обязательная строка для тренажёра:
using Number = double;

class Calculator {
public:
    void Set(Number n);
    Number GetNumber() const;

    void Add(Number n);
    void Sub(Number n);
    void Div(Number n);
    void Mul(Number n);
    void Pow(Number n);

    void Save();
    void Load();
    bool HasMem() const;

    std::string GetNumberRepr() const;

private:
    Number current_value_ = 0.0;
    Number memory_value_ = 0.0;
    bool has_memory_ = false;
};