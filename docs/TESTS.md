# Перелік тестів

Перевірка підготовленого коду: 35 PASSED. Результати власного запуску у Visual Studio додати до звіту.

| № | Тест | Призначення | Результат перевірки |
|---|---|---|---|
| 1 | Algorithms.OriginalRectangle | Метод прямокутників із прикладу №4 | PASSED |
| 2 | Algorithms.OriginalTrapezoid | Метод трапецій із прикладу №4 | PASSED |
| 3 | Algorithms.OriginalSimpson | Метод Сімпсона із прикладу №4 | PASSED |
| 4 | Algorithms.ConstantFunction | Стала функція для всіх методів | PASSED |
| 5 | Algorithms.LinearMinimumSubdivision | Лінійна функція за мінімальної кількості розбиттів | PASSED |
| 6 | Algorithms.SineIntegral | Інтеграл синуса | PASSED |
| 7 | Algorithms.ErrorCalculation | Обчислення абсолютної різниці | PASSED |
| 8 | Algorithms.ErrorSymmetry | Симетричність абсолютної різниці | PASSED |
| 9 | Errors.InvalidIntervalAllMethods | Некоректний інтервал для всіх методів | PASSED |
| 10 | Errors.ZeroSubdivisionsAllMethods | Нульове n для всіх методів | PASSED |
| 11 | Errors.NegativeSubdivisions | Від’ємне n | PASSED |
| 12 | Errors.ResourceLimit | Ліміт обчислювальних ресурсів | PASSED |
| 13 | Errors.OddSimpsonCount | Непарне n лише для Сімпсона | PASSED |
| 14 | Errors.NonFiniteBounds | NaN/Inf у межах | PASSED |
| 15 | Errors.EmptyFunction | Порожня функція | PASSED |
| 16 | Errors.NonFiniteFunctionValues | NaN/Inf у значеннях функції | PASSED |
| 17 | Errors.ResultOverflow | Переповнення результатів | PASSED |
| 18 | Errors.IntervalWidthOverflow | Переповнення ширини інтервалу | PASSED |
| 19 | Errors.ErrorFunctionRejectsInvalidInput | Некоректні аргументи оцінки похибки | PASSED |
| 20 | Errors.StepNotRepresentable | Крок, непридатний для подання double | PASSED |
| 21 | Task.ValidConstruction | Конструктор і відсутність результату до обчислення | PASSED |
| 22 | Task.RejectsInvalidConstructorArguments | Перевірки аргументів конструктора та точності | PASSED |
| 23 | Task.AllThreeResults | Одночасне отримання трьох результатів | PASSED |
| 24 | Task.ToleranceComparison | Порівняння із заданою точністю | PASSED |
| 25 | Task.SuccessfulParameterChangeResetsResults | Успішна зміна параметрів та скидання результатів | PASSED |
| 26 | Task.ClearResultsNoThrow | No-throw очищення результатів | PASSED |
| 27 | StrongGuarantee.InvalidParametersPreserveEntireState | Strong: невдала зміна параметрів | PASSED |
| 28 | StrongGuarantee.FailureAfterFirstMethodPreservesResults | Strong: помилка другого методу | PASSED |
| 29 | StrongGuarantee.FailureAfterSecondMethodKeepsNoResult | Strong: помилка третього методу без попереднього результату | PASSED |
| 30 | StrongGuarantee.CallbackExceptionPreservesResults | Strong: виняток користувацької функції | PASSED |
| 31 | StrongGuarantee.OverflowPreservesResults | Strong: переповнення | PASSED |
| 32 | StrongGuarantee.SuccessfulRecalculationReplacesAllResults | Успішне повторне обчислення | PASSED |
| 33 | ExceptionMechanism.RethrowPreservesType | Збереження похідного типу при throw; | PASSED |
| 34 | ExceptionMechanism.DiagnosticData | Додаткові діагностичні дані | PASSED |
| 35 | ExceptionMechanism.RaiiUnwinding | RAII під час розкручування стека | PASSED |

Негативний самоконтроль повинен виявити дефект у FailureAfterFirstMethodPreservesResults та FailureAfterSecondMethodKeepsNoResult. Після вимкнення дефекту всі тести проходять.
