#include <windows.h>
#include <iostream>
#include <vector>
#include<conio.h>

int main(int argc, char* argv[]) {
    setlocale(0, "rus");
    if (argc < 2) {
        std::cerr << "Недостаточно аргументов." << std::endl;
        return 1;
    }

    // Получение размера массива
    int size = atoi(argv[1]);

    // Получение элементов массива
    std::vector<char> array(size);
    for (int i = 0; i < size; ++i) {
        array[i] = atol(argv[i + 2]);
    }

    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << ' ';
    }

    _getch();
    return 0;
}
