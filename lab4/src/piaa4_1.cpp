#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> buildPrefixFunction(const string& pattern) {
    int m = pattern.length();
    vector<int> prefix(m, 0);
    int k = 0;

    cout << "=== Построение префикс-функции ===\n";
    for (int i = 1; i < m; ++i) {
        cout << "Обработка символа pattern[" << i << "] = '" << pattern[i] << "'\n";
        while (k > 0 && pattern[k] != pattern[i]) {
            cout << "Несовпадение: pattern[" << k << "] = '" << pattern[k]
                << "' != pattern[" << i << "] = '" << pattern[i] << "' → откат k = prefix[" << k - 1 << "] = " << prefix[k - 1] << "\n";
            k = prefix[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            ++k;
            cout << "Совпадение: увеличиваем k → " << k << "\n";
        }
        prefix[i] = k;
        cout << "prefix[" << i << "] = " << k << "\n\n";
    }
    return prefix;
}

vector<int> kmpSearch(const string& pattern, const string& text) {
    vector<int> result;
    int m = pattern.length();
    int n = text.length();
    vector<int> prefix = buildPrefixFunction(pattern);
    int q = 0;

    cout << "\n=== Поиск вхождений шаблона в текст ===\n";
    for (int i = 0; i < n; ++i) {
        cout << "Сравнение text[" << i << "] = '" << text[i] << "' с pattern[" << q << "]\n";
        while (q > 0 && pattern[q] != text[i]) {
            cout << "  Несовпадение -> q = prefix[" << q - 1 << "] = " << prefix[q - 1] << "\n";
            q = prefix[q - 1];
        }
        if (pattern[q] == text[i]) {
            ++q;
            cout << "  Совпадение -> q увеличен до " << q << "\n";
        }
        if (q == m) {
            int pos = i - m + 1;
            cout << "  Полное совпадение найдено на позиции " << pos << "\n";
            result.push_back(pos);
            q = prefix[q - 1];
            cout << "  Переход к следующему поиску, q = " << q << "\n";
        }
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string pattern, text;
    cout << "Введите шаблон:\n";
    getline(cin, pattern);
    cout << "Введите текст:\n";
    getline(cin, text);

    vector<int> occurrences = kmpSearch(pattern, text);

    cout << "\n=== Результат ===\n";
    if (occurrences.empty()) {
        cout << "Вхождений не найдено. Вывод: -1\n";
        cout << -1 << endl;
    }
    else {
        cout << "Шаблон найден на позициях: ";
        for (size_t i = 0; i < occurrences.size(); ++i) {
            if (i > 0) cout << ",";
            cout << occurrences[i];
        }
        cout << endl;
    }

    return 0;
}
