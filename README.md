# cef-overlay-samp

## Порядок действий для сборки и запуска проекта

### 1. Скачивание и настройка CEF
1. Скачайте win32 билд CEF с [официального сайта](https://cef-builds.spotifycdn.com/index.html).
2. Распакуйте скачанный архив в директорию `C:\cef_binary`.

### 2. Сборка проекта
1. Соберите проект с помощью CMake:
   cmake -B project -A Win32

    Скомпилируйте проект.

3. Настройка библиотек и ресурсов

    Переместите файл libcef_dll_wrapper.lib в директорию C:\cef_binary\Release.

    Скопируйте следующие директории в C:\cef_binary\Release:

        include

        Содержимое папки Resources

4. Установка DirectX SDK

    Убедитесь, что DirectX SDK June 2010 установлен по пути C:/Program Files (x86)/DirectX SDK.

5. Установка ImGui через vcpkg

    Установите ImGui с необходимыми компонентами через vcpkg:

    vcpkg install imgui[core,dx9-binding,win32-binding]:x86-windows-static

6. Сборка обоих проектов

    Соберите оба проекта с помощью CMake:
    bash
    Copy

    cmake -B project -A Win32

7. Копирование файлов в папку с игрой

    Скопируйте cef_sub-process.exe и все файлы (кроме include) из папки C:\cef_binary\Release в папку с игрой.

    Поместите cef-overlay-samp.asi в папку с игрой.

### 3. Запуск

После выполнения всех вышеуказанных шагов, запустите игру San Andreas Multiplayer. Оверлей должен появиться в игре при нажатии кнопки END.
