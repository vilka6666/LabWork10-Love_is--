// ����������� ����������� ���������
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

// ������� ��� ������ � ������ � ������� ����������
void BFSD(const vector < vector < int>>& G, int start_vertex, int target_vertex, vector<int>& DIST) {
    queue<int> Q;
    Q.push(start_vertex);
    DIST[start_vertex] = 0;

    while (!Q.empty()) {
        int current = Q.front();
        Q.pop();

        if (current == target_vertex) // ���� �������� ������� �������, ��������� �����
            break;

        for (int i = 1; i < G[current].size(); ++i) { // ���������� ������� �� <, ����� �� �������� �� ������� �������
            if (G[current][i] > 0 && DIST[i] == -1) {
                Q.push(i);
                DIST[i] = DIST[current] + G[current][i];
            }
        }
    }
}

// ������������� ������� ��� ������ � ������ ��� �������� ������� �������
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
    // ���������� ��������������� ��� ������ �������
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

    // ���������� ������� � ����������� ����������������
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

// ������� ��� ��������� � ������ ������� ���������
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

    // ����� ������� ��������� �� �����
    printf("������� ���������:\n");
    for (int i = 1; i <= num_vertices; ++i) {
        for (int j = 1; j <= num_vertices; ++j) {
            printf("%d ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    return adjacency_matrix;
}

int main(int argc, char* argv[]) {
    // ��������� ������
    setlocale(LC_ALL, "RUS");

    // ������������� ������ ��� ����������� � ���������������� �����
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
    
    // �������� ������� ���������� ��������� ������
    if (argc != 3) {
        printf("�� �� ������������ ��������� ��������� ������.\n");
        printf("���������� ����? (1 = ��, 0 = ���): ");
        scanf("%d", &isWeighted);
        printf("��������������� ����? (1 = ��, 0 = ���): ");
        scanf("%d", &isDirected);
    }
    else {
        isWeighted = atoi(argv[1]);
        isDirected = atoi(argv[2]);
    }

    // �������� ��������� ��������
    if ((isWeighted != 0 && isWeighted != 1) || (isDirected != 0 && isDirected != 1)) {
        printf("�������� ���������. ����������, �������� �����������.\n");
        return 1;
    }

    // ���� ���������� ������ � ����������� �����
    int num_vertices;
    double edge_probability;

    do {
        printf("������� ���������� ������ � �����: ");
        scanf("%d", &num_vertices);
    } while (num_vertices <= 0);

    do {
        printf("������� ����������� ������� ����� � ����� (0 - 1): ");
        scanf("%lf", &edge_probability);
    } while (edge_probability < 0.0 || edge_probability > 1.0);

    // ������������� ���������� ��������� �����
    srand(static_cast<unsigned int>(time(nullptr)));

    // ��������� � ����� ������� ���������
    auto matrix = generateAndPrintMatrix(num_vertices, edge_probability, isDirected, isWeighted);

    // ���� ��������� � �������� ������ ��� ������ ����
    int start_vertex = 1;
    int target_vertex;

    do {
        printf("������� �������� �������: ");
        scanf("%d", &target_vertex);
    } while (target_vertex <= 0 || target_vertex > num_vertices);

    // ������������� ������� ����������
    vector<int> DIST(num_vertices + 1, -1);

    // ����� ������� ������ ����������� ����
    BFSD(matrix, start_vertex, target_vertex, DIST);

    if (DIST[target_vertex] == -1) {
        printf("��������������: ���� �� �������� ������� �� ����������.\n");
    }
    else {
        printf("���������� �� �������� �������: %d\n", DIST[target_vertex]);
    }

    // ���������� ������� � �������� �����
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

    // ����� ������� � �������� �����, ����������� � ������������ ������
    printf("������: %d\n", radius);
    printf("�������: %d\n", diameter);

    printf("����������� �������: ");
    for (int i = 1; i <= num_vertices; ++i) {
        if (eccentricity[i] == radius) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("������������ �������: ");
    for (int i = 1; i <= num_vertices; ++i) {
        if (eccentricity[i] == diameter) {
            printf("%d ", i);
        }
    }
    printf("\n");

    int centroid = findGraphCentroid(matrix, num_vertices);
    printf("����� ������� �����: %d\n", centroid);

    return 0;
}