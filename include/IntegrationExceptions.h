#pragma once
#include <stdexcept>
#include <string>
class IntegrationException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
class InvalidIntervalException : public IntegrationException {
public:
    InvalidIntervalException() : IntegrationException("Ліва межа має бути меншою за праву") {}
};
class InvalidFunctionValueException : public IntegrationException {
    double x_;
public:
    explicit InvalidFunctionValueException(double x)
        : IntegrationException("Функція повернула NaN або Inf у точці x=" + std::to_string(x)), x_(x) {}
    double argument() const noexcept { return x_; }
};
class SimpsonSubdivisionException : public IntegrationException {
    int count_;
public:
    explicit SimpsonSubdivisionException(int n)
        : IntegrationException("Метод Сімпсона потребує парної кількості розбиттів: n=" + std::to_string(n)), count_(n) {}
    int count() const noexcept { return count_; }
};
