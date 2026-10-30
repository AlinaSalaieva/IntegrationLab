#include "NumericalIntegration.h"
#include "IntegrationExceptions.h"
#include <cmath>

namespace {
double checked(double value) {
    if (!std::isfinite(value))
        throw std::overflow_error("Переповнення проміжного або кінцевого результату");
    return value;
}
double valueAt(const MathFunc& f, double x) {
    const double value = f(x);
    if (!std::isfinite(value)) throw InvalidFunctionValueException(x);
    return value;
}
void validateFunction(const MathFunc& f) {
    if (!f) throw std::invalid_argument("Функцію інтегрування не задано");
}
}
void validateParameters(double a, double b, int n) {
    if (!std::isfinite(a) || !std::isfinite(b))
        throw std::invalid_argument("Межі інтегрування мають бути скінченними");
    if (a >= b) throw InvalidIntervalException();
    if (n <= 0 || n > MAX_SUBDIVISIONS)
        throw std::invalid_argument("Кількість розбиттів має належати діапазону від 1 до 1000000");
    const double width = checked(b - a);
    const double step = width / n;
    if (step <= 0 || (n > 1 && (a + step == a || b - step == b)))
        throw std::invalid_argument("Крок надто малий для подання у форматі double");
}
// Алгоритми прямокутників, трапецій та Сімпсона з лабораторної роботи №4.
double rectangle(const MathFunc& f, double a, double b, int n) {
    validateFunction(f);
    validateParameters(a, b, n);
    const double h = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        const double x = checked(a + (i + 0.5) * h);
        sum = checked(sum + valueAt(f, x));
    }
    return checked(sum * h);
}
double trapezoid(const MathFunc& f, double a, double b, int n) {
    validateFunction(f);
    validateParameters(a, b, n);
    const double h = (b - a) / n;
    double sum = checked(0.5 * checked(valueAt(f, a) + valueAt(f, b)));
    for (int i = 1; i < n; ++i)
        sum = checked(sum + valueAt(f, checked(a + i * h)));
    return checked(sum * h);
}
double simpson(const MathFunc& f, double a, double b, int n) {
    validateFunction(f);
    validateParameters(a, b, n);
    if (n % 2 != 0) throw SimpsonSubdivisionException(n);
    const double h = (b - a) / n;
    double sum = checked(valueAt(f, a) + valueAt(f, b));
    for (int i = 1; i < n; ++i) {
        const double value = valueAt(f, checked(a + i * h));
        sum = checked(sum + checked((i % 2 == 0 ? 2.0 : 4.0) * value));
    }
    return checked(sum * (h / 3.0));
}
double integrationError(double fineResult, double coarseResult) {
    if (!std::isfinite(fineResult) || !std::isfinite(coarseResult))
        throw std::invalid_argument("Результати для оцінки похибки мають бути скінченними");
    return std::abs(checked(fineResult - coarseResult));
}
