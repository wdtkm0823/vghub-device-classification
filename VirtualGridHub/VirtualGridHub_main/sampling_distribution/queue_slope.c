#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define MAX_QUEUE_SIZE 5

float randomFloat() {
    return (float)rand() / RAND_MAX;
}

typedef struct {
    float voltage;
} VoltageData;

typedef struct {
    VoltageData data[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
} VoltageQueue;

void initQueue(VoltageQueue* queue);
int isQueueEmpty(VoltageQueue* queue);
int isQueueFull(VoltageQueue* queue);
void enqueue(VoltageQueue* queue, float voltage);
VoltageData dequeue(VoltageQueue* queue);
float calculateSlope(VoltageQueue* queue);
void displayQueue(VoltageQueue* queue);

void initQueue(VoltageQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

int isQueueEmpty(VoltageQueue* queue) {
    return (queue->size == 0);
}

int isQueueFull(VoltageQueue* queue) {
    return (queue->size == MAX_QUEUE_SIZE);
}

void enqueue(VoltageQueue* queue, float voltage) {
    if (isQueueFull(queue)) {
        dequeue(queue);  // キューが満杯の場合、最も古い値を削除する
    }

    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    queue->data[queue->rear].voltage = voltage;
    queue->size++;
}

VoltageData dequeue(VoltageQueue* queue) {
    VoltageData item;
    if (isQueueEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue voltage.\n");
        item.voltage = 0.0;  // 適当なデフォルト値を返す
    } else {
        item = queue->data[queue->front];
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
        queue->size--;
    }
    return item;
}

float calculateSlope(VoltageQueue* queue) {
    if (queue->size < 2) {
        printf("Insufficient data points. Cannot calculate slope.\n");
        return 0.0;
    }

    float sumX = 0.0;
    float sumY = 0.0;
    float sumXY = 0.0;
    float sumX2 = 0.0;
    int numValues = queue->size;

    VoltageQueue tempQueue = *queue;
    while (!isQueueEmpty(&tempQueue)) {
        VoltageData voltageData = dequeue(&tempQueue);
        float x = numValues - tempQueue.size;
        float y = voltageData.voltage;
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    float meanX = sumX / numValues;
    float meanY = sumY / numValues;

    float numerator = sumXY - numValues * meanX * meanY;
    float denominator = sumX2 - numValues * meanX * meanX;

    if (denominator == 0.0) {
        printf("Cannot calculate slope. Denominator is zero.\n");
        return 0.0;
    }

    float slope = numerator / denominator;
    return slope;
}

void displayQueue(VoltageQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty.\n");
    } else {
        printf("Queue Contents: ");
        int i = queue->front;
        int count = 0;
        while (count < queue->size) {
            printf("%.2f ", queue->data[i].voltage);
            i = (i + 1) % MAX_QUEUE_SIZE;
            count++;
        }
        printf("\n");
    }
}

int main() {
    int i;
    float current_slope;
    VoltageQueue voltageQueue;
    initQueue(&voltageQueue);

    // 乱数のシードを設定（初期化）
    srand(time(NULL));

    FILE *dataFile = fopen("data.txt", "w"); // データを書き込むファイルを開く

    for (i = 0; i < 10; i++) {
        // ランダムな浮動小数点数を生成してキューに挿入
        float randomValue = randomFloat();
        enqueue(&voltageQueue, randomValue);
        displayQueue(&voltageQueue);

        if (voltageQueue.size >= 2) {
            current_slope = calculateSlope(&voltageQueue);
            printf("Current Slope: %f\n", current_slope);

            // データをファイルに書き込む
            fprintf(dataFile, "%d %f\n", i, current_slope);
        }
    }

    fclose(dataFile); // ファイルを閉じる

    return 0;
}






