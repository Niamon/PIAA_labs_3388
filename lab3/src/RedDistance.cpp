#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


struct Cell {
    int cost;
    char operation;
};


/*vector<vector<Cell>> build_dp(const string& A, const string& B, int cost_replace, int cost_insert, int cost_delete) {
    int m = A.length();
    int n = B.length();
    vector<vector<Cell>> dp(m + 1, vector<Cell>(n + 1));

    dp[0][0] = { 0, 'M' };
    for (int i = 1; i <= m; ++i)
        dp[i][0] = { dp[i - 1][0].cost + cost_delete, 'D' };
    for (int j = 1; j <= n; ++j)
        dp[0][j] = { dp[0][j - 1].cost + cost_insert, 'I' };

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = { dp[i - 1][j - 1].cost, 'M' };
            }
            else {
                int replace_cost = dp[i - 1][j - 1].cost + cost_replace;
                int insert_cost = dp[i][j - 1].cost + cost_insert;
                int delete_cost = dp[i - 1][j].cost + cost_delete;

                if (replace_cost <= insert_cost && replace_cost <= delete_cost) {
                    dp[i][j] = { replace_cost, 'R' };
                }
                else if (insert_cost <= replace_cost && insert_cost <= delete_cost) {
                    dp[i][j] = { insert_cost, 'I' };
                }
                else {
                    dp[i][j] = { delete_cost, 'D' };
                }
            }
        }
    }

    return dp;
}

void find_all_paths(const vector<vector<Cell>>& dp, const string& A, const string& B,
    int i, int j, string current, vector<string>& all_paths,
    int cost_replace, int cost_insert, int cost_delete) {
    if (i == 0 && j == 0) {
        reverse(current.begin(), current.end());
        all_paths.push_back(current);
        return;
    }

    if (i > 0 && j > 0) {
        if (A[i - 1] == B[j - 1] && dp[i][j].cost == dp[i - 1][j - 1].cost) {
            find_all_paths(dp, A, B, i - 1, j - 1, current + 'M', all_paths, cost_replace, cost_insert, cost_delete);
        }
        if (A[i - 1] != B[j - 1] && dp[i][j].cost == dp[i - 1][j - 1].cost + cost_replace) {
            find_all_paths(dp, A, B, i - 1, j - 1, current + 'R', all_paths, cost_replace, cost_insert, cost_delete);
        }
    }
    if (j > 0 && dp[i][j].cost == dp[i][j - 1].cost + cost_insert) {
        find_all_paths(dp, A, B, i, j - 1, current + 'I', all_paths, cost_replace, cost_insert, cost_delete);
    }
    if (i > 0 && dp[i][j].cost == dp[i - 1][j].cost + cost_delete) {
        find_all_paths(dp, A, B, i - 1, j, current + 'D', all_paths, cost_replace, cost_insert, cost_delete);
    }
}

void apply_operations(const string& A, const string& B, const string& operations) {
    string result = A;
    int i = 0, j = 0; 

   
    cout << result << endl;

    for (char op : operations) {
        if (op == 'M') {
            ++i;
            ++j;
        }
        else if (op == 'R') {
            result[i] = B[j];
            ++i;
            ++j;
            cout << result << endl;
        }
        else if (op == 'I') {
            result.insert(result.begin() + i, B[j]);
            ++i;
            ++j;
            cout << result << endl;
        }
        else if (op == 'D') {
            result.erase(result.begin() + i);
            cout << result << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int cost_replace, cost_insert, cost_delete;
    string A, B;

    cin >> cost_replace >> cost_insert >> cost_delete;
    cin >> A >> B;

    auto dp = build_dp(A, B, cost_replace, cost_insert, cost_delete);

    vector<string> all_paths;
    find_all_paths(dp, A, B, A.length(), B.length(), "", all_paths, cost_replace, cost_insert, cost_delete);

    cout << "Все возможные изменения:" << endl;
    for (const string& path : all_paths)
        cout << path << endl;

    if (!all_paths.empty()) {
        cout << "Пример редакционного предписания:" << endl;
        apply_operations(A, B, all_paths[0]);
    }

    return 0;
}
*/


