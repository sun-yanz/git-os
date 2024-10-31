#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <windows.h>
#include <process.h>

using namespace std;

// ��������� ��� �������� ������ � ����� worker
struct ThreadData {
    size_t size;
    long* array;
};

// ������� ��� ������ worker
unsigned __stdcall WorkerThread(void* data) {
    ThreadData* threadData = static_cast<ThreadData*>(data);
    long sumOfSquares = 0;

    for (size_t i = 0; i < threadData->size; ++i) {
        sumOfSquares += threadData->array[i] * threadData->array[i];
        this_thread::sleep_for(chrono::milliseconds(12)); // ����� 12 ��
    }

    cout << "����� ��������� ���������: " << sumOfSquares << endl;

    return 0;
}

int main() {
    setlocale(0, "rus");
    size_t size;
    cout << "������� ������ �������: ";
    cin >> size;

    vector<long> array(size);
    cout << "������� �������� �������: ";
    for (size_t i = 0; i < size; ++i) {
        cin >> array[i];
    }

    // ���� ������� ��� ��������� � ������� ������
    int waitTime;
    cout << "������� ����� �������� � �������������: ";
    cin >> waitTime;

    // �������� ��������� ������ ��� �������� � �����
    ThreadData data = { size, array.data() };

    // �������� ������ � ����������� ���������
    uintptr_t threadHandle = _beginthreadex(nullptr, 0, WorkerThread, &data, 0, nullptr);

    // �������� ����� �������� ������
    this_thread::sleep_for(chrono::milliseconds(waitTime));

    // ������ ������
    ResumeThread(reinterpret_cast<HANDLE>(threadHandle));

    // �������� ���������� ������ ��� WaitForSingleObject
    while (WaitForSingleObject(reinterpret_cast<HANDLE>(threadHandle), 0) == WAIT_TIMEOUT) {
        this_thread::sleep_for(chrono::milliseconds(100)); // �������� ������ 100 ��
    }

    // �������� ����������� ������
    CloseHandle(reinterpret_cast<HANDLE>(threadHandle));

    return 0;
}