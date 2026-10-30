#ifdef _WIN32
#include <windows.h>
#endif
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
void slicedRethrow() {
    try { throw std::invalid_argument("некоректні дані"); }
    catch (const std::exception& e) { throw e; } 
}
void correctRethrow() {
    try { throw std::invalid_argument("некоректні дані"); }
    catch (const std::exception&) { throw; }
}
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "1. throw int\n";
    try { throw 42; }
    catch (const std::exception&) { std::cout << "Обробник std::exception\n"; }
    catch (...) { std::cout << "catch(...): int не належить до std::exception\n"; }
    std::cout << "2. throw std::string\n";
    try { throw std::string("помилка"); }
    catch (const std::exception&) { std::cout << "Обробник std::exception\n"; }
    catch (...) { std::cout << "catch(...): string не належить до std::exception\n"; }
    std::cout << "3. Стандартний виняток\n";
    try { throw std::invalid_argument("некоректні дані"); }
    catch (const std::exception& e) { std::cout << "std::exception: " << e.what() << '\n'; }
    std::cout << "4. throw e: зрізання об’єкта\n";
    try { slicedRethrow(); }
    catch (const std::invalid_argument&) { std::cout << "Похідний тип збережено\n"; }
    catch (const std::exception&) { std::cout << "Залишився лише базовий тип std::exception\n"; }
    std::cout << "5. throw; зберігає тип\n";
    try { correctRethrow(); }
    catch (const std::invalid_argument&) { std::cout << "Похідний тип invalid_argument збережено\n"; }
}
