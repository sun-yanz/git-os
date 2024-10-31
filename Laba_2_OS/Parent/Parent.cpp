#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include<conio.h>

int main() {
    setlocale(0, "rus");
    // Ввод размера массива
    int size;
    std::cout << "Введите размер массива: ";
    std::cin >> size;

    // Ввод элементов массива
    std::vector<char> array(size);
    std::cout << "Введите элементы массива: ";
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    // Формирование командной строки
    std::wstring commandLine = L"Child.exe ";
    commandLine += std::to_wstring(size) + L" ";
    for (const auto& elem : array) {
        commandLine += elem + L" ";
    }

    // Настройка STARTUPINFO - структура в Windows API
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Установка параметров окна Child
    si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    si.dwX = 50; // X смещение
    si.dwY = 0; // Y смещение
    si.dwXSize = 150;
    si.dwYSize = 50;
    ZeroMemory(&pi, sizeof(pi));

    // Запуск дочернего процесса
    if (!CreateProcess(NULL, &commandLine[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        
        std::cerr << "Не удалось запустить процесс Child." << std::endl;
        return 1;
    }

    // Ожидание завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закрытие дескрипторов
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    _getch();

    return 0;
}
