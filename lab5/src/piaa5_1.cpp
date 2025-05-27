#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int ALPHABET = 5;
const int MAXNODES = 300000;

int to_index(char c) {
    if (c == 'A') return 0;
    if (c == 'C') return 1;
    if (c == 'G') return 2;
    if (c == 'T') return 3;
    return 4; // N
}

char from_index(int i) {
    return "ACGTN"[i];
}

struct Node {
    int next[ALPHABET];
    int link;
    int terminal_link;
    vector<int> output;
    int parent;
    char parent_char;
    Node() {
        memset(next, -1, sizeof(next));
        link = -1;
        terminal_link = -1;
        parent = -1;
        parent_char = 0;
    }
};

Node trie[MAXNODES];
bool real_edge[MAXNODES][ALPHABET] = {};
int trie_size = 1;

void add_pattern(const string& pattern, int id) {
    cout << "\nДобавление шаблона \"" << pattern << "\" (ID: " << id << ")\n";
    int node = 0;
    for (int i = 0; i < pattern.size(); i++) {
        int c = to_index(pattern[i]);
        if (trie[node].next[c] == -1) {
            trie[trie_size] = Node();
            trie[trie_size].parent = node;
            trie[trie_size].parent_char = pattern[i];
            trie[node].next[c] = trie_size;
            real_edge[node][c] = true;
            cout << "  Создана вершина " << trie_size << " из вершины " << node
                << " по символу '" << pattern[i] << "'\n";
            trie_size++;
        }
        node = trie[node].next[c];
    }
    trie[node].output.push_back(id);
    cout << "  Шаблон \"" << pattern << "\" заканчивается в вершине " << node << "\n";
}

void build_links() {
    cout << "\nПостроение суффиксных ссылок автомата:\n";
    queue<int> q;
    trie[0].link = 0;

    for (int c = 0; c < ALPHABET; c++) {
        if (trie[0].next[c] != -1) {
            trie[trie[0].next[c]].link = 0;
            q.push(trie[0].next[c]);
        }
        else {
            trie[0].next[c] = 0;
        }
    }

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int c = 0; c < ALPHABET; c++) {
            int u = trie[v].next[c];
            if (u != -1) {
                int f = trie[v].link;
                while (trie[f].next[c] == -1)
                    f = trie[f].link;
                trie[u].link = trie[f].next[c];

                // Построение терминальных ссылок
                trie[u].terminal_link = (!trie[trie[u].link].output.empty())
                    ? trie[u].link
                    : trie[trie[u].link].terminal_link;

                cout << "Вершина " << u << " (через '" << trie[u].parent_char
                    << "') получает суффиксную ссылку на вершину " << trie[u].link
                    << ", терминальную ссылку на " << trie[u].terminal_link << "\n";

                for (int j : trie[trie[u].link].output) {
                    trie[u].output.push_back(j);
                    cout << "  Наследован выход шаблона ID: " << j << "\n";
                }

                q.push(u);
            }
        }
    }

    cout << "\nХарактеристики построенного автомата:\n";
    for (int i = 0; i < trie_size; i++) {
        cout << "Вершина " << i << ": ";
        if (i != 0) {
            cout << "родитель = " << trie[i].parent
                << " ('" << trie[i].parent_char << "'), ";
        }
        cout << "link = " << trie[i].link
            << ", terminal_link = " << trie[i].terminal_link
            << ", переходы: ";
        for (int j = 0; j < ALPHABET; j++) {
            if (trie[i].next[j] != -1) {
                cout << from_index(j) << "->" << trie[i].next[j] << " ";
            }
        }
        if (!trie[i].output.empty()) {
            cout << ", выходы: ";
            for (int x : trie[i].output) cout << x << " ";
        }
        cout << endl;
    }

    int max_out_degree = 0;
    for (int i = 0; i < trie_size; i++) {
        int count = 0;
        for (int j = 0; j < ALPHABET; j++) {
            if (real_edge[i][j]) count++;
        }
        max_out_degree = max(max_out_degree, count);
    }
    cout << "\nМаксимальное количество дуг, исходящих из одной вершины: "
        << max_out_degree << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    string T;
    int n;
    cin >> T >> n;

    vector<string> patterns(n);
    vector<int> pattern_lengths(n);

    for (int i = 0; i < n; i++) {
        cin >> patterns[i];
        pattern_lengths[i] = patterns[i].size();
        add_pattern(patterns[i], i + 1);
    }

    build_links();

    cout << "\nОбработка текста: \"" << T << "\"\n";
    vector<pair<int, int>> result;
    int node = 0;
    for (int i = 0; i < T.size(); i++) {
        int c = to_index(T[i]);
        while (trie[node].next[c] == -1) {
            cout << "  Нет перехода по '" << T[i] << "' из вершины " << node
                << " — переходим по суффиксной ссылке на " << trie[node].link << endl;
            node = trie[node].link;
        }
        node = trie[node].next[c];
        cout << "Символ '" << T[i] << "' на позиции " << i + 1
            << ", перешли в вершину " << node << endl;

        // Обработка терминальных ссылок
        int temp = node;
        while (temp != -1 && temp != 0) {
            for (int j : trie[temp].output) {
                int pos = i - pattern_lengths[j - 1] + 2;
                cout << "  Найден шаблон ID " << j << " (\"" << patterns[j - 1]
                    << "\") начиная с позиции " << pos << "\n";
                result.push_back({ pos, j });
            }
            temp = trie[temp].terminal_link;
        }
    }

    sort(result.begin(), result.end());

    cout << "\nРезультаты совпадений (позиция, ID шаблона):\n";
    for (const auto& p : result) {
        cout << p.first << " " << p.second << "\n";
    }

    vector<bool> keep(T.size(), true);
    for (const auto& p : result) {
        int start = p.first - 1;
        int len = pattern_lengths[p.second - 1];
        for (int i = start; i < start + len && i < T.size(); i++) {
            keep[i] = false;
        }
    }

    string remaining;
    for (int i = 0; i < T.size(); i++) {
        if (keep[i]) remaining += T[i];
    }

    cout << "\nСтрока после удаления всех найденных шаблонов:\n" << remaining << endl;

    return 0;
}
