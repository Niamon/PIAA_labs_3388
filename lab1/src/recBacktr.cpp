#include <iostream>
using namespace std;

const int MAX_N = 20;

struct Square {
    int x = 0, y = 0, w = 0;
};
int countWays = 0;
bool bestPathSet = false;

void dfs(int n, int m, int heights[], int cnt, int& bestCnt,
    Square currentPath[], Square bestPath[], int& currentLen) {

    if (cnt > bestCnt) return;

// Поиск самой низкой колонки
    int left = 0, minHeight = m;
    for (int i = 0; i < n; i++) {
        if (heights[i] < minHeight) {
            minHeight = heights[i];
            left = i;
        }
    }

// Если всё покрыто
    if (minHeight == m) {
        if (cnt < bestCnt) {
            bestCnt = cnt;
            countWays = 1;
            for (int i = 0; i < cnt; ++i) {
                bestPath[i] = currentPath[i];
            }
            bestPathSet = true;
        }
        else if (cnt == bestCnt) {
            countWays++;
            if (!bestPathSet) {
                for (int i = 0; i < cnt; ++i) {
                    bestPath[i] = currentPath[i];
                }
                bestPathSet = true;
            }
        }
        return;
    }
// Поиск свободной ширины справа
    int right = left;
    while (right < n && heights[right] == minHeight && (right - left + minHeight) < m) {
        ++right;
    }

    int maxSize = right - left;
    if (m - minHeight < maxSize) maxSize = m - minHeight;

    for (int size = maxSize; size >= 1; size--) {
        if (cnt + 1 > bestCnt) continue;
        if (size >= n || size >= m) continue;

        
       /* cout << string(cnt * 2, ' ') << "[DFS level " << cnt << "] Пытаемся поставить квадрат "
            << size << "x" << size << " в точку (" << left + 1 << ", " << minHeight + 1 << ")" << endl;*/

        for (int i = left; i < left + size; ++i)
            heights[i] += size;

       
    /*    cout << string(cnt * 2, ' ') << "Высоты после установки: [";
        for (int i = 0; i < n; ++i) cout << heights[i] << (i < n - 1 ? ", " : "");
        cout << "]" << endl;*/

        currentPath[currentLen++] = { left + 1, minHeight + 1, size };
        dfs(n, m, heights, cnt + 1, bestCnt, currentPath, bestPath, currentLen);
        currentLen--;

        for (int i = left; i < left + size; ++i)
            heights[i] -= size;

       
     /*   cout << string(cnt * 2, ' ') << "Откат после квадрата " << size << "x" << size
            << " в точке (" << left + 1 << ", " << minHeight + 1 << ")" << endl;*/
    }
}


int tilingRectangle(int n, int m) {
    if (n > m) swap(n, m);

    int heights[MAX_N] = { 0 };
    Square currentPath[MAX_N * MAX_N];
    Square bestPath[MAX_N * MAX_N];
    int currentLen = 0, bestLen = n * m;

    countWays = 0;
    bestPathSet = false;

    dfs(n, m, heights, 0, bestLen, currentPath, bestPath, currentLen);

    cout << "Минимальное количество квадратов: " << bestLen << endl;
    for (int i = 0; i < bestLen; ++i) {
        cout << bestPath[i].x << " " << bestPath[i].y << " " << bestPath[i].w << endl;
    }
    cout << "Количество способов заполнения прямоугольника минимльным количеством квадратов: " << countWays << endl;

    return bestLen;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int n, m;
    cin >> n >> m;
    tilingRectangle(n, m);
    return 0;
}
