// Подключение необходимых библиотек
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <climits>
#include <locale.h>

using namespace std;

// Функция для обхода в ширину с записью расстояний
void BFSD(const vector < vector < int>>& G, int start_vertex, int target_vertex, vector<int>& DIST) {
    queue<int> Q;
    Q.push(start_vertex);
    DIST[start_vertex] = 0;

    while (!Q.empty()) {
        int current = Q.front();
        Q.pop();

        if (current == target_vertex) // Если достигли целевой вершины, завершаем поиск
            break;

        for (int i = 1; i < G[current].size(); ++i) { // Исправлено условие на <, чтобы не выходить за пределы вектора
            if (G[current][i] > 0 && DIST[i] == -1) {
                Q.push(i);
                DIST[i] = DIST[current] + G[current][i];
            }
        }
    }
}

// Перегруженная функция для обхода в ширину без указания целевой вершины
void BFSD(const vector < vector < int>>& G, int start_vertex, vector<int>& DIST) {
    queue<int> Q;
    Q.push(start_vertex);
    DIST[start_vertex] = 0;

    while (!Q.empty()) {
        int current = Q.front();
        Q.pop();

        for (int i = 1; i < G[current].size(); ++i) {
            if (G[current][i] > 0 && DIST[i] == -1) {
                Q.push(i);
                DIST[i] = DIST[current] + G[current][i];
            }
        }
    }
}

int findGraphCentroid(const vector < vector < int>>& G, int num_vertices) {
    // Вычисление эксцентриситета для каждой вершины
    vector<int> eccentricity(num_vertices + 1);
    for (int v = 1; v <= num_vertices; ++v) {
        vector<int> DIST(num_vertices + 1, -1);
        BFSD(G, v, DIST);

        int max_dist = 0;
        for (int dist : DIST) {
            if (dist != -1) {
                max_dist = max(max_dist, dist);
            }
        }
        eccentricity[v] = max_dist;
    }

    // Нахождение вершины с минимальным эксцентриситетом
    int min_eccentricity = INT_MAX;
    int centroid = -1;
    for (int v = 1; v <= num_vertices; ++v) {
        if (eccentricity[v] < min_eccentricity) {
            min_eccentricity = eccentricity[v];
            centroid = v;
        }
    }

    return centroid;
}

// Функция для генерации и печати матрицы смежности
vector < vector < int>> generateAndPrintMatrix(int num_vertices, double edge_probability, bool isDirected, bool isWeighted) {
    vector < vector < int>> adjacency_matrix(num_vertices + 1, vector<int>(num_vertices + 1, 0));
    for (int i = 1; i <= num_vertices; ++i) {
        for (int j = (isDirected ? 1 : i + 1); j <= num_vertices; ++j) {
            if (i != j && static_cast<double>(rand()) / RAND_MAX < edge_probability) {
                if (isWeighted) {
                    adjacency_matrix[i][j] = rand() % 10 + 1;
                }
                else {
                    adjacency_matrix[i][j] = 1;
                }

                if (!isDirected) {
                    adjacency_matrix[j][i] = adjacency_matrix[i][j];
                }
            }
        }
    }

    // Вывод матрицы смежности на экран
    printf("Матрица смежности:\n");
    for (int i = 1; i <= num_vertices; ++i) {
        for (int j = 1; j <= num_vertices; ++j) {
            printf("%d ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    return adjacency_matrix;
}

int main(int argc, char* argv[]) {
    // Установка локали
    setlocale(LC_ALL, "RUS");

    // Инициализация флагов для взвешенного и ориентированного графа
    int isWeighted = 0;
    int isDirected = 0;

    if (argc > 1)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "-w") == 0)
                isWeighted = 1;

            if (strcmp(argv[i], "-o") == 0)
                isDirected = 1;
        }
    }
    
    // Проверка наличия аргументов командной строки
    if (argc != 3) {
        printf("Вы не предоставили аргументы командной строки.\n");
        printf("Взвешенный граф? (1 = да, 0 = нет): ");
        scanf("%d", &isWeighted);
        printf("Ориентированный граф? (1 = да, 0 = нет): ");
        scanf("%d", &isDirected);
    }
    else {
        isWeighted = atoi(argv[1]);
        isDirected = atoi(argv[2]);
    }

    // Проверка введенных значений
    if ((isWeighted != 0 && isWeighted != 1) || (isDirected != 0 && isDirected != 1)) {
        printf("Неверные аргументы. Пожалуйста, следуйте инструкциям.\n");
        return 1;
    }

    // Ввод количества вершин и вероятности ребер
    int num_vertices;
    double edge_probability;

    do {
        printf("Введите количество вершин в графе: ");
        scanf("%d", &num_vertices);
    } while (num_vertices <= 0);

    do {
        printf("Введите вероятность наличия ребра в графе (0 - 1): ");
        scanf("%lf", &edge_probability);
    } while (edge_probability < 0.0 || edge_probability > 1.0);

    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(nullptr)));

    // Генерация и вывод матрицы смежности
    auto matrix = generateAndPrintMatrix(num_vertices, edge_probability, isDirected, isWeighted);

    // Ввод начальной и конечной вершин для поиска пути
    int start_vertex = 1;
    int target_vertex;

    do {
        printf("Введите конечную вершину: ");
        scanf("%d", &target_vertex);
    } while (target_vertex <= 0 || target_vertex > num_vertices);

    // Инициализация массива расстояний
    vector<int> DIST(num_vertices + 1, -1);

    // Вызов функции поиска кратчайшего пути
    BFSD(matrix, start_vertex, target_vertex, DIST);

    if (DIST[target_vertex] == -1) {
        printf("Предупреждение: Путь до заданной вершины не существует.\n");
    }
    else {
        printf("Расстояние до заданной вершины: %d\n", DIST[target_vertex]);
    }

    // Вычисление радиуса и диаметра графа
    vector<int> eccentricity(num_vertices + 1, 0);
    int radius = INT_MAX, diameter = INT_MIN;

    for (int v = 1; v <= num_vertices; ++v) {
        vector<int> DIST(num_vertices + 1, -1);
        BFSD(matrix, v, DIST);

        int max_dist = 0;
        for (int dist : DIST) {
            if (dist != -1) {
                max_dist = max(max_dist, dist);
            }
        }

        eccentricity[v] = max_dist;
        radius = min(radius, max_dist);
        diameter = max(diameter, max_dist);
    }

    // Вывод радиуса и диаметра графа, центральных и периферийных вершин
    printf("Радиус: %d\n", radius);
    printf("Диаметр: %d\n", diameter);

    printf("Центральные вершины: ");
    for (int i = 1; i <= num_vertices; ++i) {
        if (eccentricity[i] == radius) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Периферийные вершины: ");
    for (int i = 1; i <= num_vertices; ++i) {
        if (eccentricity[i] == diameter) {
            printf("%d ", i);
        }
    }
    printf("\n");

    int centroid = findGraphCentroid(matrix, num_vertices);
    printf("Центр тяжести графа: %d\n", centroid);

    return 0;
}