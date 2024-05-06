#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Node {
    int payload;
    struct Node* next;
} node;

char* two_d_alloc(size_t N, size_t M, size_t sz) {
    return (char*)malloc(N * M * sz);
}

void two_d_dealloc(char* array) {
    free(array);
}

int is_valid_index(size_t i, size_t j, size_t M, size_t N) {
    return (i < N) && (j < M);
}

int two_d_store_int(int arg, char* array, size_t i, size_t j, size_t M, size_t N) {
    if (array == NULL || i >= M || j >= N) {
        return -1; // Error
    }

    int* ptr = (int*)(array + (i * M + j) * sizeof(int));
    *ptr = arg;

    return 0;
}

int two_d_fetch_int(char* array, size_t i, size_t j, size_t M, size_t N) {
    if (array == NULL || i >= M || j >= N) {
        printf("Error: Invalid arguments\n");
        return -1; // Error
    }

    int* ptr = (int*)(array + (i * M + j) * sizeof(int));
    return *ptr;

    return 0;
}

int two_d_store(void* arg, char* array, size_t i, size_t j, size_t M, size_t N, size_t sz) {
    if (array == NULL || i >= M || j >= N) {
        return -1; // Error
    }

    char* ptr = array + (i * M + j) * sz;
    memcpy(ptr, arg, sz);

    return 0;
}

void* two_d_fetch(char* array, size_t i, size_t j, size_t M, size_t N, size_t sz) {
    if (array == NULL || i >= M || j >= N) {
        printf("Error: Invalid arguments\n");
        return NULL; // Error
    }

    char* ptr = array + (i * M + j) * sz;
    return (void*)ptr;
}

void print_array(char* array, size_t M, size_t N, size_t sz) {
    uintptr_t addr = (uintptr_t)array;

    for (size_t i = 0; i < N; ++i) {
        printf("%p\t", (void*)addr);

        for (size_t j = 0; j < M * sz; ++j) {
            printf("%02x ", array[i * M * sz + j]);
        }

        printf("\n");
        addr += M * sz;
    }
}

void test_Int() {
    size_t numOfRows = 2, numOfColumns = 4, elemSize = sizeof(int);
    char* buffer = two_d_alloc(numOfRows, numOfColumns, elemSize);
    size_t rowIdx, colIdx;
    int inputValue, fetchedValue, failedTests = 0;

    for (int test = 0; test < 100; test++) {
        rowIdx = abs(rand()) % numOfRows;
        colIdx = abs(rand()) % numOfColumns;
        inputValue = rand() % 10000;
        two_d_store_int(inputValue, buffer, rowIdx, colIdx, numOfRows, numOfColumns);
        fetchedValue = two_d_fetch_int(buffer, rowIdx, colIdx, numOfRows, numOfColumns);
        printf("Stored Value = %d, Retrieved Value = %d\n", inputValue, fetchedValue);

        if (inputValue != fetchedValue) {
            failedTests++;
        }
    }

    printf("Number of failed tests: %d\n", failedTests);
    two_d_dealloc(buffer);
}


void test_Floats() {
    size_t numOfRows = 3, numOfColumns = 4, elemSize = sizeof(float);
    char* arrayBuffer = two_d_alloc(numOfRows, numOfColumns, elemSize);

    float inputValue, fetchedValue;
    float failedTestCount = 0;
    float epsilon = 0.0001;
    size_t rowIndex, colIndex;

    for (int test = 0; test < 10; test++) {
        rowIndex = abs(rand()) % numOfRows;
        colIndex = abs(rand()) % numOfColumns;
        inputValue = (float)rand() / RAND_MAX * 100;

        two_d_store(&inputValue, arrayBuffer, rowIndex, colIndex, numOfRows, numOfColumns, elemSize);
        float* fetchedPtr = (float*)two_d_fetch(arrayBuffer, rowIndex, colIndex, numOfRows, numOfColumns, elemSize);

        if (fetchedPtr != NULL) {
            fetchedValue = *fetchedPtr;
        }

        printf("Input Value = %f, Fetched Value = %f\n", inputValue, fetchedValue);

        if (fabs(inputValue - fetchedValue) > epsilon) {
            failedTestCount++;
        }
    }

    printf("Number of failed tests: %f\n", failedTestCount);
    two_d_dealloc(arrayBuffer);
}



void test_Array() {
    size_t rows = 3, columns = 4, elementSize = sizeof(int);
    char* buffer = two_d_alloc(rows, columns, elementSize);
    int value = 1;

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < columns; col++) {
            int* currentCell = (int*)(buffer + (row * columns + col) * elementSize);
            *currentCell = value;
            value++;
        }
    }

    printf("Array Contents:\n");
    print_array(buffer, rows, columns, elementSize);

    two_d_dealloc(buffer);
}



int main() {
    //test_Int();
    test_Floats();
    //test_Array();

    return 0;
}