vector<vector<Cell>> build_dp(const string& A, const string& B, int cost_replace, int cost_insert, int cost_delete) {
    int m = A.length();
    int n = B.length();
    vector<vector<Cell>> dp(m + 1, vector<Cell>(n + 1));

    dp[0][0] = { 0, 'M' };
    for (int i = 1; i <= m; ++i)
        dp[i][0] = { dp[i - 1][0].cost + cost_delete, 'D' };
    for (int j = 1; j <= n; ++j)
        dp[0][j] = { dp[0][j - 1].cost + cost_insert, 'I' };

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = { dp[i - 1][j - 1].cost, 'M' };
            }
            else {
                int replace_cost = dp[i - 1][j - 1].cost + cost_replace;
                int insert_cost = dp[i][j - 1].cost + cost_insert;
                int delete_cost = dp[i - 1][j].cost + cost_delete;

                if (replace_cost <= insert_cost && replace_cost <= delete_cost) {
                    dp[i][j] = { replace_cost, 'R' };
                }
                else if (insert_cost <= replace_cost && insert_cost <= delete_cost) {
                    dp[i][j] = { insert_cost, 'I' };
                }
                else {
                    dp[i][j] = { delete_cost, 'D' };
                }
            }
        }
    }

    return dp;
}





void print_dp_table(const vector<vector<Cell>>& dp, const string& A, const string& B) {
    cout << "\nТаблица стоимости преобразований (dp):\n    ";
    for (char ch : B) cout << "     " << ch;
    cout << "\n  ";
    for (int j = 0; j <= B.length(); ++j) {
        cout << "[" << dp[0][j].cost << "," << dp[0][j].operation << "]";
    }
    cout << endl;

    for (int i = 1; i <= A.length(); ++i) {
        cout << A[i - 1] << " ";
        for (int j = 0; j <= B.length(); ++j) {
            cout << "[" << dp[i][j].cost << "," << dp[i][j].operation << "]";
        }
        cout << endl;
    }
}

void find_all_paths(const vector<vector<Cell>>& dp, const string& A, const string& B,
    int i, int j, string current, vector<string>& all_paths,
    int cost_replace, int cost_insert, int cost_delete) {
    if (i == 0 && j == 0) {
        reverse(current.begin(), current.end());
        all_paths.push_back(current);
        return;
    }

    if (i > 0 && j > 0) {
        if (A[i - 1] == B[j - 1] && dp[i][j].cost == dp[i - 1][j - 1].cost) {
            find_all_paths(dp, A, B, i - 1, j - 1, current + 'M', all_paths, cost_replace, cost_insert, cost_delete);
        }
        if (A[i - 1] != B[j - 1] && dp[i][j].cost == dp[i - 1][j - 1].cost + cost_replace) {
            find_all_paths(dp, A, B, i - 1, j - 1, current + 'R', all_paths, cost_replace, cost_insert, cost_delete);
        }
    }
    if (j > 0 && dp[i][j].cost == dp[i][j - 1].cost + cost_insert) {
        find_all_paths(dp, A, B, i, j - 1, current + 'I', all_paths, cost_replace, cost_insert, cost_delete);
    }
    if (i > 0 && dp[i][j].cost == dp[i - 1][j].cost + cost_delete) {
        find_all_paths(dp, A, B, i - 1, j, current + 'D', all_paths, cost_replace, cost_insert, cost_delete);
    }
}

void apply_operations(const string& A, const string& B, const string& operations) {
    string result = A;
    int i = 0, j = 0;

    cout << "\nПошаговое применение операций:\n";
    cout << "Начальное слово: " << result << endl;

    for (char op : operations) {
        if (op == 'M') {
            cout << "Match (совпадение): " << result[i] << " == " << B[j] << " (позиция " << i << ")\n";
            ++i; ++j;
        }
        else if (op == 'R') {
            cout << "Replace (замена): " << result[i] << " -> " << B[j] << " (позиция " << i << ")\n";
            result[i] = B[j];
            ++i; ++j;
            cout << "Результат: " << result << endl;
        }
        else if (op == 'I') {
            cout << "Insert (вставка): " << B[j] << " на позицию " << i << "\n";
            result.insert(result.begin() + i, B[j]);
            ++i; ++j;
            cout << "Результат: " << result << endl;
        }
        else if (op == 'D') {
            cout << "Delete (удаление): " << result[i] << " с позиции " << i << "\n";
            result.erase(result.begin() + i);
            cout << "Результат: " << result << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int cost_replace, cost_insert, cost_delete;
    string A, B;

    cout << "Введите стоимость замены, вставки и удаления: ";
    cin >> cost_replace >> cost_insert >> cost_delete;
    cout << "Введите исходную строку A: ";
    cin >> A;
    cout << "Введите целевую строку B: ";
    cin >> B;

    auto dp = build_dp(A, B, cost_replace, cost_insert, cost_delete);
    print_dp_table(dp, A, B);

    vector<string> all_paths;
    find_all_paths(dp, A, B, A.length(), B.length(), "", all_paths, cost_replace, cost_insert, cost_delete);

    cout << "\nВсе возможные редакционные пути (операции):" << endl;
    for (const string& path : all_paths)
        cout << path << endl;

    if (!all_paths.empty()) {
        cout << "\nПример применения операций для первой последовательности:\n";
        apply_operations(A, B, all_paths[0]);
    }

    return 0;
}