#pragma once
#include "NumericalIntegration.h"
#include <optional>
struct IntegrationResults {
    double rectangle;
    double trapezoid;
    double simpson;
    bool operator==(const IntegrationResults&) const = default;
};
class IntegrationTask {
    double a_;
    double b_;
    int n_;
    double tolerance_;
    std::optional<IntegrationResults> results_;
    static void validateTolerance(double tolerance);
public:
    IntegrationTask(double a, double b, int n, double tolerance = 0.001);
    IntegrationTask(const IntegrationTask&) = default;
    // Гарантія відсутності винятків (no-throw): копіюється лише простий стан.
    IntegrationTask& operator=(const IntegrationTask&) noexcept = default;
    double left() const noexcept { return a_; }
    double right() const noexcept { return b_; }
    int subdivisions() const noexcept { return n_; }
    double tolerance() const noexcept { return tolerance_; }
    bool hasResults() const noexcept { return results_.has_value(); }
    IntegrationResults results() const;
    bool withinTolerance(double reference) const;
    // Сильна гарантія (strong): перевірка всіх параметрів до зміни стану.
    // Після успіху попередні результати скидаються, бо параметри змінилися.
    void setParameters(double a, double b, int n, double tolerance);
    // Сильна гарантія (strong): результати трьох методів готуються локально.
    // Фіксація виконується лише після успіху всіх обчислень.
    void computeAll(const MathFunc& f);
    // Гарантія відсутності винятків (no-throw).
    void clearResults() noexcept;
    // Гарантія відсутності винятків (no-throw): автоматичне звільнення ресурсів.
    ~IntegrationTask() noexcept = default;
};
