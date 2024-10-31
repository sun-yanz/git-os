#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <windows.h>
#include <process.h>

using namespace std;

// Структура для передачи данных в поток worker
struct ThreadData {
    size_t size;
    long* array;
};

// Функция для потока worker
unsigned __stdcall WorkerThread(void* data) {
    ThreadData* threadData = static_cast<ThreadData*>(data);
    long sumOfSquares = 0;

    for (size_t i = 0; i < threadData->size; ++i) {
        sumOfSquares += threadData->array[i] * threadData->array[i];
        this_thread::sleep_for(chrono::milliseconds(12)); // Пауза 12 мс
    }

    cout << "Сумма квадратов элементов: " << sumOfSquares << endl;

    return 0;
}

int main() {
    setlocale(0, "rus");
    size_t size;
    cout << "Введите размер массива: ";
    cin >> size;

    vector<long> array(size);
    cout << "Введите элементы массива: ";
    for (size_t i = 0; i < size; ++i) {
        cin >> array[i];
    }

    // Ввод времени для остановки и запуска потока
    int waitTime;
    cout << "Введите время ожидания в миллисекундах: ";
    cin >> waitTime;

    // Создание структуры данных для передачи в поток
    ThreadData data = { size, array.data() };

    // Создание потока в подвешенном состоянии
    uintptr_t threadHandle = _beginthreadex(nullptr, 0, WorkerThread, &data, 0, nullptr);

    // Ожидание перед запуском потока
    this_thread::sleep_for(chrono::milliseconds(waitTime));

    // Запуск потока
    ResumeThread(reinterpret_cast<HANDLE>(threadHandle));

    // Ожидание завершения потока без WaitForSingleObject
    while (WaitForSingleObject(reinterpret_cast<HANDLE>(threadHandle), 0) == WAIT_TIMEOUT) {
        this_thread::sleep_for(chrono::milliseconds(100)); // Проверка каждые 100 мс
    }

    // Закрытие дескриптора потока
    CloseHandle(reinterpret_cast<HANDLE>(threadHandle));

    return 0;
}