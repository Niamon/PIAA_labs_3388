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
    vector<int> output;
    int parent;
    char parent_char;
    Node() {
        memset(next, -1, sizeof(next));
        link = -1;
        parent = -1;
        parent_char = 0;
    }
};

Node trie[MAXNODES];
int trie_size = 1;

void add_pattern(const string& pattern, int id) {
    cout << "\nДобавляется паттерн \"" << pattern << "\" с id = " << id << endl;
    int node = 0;
    for (int i = 0; i < pattern.size(); i++) {
        int c = to_index(pattern[i]);
        if (trie[node].next[c] == -1) {
            trie[trie_size] = Node();
            trie[trie_size].parent = node;
            trie[trie_size].parent_char = pattern[i];
            trie[node].next[c] = trie_size;
            cout << "  Создана вершина " << trie_size << " из " << node
                << " по символу '" << pattern[i] << "'\n";
            trie_size++;
        }
        node = trie[node].next[c];
    }
    trie[node].output.push_back(id);
}

void build_links() {
    cout << "\n=== Построение суффиксных ссылок ===\n";
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
                while (trie[f].next[c] == -1) {
                    f = trie[f].link;
                }
                trie[u].link = trie[f].next[c];
                cout << "Вершина " << u << " (символ '" << trie[u].parent_char << "') получает ссылку на "
                    << trie[u].link << endl;

                for (int j = 0; j < trie[trie[u].link].output.size(); j++) {
                    int out = trie[trie[u].link].output[j];
                    trie[u].output.push_back(out);
                    cout << "  Наследует выход: " << out << endl;
                }

                q.push(u);
            }
        }
    }

    cout << "\n=== Характеристики вершин ===\n";
    for (int i = 0; i < trie_size; i++) {
        cout << "Вершина " << i << ": ";
        if (i != 0) {
            cout << "родитель = " << trie[i].parent << " ('" << trie[i].parent_char << "'), ";
        }
        cout << "link = " << trie[i].link << ", переходы: ";
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
}

int main() {
    setlocale(LC_ALL, "russian");
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

    cout << "\n=== Обработка текста ===\n";
    vector<pair<int, int>> result;
    int node = 0;
    for (int i = 0; i < T.size(); i++) {
        int c = to_index(T[i]);
        while (trie[node].next[c] == -1) {
            node = trie[node].link;
        }
        node = trie[node].next[c];

        cout << "Символ '" << T[i] << "' на позиции " << i + 1 << ", перешли в вершину " << node << endl;

        for (int j = 0; j < trie[node].output.size(); j++) {
            int pattern_id = trie[node].output[j];
            int pos = i - pattern_lengths[pattern_id - 1] + 2;
            cout << "  Найден паттерн " << pattern_id << " на позиции " << pos << endl;
            result.push_back({ pos, pattern_id });
        }
    }

    sort(result.begin(), result.end());

    cout << "\n=== Результаты совпадений (позиция, ID шаблона) ===\n";
    for (const auto& p : result) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}
