#include "IntegrationTask.h"
#include <cmath>
#include <stdexcept>
#include <utility>
void IntegrationTask::validateTolerance(double tolerance) {
    if (!std::isfinite(tolerance) || tolerance <= 0)
        throw std::invalid_argument("Точність має бути додатним скінченним числом");
}
IntegrationTask::IntegrationTask(double a, double b, int n, double tolerance)
    : a_(a), b_(b), n_(n), tolerance_(tolerance) {
    validateParameters(a, b, n);
    validateTolerance(tolerance);
}
void IntegrationTask::setParameters(double a, double b, int n, double tolerance) {
    validateParameters(a, b, n);
    validateTolerance(tolerance);
    a_ = a;
    b_ = b;
    n_ = n;
    tolerance_ = tolerance;
    results_.reset();
}
void IntegrationTask::computeAll(const MathFunc& f) {
#ifdef LAB7_INJECT_BUG
    // Навмисний дефект для негативного самоконтролю: передчасна зміна стану.
    results_ = IntegrationResults{rectangle(f, a_, b_, n_), 0, 0};
#endif
    const IntegrationResults prepared{
        rectangle(f, a_, b_, n_),
        trapezoid(f, a_, b_, n_),
        simpson(f, a_, b_, n_)
    };
    static_assert(noexcept(results_ = prepared));
    results_ = prepared;
}
void IntegrationTask::clearResults() noexcept { results_.reset(); }
IntegrationResults IntegrationTask::results() const {
    if (!results_) throw std::logic_error("Результати ще не обчислено");
    return *results_;
}
bool IntegrationTask::withinTolerance(double reference) const {
    if (!std::isfinite(reference))
        throw std::invalid_argument("Еталонне значення має бути скінченним");
    const auto result = results();
    return integrationError(result.rectangle, reference) <= tolerance_
        && integrationError(result.trapezoid, reference) <= tolerance_
        && integrationError(result.simpson, reference) <= tolerance_;
}
