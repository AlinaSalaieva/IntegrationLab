# Запуск і скриншоти

1. Відкрити папку проєкту у Visual Studio. Вибрати x64-debug, дочекатися
   налаштування CMake та виконати «Зібрати все».
2. Прочитати прогноз RAII у README до запуску демонстрації.
3. Вибрати IntegrationDemo.exe та натиснути Ctrl+F5.

Зробити такі скриншоти:

1. Початкові обчислення f(x)=3*x*x на [0,2], n=100:
   прямокутники 7.9998, трапеції 8.0004, Сімпсон 8.
   Похибка Сімпсона може бути дуже малим ненульовим числом через double.
2. Сценарії помилок 1–7 з типами винятків.
3. Сильна гарантія: результати до й після помилки другого методу,
   рядок «Усі результати збережено: true».
4. Повний блок RAII із входами, виходами, журналом level2 та обробкою в main.
5. Запустити AntipatternDemo.exe: зняти int/string, catch(...) і throw;.
6. Запустити IntegrationTests.exe у x64-debug: зняти підсумок PASSED 35 tests.
7. Перемкнутися на x64-negative, зібрати та запустити IntegrationTests.exe.
   Зняти помилку StrongGuarantee.FailureAfterFirstMethodPreservesResults
   та підсумок FAILED. Це навмисна помилка, так і повинно бути.
8. Повернутися до x64-debug, зібрати та повторно запустити IntegrationTests.exe.
   Зняти PASSED 35 tests після відновлення правильної реалізації.
9. Відкрити Developer PowerShell у Visual Studio і виконати з кореня проєкту:

```powershell
ctest --test-dir out/build/x64-debug --output-on-failure
```

Зняти підсумок «100% tests passed, 0 tests failed out of 35».

Для показу лише одного тесту негативного самоконтролю можна виконати:

```powershell
.\out\build\x64-negative\IntegrationTests.exe --gtest_filter=StrongGuarantee.FailureAfterFirstMethodPreservesResults
```

Результати зберігаються як трійка: прямокутники, трапеції, Сімпсон.
У цьому тесті правильна попередня трійка приблизно {7.9998, 8.0004, 8},
а дефектна — {2, 0, 0}. Перевірка типу винятку проходить, перевірка
незмінності результатів — ні. Після повернення до x64-debug всі тести
мають пройти. Залишити x64-debug основною конфігурацією для здачі.
