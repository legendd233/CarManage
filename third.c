#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 5
#define FEE_RATE 5

typedef struct {
    char licensePlate[10];
    time_t arrivalTime;
    time_t departureTime;
} Car;

typedef struct {
    Car cars[MAX_SIZE];
    int top;
} Stack;

typedef struct Node {
    Car car;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

void initializeStack(Stack* stack) {
    stack->top = -1;
}

int isStackEmpty(Stack* stack) {
    return (stack->top == -1);
}

int isStackFull(Stack* stack) {
    return (stack->top == MAX_SIZE - 1);
}

void pushToStack(Stack* stack, Car car) {
    if (isStackFull(stack)) {
        printf("ͣ�������������� %s ���ڵȴ���\n", car.licensePlate);
    } else {
        stack->top++;
        stack->cars[stack->top] = car;
    }
}

Car popFromStack(Stack* stack) {
    if (isStackEmpty(stack)) {
        Car emptyCar;
        strcpy(emptyCar.licensePlate, "");
        emptyCar.arrivalTime = -1;
        emptyCar.departureTime = -1;
        return emptyCar;
    } else {
        Car car = stack->cars[stack->top];
        stack->top--;
        return car;
    }
}

void initializeQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(Queue* queue) {
    return (queue->front == NULL);
}

void enqueue(Queue* queue, Car car) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->car = car;
    newNode->next = NULL;

    if (isQueueEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

Car dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        Car emptyCar;
        strcpy(emptyCar.licensePlate, "");
        emptyCar.arrivalTime = -1;
        emptyCar.departureTime = -1;
        return emptyCar;
    } else {
        Node* temp = queue->front;
        Car car = temp->car;
        queue->front = queue->front->next;
        free(temp);
        return car;
    }
}

void clearScreen() {
    system("clear || cls");
}

void printParkingTimeAndFee(Car car) {
    struct tm *localTime;
    time_t currentTime;
    time(&currentTime);



    double parkingTime = difftime(car.departureTime, car.arrivalTime) / 60.0;
    int fee = (int)(parkingTime * FEE_RATE);
    printf("����%s�շ������\n",car.licensePlate);
    localTime = localtime(&car.arrivalTime);
    printf("��������ʱ�䣺%s", asctime(localTime));
    localTime = localtime(&car.arrivalTime);
    printf("�����뿪ʱ�䣺%s", asctime(localTime));
    printf("��ͣ��ʱ����%.2lf Сʱ\nͣ�����ã�%d Ԫ��\n",  parkingTime, fee);
}

int main() {
    Stack parkingLot;
    Queue driveway;
    initializeStack(&parkingLot);
    initializeQueue(&driveway);

    int option;
    char licensePlate[20];
    time_t currentTime;


    Car emptyCar;


    do {
        printf("************************************************\n");
        printf("**********��ջ�Ͷ���ʵ��ͣ��������ϵͳ**********\n");
        printf("************************************************\n");
        printf("��ǰͣ�����й���%d��������ʣ%d��ͣ��λ��������ʾ��\n", parkingLot.top+1, MAX_SIZE - parkingLot.top - 1);
       //printf("��λ1    %s\n", parkingLot.cars[0].licensePlate);
        for (int i = 1-1; i < MAX_SIZE; i++) {
            if (i <= parkingLot.top) {
                printf("��λ%d    %s\n", i + 1, parkingLot.cars[i].licensePlate);
            } else {
                printf("��λ%d    __________\n", i + 1);
            }
        }
        printf("\n");
        printf("�������ͣ�   1----����     2----ȡ��     3----����     0----�˳�\n");
        printf("��ѡ��Ҫִ�еĲ�����");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("�����복�ƺţ�");
                scanf("%s", licensePlate);

                time(&currentTime);

                Car car;
                strcpy(car.licensePlate, licensePlate);
                car.arrivalTime = currentTime;
                car.departureTime = -1;

                if (!isStackFull(&parkingLot)) {
                    pushToStack(&parkingLot, car);
                } else {
                    enqueue(&driveway, car);
                }
                break;

            case 2:
                printf("�����복�ƺţ�");
                scanf("%s", licensePlate);

                time(&currentTime);

                Car removedCar;

                while (!isStackEmpty(&parkingLot)) {
                    Car car = popFromStack(&parkingLot);
                    if (strcmp(car.licensePlate, licensePlate) == 0) {
                        car.departureTime = currentTime;
                        removedCar = car;
                        break;
                    } else {
                        enqueue(&driveway, car);
                    }
                }

                if (strcmp(removedCar.licensePlate, licensePlate) == 0) {
                    printParkingTimeAndFee(removedCar);
                } else {
                    printf("ͣ�����в����ڳ��ƺ�Ϊ%s�ĳ�����\n", licensePlate);
                }

                while (!isQueueEmpty(&driveway)) {
                    Car car = dequeue(&driveway);
                    pushToStack(&parkingLot, car);
                }
                break;

            case 3:
                clearScreen();
                break;

            case 0:
                break;

            default:
                printf("��Ч�Ĳ������ͣ�������ѡ��\n");
                break;
        }

        printf("\n");
    } while (option != 0);

    return 0;
}
