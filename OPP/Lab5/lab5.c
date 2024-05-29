#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <mpi.h>

#define ERROR (-1)
#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0

#define PARAM 1000
#define LISTS_NUM 3
#define TOTAL_TASKS 2400

pthread_mutex_t mutex, sendThreadMutex, executeThreadMutex;
pthread_cond_t finishTasks, newTasksAvailable;
pthread_t sendThread, receiveThread, executeThread;
int *list;
int executedLists;
int tasks;
int executedTasks;
int procRank;
int procNum;

int sendThreadGetSignal = FALSE;
int executeThreadGetSignal = FALSE;

void initList(int tasksPerProc) {
    for (int i = 0; i < tasksPerProc; ++i)
        list[i] = i * abs(procRank - (executedLists%procNum)) * PARAM;
}

void *sendRequestRoutine(void *args) {
    MPI_Status status;
    pthread_mutex_lock(&sendThreadMutex);                  // захватываем мьютекс сразу, нужен только для wait

    while (TRUE){
        pthread_cond_wait(&finishTasks, &sendThreadMutex); // ждем пока процесс не завершит все задачи из текущего списка
        sendThreadGetSignal = TRUE;
        if (executedLists > LISTS_NUM){
            pthread_mutex_unlock(&sendThreadMutex);
            pthread_exit(NULL);
        }
        int sendRequestFlag = TRUE;
        int receiveTasks;
        for (int i = 0; i < procNum; ++i){                  // посылаем запрос на дополнительные задания всем процессам, кроме себя
            if (i == procRank)
                continue;
            MPI_Send(&sendRequestFlag, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Recv(&receiveTasks, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            if (receiveTasks == 0)
                continue;
            MPI_Recv(&(list[0]), receiveTasks, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            break;
        }
        pthread_mutex_lock(&mutex);
        tasks = receiveTasks;
        executedTasks = 0;
        while (!executeThreadGetSignal)         //пока задачи не взяты на выполнение
            pthread_cond_signal(&newTasksAvailable);    // отправляем сигнал о получении новых заданий
        executeThreadGetSignal = FALSE;
        pthread_mutex_unlock(&mutex);
    }
}

void *receiveRequestRoutine(void *args){
    MPI_Status status;
    while (TRUE) {
        int receiveRequestFlag = FALSE;
        MPI_Recv(&receiveRequestFlag, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); // ждем оповещения о готовности другого процесса получить доп задания
        if (!receiveRequestFlag)
            pthread_exit(NULL);
        pthread_mutex_lock(&mutex);                       // захватываем мьютекс, предотвращая дальнейшую обработку заданий
        int sendTasks = (tasks - executedTasks) / 2;      // столько заданий пошлем
        int leftTasks = (tasks - executedTasks + 1) / 2;  // столько оставим себе
        MPI_Send(&sendTasks, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
        if (sendTasks != 0) {
            MPI_Send(&(list[executedTasks + leftTasks]), sendTasks, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
            tasks = executedTasks + leftTasks;
        }
        pthread_mutex_unlock(&mutex);                     // освобождаем мьютекс
    }
}

void *executeRoutine(void *args){
    int tasksPerProc = TOTAL_TASKS / procNum;
    list = (int *) malloc (tasksPerProc * sizeof(int));
    executedLists = 0;

    double *iterTimes = (double *) malloc(procNum * sizeof(double));
    double iterTime;
    double disbalanceRate;
    double globalRes = 0;

    for (int listId = 0; listId < LISTS_NUM; ++listId) {      // итерируемся по всем спискам
        double start = MPI_Wtime();

        initList(tasksPerProc);                             // инициализируем список
        executedLists++;
        tasks = tasksPerProc;
        executedTasks = 0;

        int totalExecutedTasks = 0;
        double disbalanceTime = 0;
        double maxIterTime = 0;

        while(TRUE) {
            if (tasks == 0)                                 // если количество необходимых заданий равно 0, значит процесс выполнил свои задания и не смог взять их у других
                break;
            for (int taskId = 0; taskId < tasks; ++taskId){ // итерируемся по всем заданиям
                pthread_mutex_lock(&mutex);                 // захватываем мьютекс
                int duration = list[taskId];                // выбираем задание
                executedTasks++;                            // увеличиваем счетчик выполненных заданий, так как рано или поздно взятое задание точно выполнится
                pthread_mutex_unlock(&mutex);               // освобождаем мьютекс
                for (int i = 0; i < duration; ++i)          // выполняем задание
                    globalRes += sin(i);
            }
            totalExecutedTasks += executedTasks;

            pthread_mutex_lock(&executeThreadMutex);                     // захватываем мьютекс (это для pthread_cond_wait())
            while (!sendThreadGetSignal)                                 // пока не инициирована отправка
                pthread_cond_signal(&finishTasks);                       // сигнализируем о завершении своих заданий
            sendThreadGetSignal = FALSE;
            pthread_cond_wait(&newTasksAvailable, &executeThreadMutex);  // ожидаем новых заданий
            executeThreadGetSignal = TRUE;                               // берём задачу на выполнение
            pthread_mutex_unlock(&executeThreadMutex);                   // освобождаем мьютекс
        }

        MPI_Barrier(MPI_COMM_WORLD); // синхронизация процессов перед выполнением заданий из следующего списка
    }
    pthread_mutex_lock(&mutex);
    int receiveRequestFlag = FALSE;
    executedLists++;
    MPI_Send(&receiveRequestFlag, 1, MPI_INT, procRank, 0, MPI_COMM_WORLD); // завершаем receiveThread
    pthread_cond_signal(&finishTasks);                                                // завершаем sendThread
    pthread_mutex_unlock(&mutex);

    free(iterTimes);
    free(list);
    pthread_exit(NULL);
}

int initThreads() {
    pthread_attr_t attrs;
    if (pthread_attr_init(&attrs) != SUCCESS) {
        perror("Can't initialize thread's attributes");
        return ERROR;
    }

    if (pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE) != SUCCESS) {
        perror("Can't make threads joinable");
        return ERROR;
    }

    if (pthread_create(&sendThread, &attrs, sendRequestRoutine, NULL) != SUCCESS) {
        perror("Can't create sendThread");
        return ERROR;
    }
    if (pthread_create(&receiveThread, &attrs, receiveRequestRoutine, NULL) != SUCCESS) {
        perror("Can't create receiveThread");
        return ERROR;
    }
    if (pthread_create(&executeThread, &attrs, executeRoutine, NULL) != SUCCESS) {
        perror("Can't create executeThread");
        return ERROR;
    }
    pthread_attr_destroy(&attrs);

    if (pthread_join(sendThread, NULL) != SUCCESS) {
        perror("Can't join with sendThread");
        return ERROR;
    }
    if (pthread_join(receiveThread, NULL) != SUCCESS) {
        perror("Can't join with receiveThread");
        return ERROR;
    }
    if (pthread_join(executeThread, NULL) != SUCCESS) {
        perror("Can't join with executeThread");
        return ERROR;
    }

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE) {
        printf("Can't get needed thread support level\n");
        return FAILURE;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&sendThreadMutex, NULL);
    pthread_mutex_init(&executeThreadMutex, NULL);
    pthread_cond_init(&finishTasks, NULL);
    pthread_cond_init(&newTasksAvailable, NULL);

    double start = MPI_Wtime();
    initThreads();
    double end = MPI_Wtime();
    if (procRank == 0)
        printf("SIZE = %d ; TOTAL TIME = %lf\n", procNum, end - start);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&sendThreadMutex);
    pthread_mutex_destroy(&executeThreadMutex);
    pthread_cond_destroy(&finishTasks);
    pthread_cond_destroy(&newTasksAvailable);
    MPI_Finalize();

    return SUCCESS;
}
