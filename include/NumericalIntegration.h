#pragma once
#include <functional>
using MathFunc = std::function<double(double)>;
inline constexpr int MAX_SUBDIVISIONS = 1000000;
void validateParameters(double a, double b, int n);
double rectangle(const MathFunc& f, double a, double b, int n);
double trapezoid(const MathFunc& f, double a, double b, int n);
double simpson(const MathFunc& f, double a, double b, int n);
double integrationError(double fineResult, double coarseResult);
