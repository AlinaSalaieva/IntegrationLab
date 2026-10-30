# Лабораторна робота №7 — варіант 1

Чисельне інтегрування. Обробка винятків та модульне тестування.
C++20, Visual Studio, CMake, vcpkg, Google Test, CTest.

## Основа проєкту

Використано власну бібліотеку лабораторної №4:
https://github.com/AlinaSalaieva/lab-4

Коміт: 4cc079490e3ecd6aec4c2cdd4de7775a39cb2fe8.
Алгоритми з 1_IntegrationLib/MathLib/src/NumericalIntegration.cpp:
rectangle (середні прямокутники), trapezoid, simpson, integrationError.
Їх збережено та доповнено перевірками. Замість DLL збирається статична
IntegrationLib. Автоматичне збільшення непарного n у методі Сімпсона замінено
винятком, як вимагає №7. Повернення 0 при неправильних аргументах замінено
винятками. MathFunc розширено до std::function для підтримки звичайних функцій
та лямбда-виразів. Клієнти чужої NumericalDiff та результати компіляції з №4
до цього проєкту не включені.

## Файли

| Файл | Призначення |
|---|---|
| include/NumericalIntegration.h, src/NumericalIntegration.cpp | Алгоритми №4 з перевірками |
| include/IntegrationTask.h, src/IntegrationTask.cpp | Стан задачі та сильна гарантія |
| include/IntegrationExceptions.h | Власна ієрархія винятків |
| include/ScopeLogger.h | RAII та порядок деструкторів |
| demo/main.cpp | Обчислення, помилки, strong guarantee та три рівні викликів |
| antipattern/main.cpp | int/string, catch(...), throw e; і throw; |
| tests/IntegrationTests.cpp | Модульні тести Google Test |
| CMakeLists.txt, CMakePresets.json | Бібліотека STATIC, демонстрації, тести, CTest |
| vcpkg.json | Google Test та зафіксований baseline |
| docs/SCREENSHOTS.md | Покроковий запуск і перелік скриншотів |
| docs/REPORT_GUIDE.md | Дев'ять пунктів звіту |
| docs/TESTS.md | Перелік тестів |
| docs/VERIFICATION.md | Результати перевірки підготовленого коду |

## Запуск у Visual Studio без батників

1. У Visual Studio Installer установити розробку класичних програм мовою C++,
   MSVC, Windows SDK, C++ CMake tools for Windows та vcpkg (якщо компонент доступний).
2. Розпакувати проєкт, наприклад у C:/Projects/TPP_Lab7_Integration_UA.
3. Файл → Відкрити → Папка: вибрати папку з CMakeLists.txt.
4. Вибрати x64-debug та дочекатися конфігурації CMake і встановлення Google Test.
   Потрібен інтернет. CMake шукає vcpkg у VC/vcpkg останньої Visual Studio
   або в каталозі зі змінної середовища VCPKG_ROOT.
5. Збирання → Зібрати все.
6. Вибрати IntegrationDemo.exe у списку запуску, натиснути Ctrl+F5.
7. Аналогічно запустити IntegrationTests.exe та AntipatternDemo.exe.

Якщо vcpkg не знайдено, задати змінну середовища користувача VCPKG_ROOT
зі шляхом до встановленого vcpkg, де є scripts/buildsystems/vcpkg.cmake.
Повністю перезапустити Visual Studio та очистити кеш CMake перед повторною
конфігурацією. Код алгоритмів змінювати не потрібно.

Команди в Developer PowerShell Visual Studio з кореня проєкту:

```powershell
cmake --preset x64-debug
cmake --build out/build/x64-debug
ctest --test-dir out/build/x64-debug --output-on-failure
.\out\build\x64-debug\IntegrationDemo.exe
.\out\build\x64-debug\IntegrationTests.exe
.\out\build\x64-debug\AntipatternDemo.exe
```

## Контракт та інваріанти

IntegrationTask зберігає межі a, b, кількість розбиттів n, точність tolerance
і необов'язкові результати трьох методів. a і b скінченні, a < b;
n від 1 до 1000000; tolerance — додатне скінченне число. Крок повинен
бути додатним і придатним для подання у double. Непарне n дозволено для
прямокутників і трапецій, але simpson та computeAll відхиляють його.

Якщо результати існують, усі три є скінченними та відповідають останньому
успішному computeAll для поточних параметрів. Зовнішній код отримує лише копію
результатів. Зміна параметрів після повної перевірки скидає старі результати.
results() до успішного розрахунку генерує std::logic_error.

