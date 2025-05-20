#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

const int ALPHABET = 5;
const int MAXNODES = 300000;

int to_index(char c) {
    if (c == 'A') return 0;
    if (c == 'C') return 1;
    if (c == 'G') return 2;
    if (c == 'T') return 3;
    return 4; // 'N'
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
int trie_size = 1;

struct SubPattern {
    string s;
    int offset;
};

void add_pattern(const string& pattern, int id) {
    int node = 0;
    for (int i = 0; i < pattern.size(); i++) {
        int c = to_index(pattern[i]);
        if (trie[node].next[c] == -1) {
            trie[trie_size] = Node();
            trie[trie_size].parent = node;
            trie[trie_size].parent_char = pattern[i];
            trie[node].next[c] = trie_size;

            cout << "Добавлена вершина " << trie_size << ": " << pattern[i] << " из " << node << endl;

            trie_size++;
        }
        node = trie[node].next[c];
    }
    trie[node].output.push_back(id);
    cout << "Паттерн \"" << pattern << "\" добавлен в вершину " << node << " (ID: " << id << ")\n";
}

void build_links() {
    cout << "\n=== Построение суффиксных и терминальных ссылок ===\n";

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
        int v = q.front();
        q.pop();
        for (int c = 0; c < ALPHABET; c++) {
            int u = trie[v].next[c];
            if (u != -1) {
                int f = trie[v].link;
                while (trie[f].next[c] == -1) {
                    f = trie[f].link;
                }
                trie[u].link = trie[f].next[c];
                trie[u].terminal_link = (!trie[trie[u].link].output.empty())
                    ? trie[u].link
                    : trie[trie[u].link].terminal_link;

                cout << "Вершина " << u << ": суффиксная ссылка -> " << trie[u].link
                    << ", терминальная ссылка -> " << trie[u].terminal_link << endl;

                q.push(u);
            }
        }
    }

    cout << "\n=== Характеристики автомата (всех вершин) ===\n";
    for (int i = 0; i < trie_size; i++) {
        cout << "Вершина " << i << ": ";
        if (i != 0) {
            cout << "родитель = " << trie[i].parent << " ('" << trie[i].parent_char << "'), ";
        }
        cout << "link = " << trie[i].link << ", terminal_link = " << trie[i].terminal_link << ", ";
        cout << "переходы: ";
        for (int j = 0; j < ALPHABET; j++) {
            if (trie[i].next[j] != -1) {
                cout << from_index(j) << "->" << trie[i].next[j] << " ";
            }
        }
        if (!trie[i].output.empty()) {
            cout << ", выходы: ";
            for (int id : trie[i].output) cout << id << " ";
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    string T, P;
    char wildcard;
    cin >> T >> P >> wildcard;

    vector<SubPattern> subpatterns;
    string current = "";
    int offset = 0;

    for (int i = 0; i < P.size(); i++) {
        if (P[i] == wildcard) {
            if (!current.empty()) {
                subpatterns.push_back({ current, offset });
                current = "";
            }
            offset = i + 1;
        }
        else {
            if (current.empty()) offset = i;
            current += P[i];
        }
    }
    if (!current.empty()) {
        subpatterns.push_back({ current, offset });
    }

    cout << "\n=== Разбиение шаблона на подстроки ===\n";
    for (int i = 0; i < subpatterns.size(); i++) {
        cout << i << ": \"" << subpatterns[i].s << "\" (offset = " << subpatterns[i].offset << ")\n";
        add_pattern(subpatterns[i].s, i);
    }

    build_links();

    int m = P.size();
    vector<int> match(T.size(), 0);

    cout << "\n=== Обработка текста ===\n";
    int node = 0;
    for (int i = 0; i < T.size(); i++) {
        int c = to_index(T[i]);
        while (trie[node].next[c] == -1)
            node = trie[node].link;
        node = trie[node].next[c];

        cout << "Символ '" << T[i] << "' (позиция " << i + 1 << "), перешли в вершину " << node << endl;

        int temp = node;
        while (temp != -1 && temp != 0) {
            for (int j = 0; j < trie[temp].output.size(); j++) {
                int pat_id = trie[temp].output[j];
                int pos = i - subpatterns[pat_id].s.size() + 1 - subpatterns[pat_id].offset;
                if (0 <= pos && pos + m <= T.size()) {
                    match[pos]++;
                    cout << "  Найден шаблон " << pat_id << " на позиции " << pos << endl;
                }
            }
            temp = trie[temp].terminal_link;
        }
    }

    cout << "\n=== Все совпадения шаблона (позиции начала шаблона) ===\n";
    vector<int> found_positions;
    for (int i = 0; i < T.size(); i++) {
        if (match[i] == subpatterns.size()) {
            cout << (i + 1) << "\n";
            found_positions.push_back(i);
        }
    }

    int max_out = 0;
    for (int i = 0; i < trie_size; i++) {
        int cnt = 0;
        for (int j = 0; j < ALPHABET; j++)
            if (trie[i].next[j] != -1) cnt++;
        max_out = max(max_out, cnt);
    }
    cout << "\nМакс. количество исходящих рёбер из вершины: " << max_out << endl;

    cout << "\n=== Строка без вхождений шаблона ===\n";
    vector<bool> masked(T.size(), false);
    for (int start : found_positions) {
        for (int i = start; i < start + m; i++) {
            if (i < T.size()) masked[i] = true;
        }
    }

    for (int i = 0; i < T.size(); i++) {
        if (!masked[i]) cout << T[i];
    }
    cout << endl;

    return 0;
}
