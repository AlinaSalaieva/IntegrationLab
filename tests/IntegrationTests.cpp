#include "IntegrationTask.h"
#include "IntegrationExceptions.h"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <type_traits>
void PrintTo(const IntegrationResults& value, std::ostream* out) {
    *out << "{ " << value.rectangle << ", " << value.trapezoid << ", " << value.simpson << " }";
}
namespace {
double quadratic(double x) { return 3*x*x; }
double linear(double x) { return x; }
double constant(double) { return 2; }
constexpr double inf = std::numeric_limits<double>::infinity();
constexpr double nanValue = std::numeric_limits<double>::quiet_NaN();
}
TEST(Algorithms, OriginalRectangle) { EXPECT_NEAR(rectangle(quadratic, 0, 2, 100), 7.9998, 1e-10); }
TEST(Algorithms, OriginalTrapezoid) { EXPECT_NEAR(trapezoid(quadratic, 0, 2, 100), 8.0004, 1e-10); }
TEST(Algorithms, OriginalSimpson) { EXPECT_NEAR(simpson(quadratic, 0, 2, 100), 8, 1e-10); }
TEST(Algorithms, ConstantFunction) {
    EXPECT_NEAR(rectangle(constant, -1, 2, 2), 6, 1e-12);
    EXPECT_NEAR(trapezoid(constant, -1, 2, 2), 6, 1e-12);
    EXPECT_NEAR(simpson(constant, -1, 2, 2), 6, 1e-12);
}
TEST(Algorithms, LinearMinimumSubdivision) {
    EXPECT_NEAR(rectangle(linear, 0, 2, 1), 2, 1e-12);
    EXPECT_NEAR(trapezoid(linear, 0, 2, 1), 2, 1e-12);
    EXPECT_NEAR(simpson(linear, 0, 2, 2), 2, 1e-12);
}
TEST(Algorithms, SineIntegral) {
    EXPECT_NEAR(simpson([](double x){return std::sin(x);}, 0, std::acos(-1.0), 100), 2, 1e-7);
}
TEST(Algorithms, ErrorCalculation) { EXPECT_NEAR(integrationError(8.1, 8), 0.1, 1e-12); }
TEST(Algorithms, ErrorSymmetry) { EXPECT_DOUBLE_EQ(integrationError(2, 5), integrationError(5, 2)); }
TEST(Errors, InvalidIntervalAllMethods) {
    EXPECT_THROW(rectangle(linear, 2, 0, 2), InvalidIntervalException);
    EXPECT_THROW(trapezoid(linear, 0, 0, 2), InvalidIntervalException);
    EXPECT_THROW(simpson(linear, 2, 0, 2), InvalidIntervalException);
}
TEST(Errors, ZeroSubdivisionsAllMethods) {
    EXPECT_THROW(rectangle(linear, 0, 2, 0), std::invalid_argument);
    EXPECT_THROW(trapezoid(linear, 0, 2, 0), std::invalid_argument);
    EXPECT_THROW(simpson(linear, 0, 2, 0), std::invalid_argument);
}
TEST(Errors, NegativeSubdivisions) { EXPECT_THROW(rectangle(linear, 0, 2, -1), std::invalid_argument); }
TEST(Errors, ResourceLimit) { EXPECT_THROW(trapezoid(linear, 0, 2, MAX_SUBDIVISIONS+1), std::invalid_argument); }
TEST(Errors, OddSimpsonCount) {
    EXPECT_THROW(simpson(linear, 0, 2, 3), SimpsonSubdivisionException);
    EXPECT_NO_THROW(rectangle(linear, 0, 2, 3));
    EXPECT_NO_THROW(trapezoid(linear, 0, 2, 3));
}
TEST(Errors, NonFiniteBounds) {
    EXPECT_THROW(rectangle(linear, nanValue, 2, 2), std::invalid_argument);
    EXPECT_THROW(trapezoid(linear, 0, inf, 2), std::invalid_argument);
    EXPECT_THROW(simpson(linear, -inf, 2, 2), std::invalid_argument);
}
TEST(Errors, EmptyFunction) {
    MathFunc empty;
    EXPECT_THROW(rectangle(empty, 0, 2, 2), std::invalid_argument);
    EXPECT_THROW(trapezoid(empty, 0, 2, 2), std::invalid_argument);
    EXPECT_THROW(simpson(empty, 0, 2, 2), std::invalid_argument);
}
TEST(Errors, NonFiniteFunctionValues) {
    EXPECT_THROW(rectangle([](double){return nanValue;}, 0, 2, 2), InvalidFunctionValueException);
    EXPECT_THROW(trapezoid([](double){return inf;}, 0, 2, 2), InvalidFunctionValueException);
    EXPECT_THROW(simpson([](double){return -inf;}, 0, 2, 2), InvalidFunctionValueException);
}
TEST(Errors, ResultOverflow) {
    EXPECT_THROW(rectangle([](double){return 1e308;}, 0, 10, 2), std::overflow_error);
    EXPECT_THROW(trapezoid([](double){return 1e308;}, 0, 10, 2), std::overflow_error);
    EXPECT_THROW(simpson([](double){return 1e308;}, 0, 10, 2), std::overflow_error);
}
TEST(Errors, IntervalWidthOverflow) { EXPECT_THROW(rectangle(linear, -1e308, 1e308, 2), std::overflow_error); }
TEST(Errors, ErrorFunctionRejectsInvalidInput) {
    EXPECT_THROW(integrationError(inf, 2), std::invalid_argument);
    EXPECT_THROW(integrationError(2, nanValue), std::invalid_argument);
    EXPECT_THROW(integrationError(1e308, -1e308), std::overflow_error);
}
TEST(Errors, StepNotRepresentable) {
    EXPECT_THROW(rectangle(linear, 1.0, std::nextafter(1.0, 2.0), 100), std::invalid_argument);
}
TEST(Task, ValidConstruction) {
    IntegrationTask t(0, 2, 100, .001);
    EXPECT_DOUBLE_EQ(t.left(), 0); EXPECT_DOUBLE_EQ(t.right(), 2);
    EXPECT_EQ(t.subdivisions(), 100); EXPECT_DOUBLE_EQ(t.tolerance(), .001);
    EXPECT_FALSE(t.hasResults()); EXPECT_THROW(t.results(), std::logic_error);
}
TEST(Task, RejectsInvalidConstructorArguments) {
    EXPECT_THROW(IntegrationTask(2, 0, 2), InvalidIntervalException);
    EXPECT_THROW(IntegrationTask(0, 2, 0), std::invalid_argument);
    EXPECT_THROW(IntegrationTask(0, 2, 2, 0), std::invalid_argument);
    EXPECT_THROW(IntegrationTask(0, 2, 2, -1), std::invalid_argument);
    EXPECT_THROW(IntegrationTask(0, 2, 2, inf), std::invalid_argument);
    EXPECT_THROW(IntegrationTask(0, 2, 2, nanValue), std::invalid_argument);
}
TEST(Task, AllThreeResults) {
    IntegrationTask t(0, 2, 100); t.computeAll(quadratic); const auto r=t.results();
    EXPECT_TRUE(t.hasResults()); EXPECT_NEAR(r.rectangle,7.9998,1e-10);
    EXPECT_NEAR(r.trapezoid,8.0004,1e-10); EXPECT_NEAR(r.simpson,8,1e-10);
}
TEST(Task, ToleranceComparison) {
    IntegrationTask t(0, 2, 100,.001); t.computeAll(quadratic);
    EXPECT_TRUE(t.withinTolerance(8)); EXPECT_FALSE(t.withinTolerance(9));
    EXPECT_THROW(t.withinTolerance(inf),std::invalid_argument);
}
TEST(Task, SuccessfulParameterChangeResetsResults) {
    IntegrationTask t(0, 2, 100);t.computeAll(quadratic);t.setParameters(1,3,20,.01);
    EXPECT_DOUBLE_EQ(t.left(),1);EXPECT_DOUBLE_EQ(t.right(),3);
    EXPECT_EQ(t.subdivisions(),20);EXPECT_DOUBLE_EQ(t.tolerance(),.01);
    EXPECT_FALSE(t.hasResults());EXPECT_THROW(t.results(),std::logic_error);
}
TEST(Task, ClearResultsNoThrow) {
    IntegrationTask t(0,2,100);t.computeAll(quadratic);
    static_assert(noexcept(t.clearResults()));
    t.clearResults();EXPECT_FALSE(t.hasResults());EXPECT_NO_THROW(t.clearResults());
}
TEST(StrongGuarantee, InvalidParametersPreserveEntireState) {
    IntegrationTask t(0,2,100,.001);t.computeAll(quadratic);const auto before=t.results();
    EXPECT_THROW(t.setParameters(1,3,20,0),std::invalid_argument);
    EXPECT_EQ(t.results(),before);EXPECT_DOUBLE_EQ(t.left(),0);EXPECT_DOUBLE_EQ(t.right(),2);
    EXPECT_EQ(t.subdivisions(),100);EXPECT_DOUBLE_EQ(t.tolerance(),.001);
    EXPECT_THROW(t.setParameters(3,1,20,.1),InvalidIntervalException);
    EXPECT_EQ(t.results(),before);
}
TEST(StrongGuarantee, FailureAfterFirstMethodPreservesResults) {
    IntegrationTask t(0,2,100);t.computeAll(quadratic);const auto before=t.results();
    const MathFunc bad=[](double x){if(x==0)throw InvalidFunctionValueException(x);return x;};
    EXPECT_THROW(t.computeAll(bad),InvalidFunctionValueException);
    EXPECT_TRUE(t.hasResults());EXPECT_EQ(t.results(),before);
    EXPECT_DOUBLE_EQ(t.left(),0);EXPECT_DOUBLE_EQ(t.right(),2);EXPECT_EQ(t.subdivisions(),100);
}
TEST(StrongGuarantee, FailureAfterSecondMethodKeepsNoResult) {
    IntegrationTask t(0,2,3);
    EXPECT_THROW(t.computeAll(linear),SimpsonSubdivisionException);
    EXPECT_FALSE(t.hasResults());EXPECT_THROW(t.results(),std::logic_error);
}
TEST(StrongGuarantee, CallbackExceptionPreservesResults) {
    IntegrationTask t(0,2,10);t.computeAll(quadratic);const auto before=t.results();
    EXPECT_THROW(t.computeAll([](double)->double{throw std::domain_error("помилка функції");}),std::domain_error);
    EXPECT_EQ(t.results(),before);
}
TEST(StrongGuarantee, OverflowPreservesResults) {
    IntegrationTask t(0,2,100);t.computeAll(quadratic);const auto before=t.results();
    EXPECT_THROW(t.computeAll([](double){return 1e308;}),std::overflow_error);
    EXPECT_EQ(t.results(),before);
}
TEST(StrongGuarantee, SuccessfulRecalculationReplacesAllResults) {
    IntegrationTask t(0,2,100);t.computeAll(quadratic);t.computeAll(linear);
    EXPECT_NEAR(t.results().rectangle,2,1e-12);EXPECT_NEAR(t.results().trapezoid,2,1e-12);
    EXPECT_NEAR(t.results().simpson,2,1e-12);
}
TEST(ExceptionMechanism, RethrowPreservesType) {
    auto f=[]{try{throw SimpsonSubdivisionException(3);}catch(const IntegrationException&){throw;}};
    EXPECT_THROW(f(),SimpsonSubdivisionException);
    static_assert(std::is_base_of_v<std::runtime_error,IntegrationException>);
}
TEST(ExceptionMechanism, DiagnosticData) {
    SimpsonSubdivisionException e(3);EXPECT_EQ(e.count(),3);
    InvalidFunctionValueException value(1.5);EXPECT_DOUBLE_EQ(value.argument(),1.5);
}
TEST(ExceptionMechanism, RaiiUnwinding) {
    struct Guard {bool& destroyed;~Guard()noexcept{destroyed=true;}};
    bool destroyed=false;
    try{Guard g{destroyed};throw SimpsonSubdivisionException(3);}catch(const IntegrationException&){}
    EXPECT_TRUE(destroyed);
}
