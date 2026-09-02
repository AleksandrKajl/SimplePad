# SimplePad

Простой текстовый редактор на C++ и Qt Widgets.

## Сборка

Для сборки нужны CMake 3.16 или новее, компилятор с поддержкой C++17 и Qt 5 или Qt 6 с модулями `Widgets` и `PrintSupport`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Исполняемый файл будет создан в каталоге `build`.

В Qt Creator нужно открыть корневой `CMakeLists.txt` и выбрать подходящий комплект Qt.
