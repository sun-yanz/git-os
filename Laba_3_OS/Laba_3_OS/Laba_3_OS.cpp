#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

std::mutex mtx; // Мьютекс для критической секции
std::condition_variable cv_main; // Сигнал для потока main
std::condition_variable cv_count; // Сигнал для потока CountElement
std::condition_variable cv_work;
bool work_done = false; // Флаг для управления событиями
bool count_done = false; // Флаг для управления событиями
std::vector<char> result_array; // Итоговый массив
std::vector<std::string> input_array; // Исходный массив
int count = 0;

void work(const std::vector<char>& input_array) {
    setlocale(0, "rus");

    int sleepTime;
    std::cout << "Введите временной интервал порядка отдыха после подготовки одного элемента в массиве (в мс): ";
    std::cin >> sleepTime;

    // Поиск количеств лексем
    std::cout << "Введите строку для анализа (лексемы отделены пробелом и тире): ";
    std::string input;
    std::cin.ignore();
    std::getline(std::cin, input);

    std::string delimiter = " - ";
    size_t pos = 0;
    while ((pos = input.find_first_of(" -")) != std::string::npos) {
        std::string token = input.substr(0, pos);
        if (!token.empty() && std::isdigit(token[0])) {
            result_array.push_back(token[0]);
        }
        else {
            result_array.push_back('_');
        }
        input.erase(0, pos + 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
    if (!input.empty() && std::isdigit(input[0])) {
        result_array.push_back(input[0]);
    }
    else {
        result_array.push_back('_');
    }

    // Установить сигнал для потоков main и CountElement
    {
        std::lock_guard<std::mutex> lock(mtx);
        work_done = true;
    }
    cv_main.notify_one(); // Уведомление потока main
    cv_count.notify_one(); // Уведомление потока CountElement
}

void CountElement() {
    setlocale(0, "rus");
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_count.wait(lock, [] { return work_done; }); // Ждем сигнала от work
    }

    for (char ch : result_array) {
        if (ch == '_') break;
        count++;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        count_done = true;
        cv_work.notify_all(); // Уведомляем main о завершении подсчета
    }
}

int main() {
    setlocale(0, "rus");
    int size;
    std::cout << "Введите размерность массива: ";
    std::cin >> size;

    std::vector<char> input_array(size);
    std::cout << "Введите элементы массива (символы): ";
    for (int i = 0; i < size; ++i) {
        std::cin >> input_array[i];
    }

    std::cout << "Исходный массив: ";
    for (char ch : input_array) {
        std::cout << ch << ' ';
    }
    std::cout << std::endl;

    std::thread t_work(work, input_array);
    std::thread t_count(CountElement);

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_work.wait(lock, [] { return work_done; }); // Ждем завершения работы потока work
    }

    std::cout << "Итоговый массив: ";
    for (char ch : result_array) {
        std::cout << ch << ' ';
    }
    std::cout << std::endl;

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_count.wait(lock, [] { return count_done; }); // Ждем завершения подсчета
    }

    std::cout << "Количество элементов до символа '_': " << count << std::endl;

    t_work.join();
    t_count.join();

    return 0;
}