tolerance застосовується у withinTolerance(reference): усі три абсолютні
похибки мають бути не більшими за tolerance. Кількість розбиттів задається
явно; автоматичний підбір n тут не реалізовано. integrationError з №4
обчислює абсолютну різницю; це не доведена верхня межа похибки інтегрування.

## Винятки

std::runtime_error → IntegrationException → InvalidIntervalException,
InvalidFunctionValueException, SimpsonSubdivisionException.
Останні два класи також зберігають точку x або непарне n для діагностики.

| Умова | Тип винятку |
|---|---|
| a >= b | InvalidIntervalException |
| Нульове, від'ємне або надмірне n | std::invalid_argument |
| Непарне n для Сімпсона | SimpsonSubdivisionException |
| NaN/Inf у межах, точності чи еталоні | std::invalid_argument |
| NaN/Inf, повернені функцією | InvalidFunctionValueException |
| Порожня функція | std::invalid_argument |
| Переповнення проміжного або кінцевого результату | std::overflow_error |
| Результат ще не обчислено | std::logic_error |

Перевірки double явні, оскільки арифметичне переповнення саме по собі
не гарантує винятку C++. Винятки користувацької функції поширюються назовні.

## Гарантії безпеки

| Метод | Гарантія |
|---|---|
| setParameters | strong: усі перевірки до запису, після успіху скидання результатів |
| computeAll | strong: три локальні результати, потім фіксація простого стану |
| clearResults | no-throw, noexcept |
| Копіювальне присвоювання | no-throw, noexcept |
| Деструктор | no-throw, noexcept |

Гарантії зазначено також у коментарях до методів.
computeAll обчислює rectangle, trapezoid та simpson у тимчасовій структурі.
Присвоєння optional зі структурою трьох double не генерує винятків;
це додатково перевіряється static_assert(noexcept(...)). Якщо будь-який
метод завершується винятком, попередні результати та параметри не змінюються.
Гарантія охоплює IntegrationTask; побічні ефекти довільної користувацької
функції на зовнішні об'єкти бібліотека відкотити не може.

## Очікуваний порядок RAII — прогноз до запуску

```text
[ВХІД] level1
[ВХІД] level2
[ВХІД] level3
[ВИХІД] level3
[ЖУРНАЛ] level2: Метод Сімпсона потребує парної кількості розбиттів: n=3
[ВИХІД] level2
[ВИХІД] level1
[ГОЛОВНА] Метод Сімпсона потребує парної кількості розбиттів: n=3
Програма продовжує роботу.
```

Прочитати цей прогноз до демонстрації; після запуску порівняти його з
фактичним результатом. ScopeLogger показує зворотний порядок деструкторів.
std::unique_ptr автоматично звільняє IntegrationTask при винятку.
Проміжний обробник level2 використовує const& та throw;, зберігаючи тип.
Деструктор ScopeLogger noexcept перехоплює можливі помилки журналювання.

## Негативний самоконтроль

Перемкнути Visual Studio на x64-negative, зібрати та запустити IntegrationTests.
LAB7_INJECT_BUG вмикає передчасний запис результату прямокутників і нулів
для двох інших методів. Якщо наступний метод завершується помилкою,
старі результати вже втрачено. Тести сильної гарантії повинні показати FAILED.
Це очікуваний результат навчального самоконтролю.

Для наочного запуску одного тесту:

```powershell
.\out\build\x64-negative\IntegrationTests.exe --gtest_filter=StrongGuarantee.FailureAfterFirstMethodPreservesResults
```

Після скриншота повернутися до x64-debug, зібрати та повторно запустити
всі тести. Основна конфігурація при здачі — x64-debug.

## Антипатерни

AntipatternDemo показує, що int та std::string не належать до std::exception,
але перехоплюються catch(...). Стандартні та власні класи надають what()
і підтримують ієрархію обробників. throw e; при перехопленні базового типу
може зрізати похідну частину; throw; зберігає початковий тип.

## GitHub

Завантажити всі вихідні файли цього архіву у новий репозиторій лабораторної №7.
Не додавати out, build, .vs, vcpkg_installed, виконувані файли та бібліотеки.
.gitignore містить потрібні виключення. При завантаженні через браузер
відбирати файли вручну — .gitignore не фільтрує браузерне завантаження.
Посилання на новий репозиторій додати до звіту після публікації.
