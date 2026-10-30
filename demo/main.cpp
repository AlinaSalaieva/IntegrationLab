#ifdef _WIN32
#include <windows.h>
#endif
#include "IntegrationTask.h"
#include "IntegrationExceptions.h"
#include "ScopeLogger.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>

double function(double x) { return 3 * x * x; }
void print(const IntegrationResults& r) {
    std::cout << std::setprecision(10)
        << "Прямокутники: " << r.rectangle << '\n'
        << "Трапеції: " << r.trapezoid << '\n'
        << "Сімпсон: " << r.simpson << '\n';
}
template<class Operation> void scenario(const char* name, Operation operation) {
    std::cout << '\n' << name << '\n';
    try { operation(); }
    catch (const InvalidIntervalException& e) { std::cout << "InvalidIntervalException: " << e.what() << '\n'; }
    catch (const SimpsonSubdivisionException& e) { std::cout << "SimpsonSubdivisionException: " << e.what() << '\n'; }
    catch (const InvalidFunctionValueException& e) { std::cout << "InvalidFunctionValueException: " << e.what() << '\n'; }
    catch (const IntegrationException& e) { std::cout << "IntegrationException: " << e.what() << '\n'; }
    catch (const std::invalid_argument& e) { std::cout << "invalid_argument: " << e.what() << '\n'; }
    catch (const std::overflow_error& e) { std::cout << "overflow_error: " << e.what() << '\n'; }
    catch (const std::exception& e) { std::cout << "std::exception: " << e.what() << '\n'; }
}
void level3() {
    ScopeLogger logger("level3");
    auto task = std::make_unique<IntegrationTask>(0, 2, 3);
    task->computeAll(function);
}
void level2() {
    ScopeLogger logger("level2");
    try { level3(); }
    catch (const IntegrationException& e) {
        std::cout << "[ЖУРНАЛ] level2: " << e.what() << '\n';
        throw;
    }
}
void level1() { ScopeLogger logger("level1"); level2(); }
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    try {
        std::cout << "ЛАБОРАТОРНА РОБОТА 7 — ВАРІАНТ 1 — ЧИСЕЛЬНЕ ІНТЕГРУВАННЯ\n";
        std::cout << "f(x) = 3*x*x; [0, 2]; n = 100; точне значення = 8\n";
        IntegrationTask task(0, 2, 100, 0.001);
        task.computeAll(function); print(task.results());
        std::cout << "У межах заданої точності: " << std::boolalpha << task.withinTolerance(8) << '\n';
        std::cout << "Похибка Сімпсона: " << integrationError(task.results().simpson, 8) << '\n';
        scenario("1. Недопустимий інтервал", [] { IntegrationTask bad(2, 0, 10); });
        scenario("2. Нульова кількість розбиттів", [] { IntegrationTask bad(0, 2, 0); });
        scenario("3. Непарна кількість для Сімпсона", [] { simpson(function, 0, 2, 3); });
        scenario("4. Нескінченна межа", [] { IntegrationTask bad(0, std::numeric_limits<double>::infinity(), 10); });
        scenario("5. NaN у значенні функції", [] { rectangle([](double) { return std::numeric_limits<double>::quiet_NaN(); }, 0, 2, 10); });
        scenario("6. Переповнення результату", [] { rectangle([](double) { return 1e308; }, 0, 10, 2); });
        scenario("7. Некоректна точність", [] { IntegrationTask bad(0, 2, 10, 0); });
        std::cout << "\nСИЛЬНА ГАРАНТІЯ: до операції\n"; print(task.results());
        const auto old = task.results();
        scenario("Помилка другого методу після успіху першого", [&] {
            task.computeAll([](double x) {
                if (x == 0) return std::numeric_limits<double>::quiet_NaN();
                return x;
            });
        });
        std::cout << "Після невдалої операції:\n"; print(task.results());
        std::cout << "Усі результати збережено: " << (task.results() == old) << '\n';
        std::cout << "\nRAII / РОЗКРУЧУВАННЯ СТЕКА\n";
        try { level1(); }
        catch (const SimpsonSubdivisionException& e) { std::cout << "[ГОЛОВНА] " << e.what() << '\n'; }
        std::cout << "Програма продовжує роботу.\n";
    } catch (const std::exception& e) {
        std::cerr << "Неочікувана помилка: " << e.what() << '\n'; return 1;
    }
}
