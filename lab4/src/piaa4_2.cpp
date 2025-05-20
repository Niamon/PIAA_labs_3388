#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> buildPrefixFunction(const string& pattern) {
    int m = pattern.length();
    vector<int> prefix(m, 0);
    int k = 0;

    cout << "=== Построение префикс-функции для шаблона \"" << pattern << "\" ===\n";
    for (int i = 1; i < m; ++i) {
        cout << "Обработка pattern[" << i << "] = '" << pattern[i] << "'\n";
        while (k > 0 && pattern[k] != pattern[i]) {
            cout << "  Несовпадение: pattern[" << k << "] = '" << pattern[k]
                << "' != pattern[" << i << "] = '" << pattern[i]
                << "' → откат k = prefix[" << k - 1 << "] = " << prefix[k - 1] << "\n";
            k = prefix[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            ++k;
            cout << "  Совпадение → увеличиваем k до " << k << "\n";
        }
        prefix[i] = k;
        cout << "  Устанавливаем prefix[" << i << "] = " << k << "\n\n";
    }
    return prefix;
}

int kmpSearchFirstOccurrence(const string& pattern, const string& text) {
    int m = pattern.length();
    int n = text.length();
    vector<int> prefix = buildPrefixFunction(pattern);
    int q = 0;

    cout << "\n=== Поиск первого вхождения шаблона \"" << pattern << "\" в строку \"" << text << "\" ===\n";
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
            int startIndex = i - m + 1;
            cout << "  Полное совпадение найдено на позиции " << startIndex << "\n";
            return startIndex;
        }
    }

    cout << "Совпадение не найдено\n";
    return -1;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string A, B;
    cout << "Введите строку B (исходная строка): ";
    cin >> B;
    cout << "Введите строку A (целевая перестановка): ";
    cin >> A;

    if (A.length() != B.length()) {
        cout << "Строки разной длины -> -1\n";
        cout << -1 << endl;
        return 0;
    }

    string BB = B + B;
    cout << "\nПроверка, содержится ли A в B+B = \"" << BB << "\"\n";
    int index = kmpSearchFirstOccurrence(A, BB);

    cout << "\n=== Результат ===\n";
    if (index >= 0 && index < B.length()) {
        cout << "Строка A встречается в B+B начиная с позиции " << index << "\n";
        cout << index << endl;
    }
    else {
        cout << "Строка A не является циклическим сдвигом строки B -> -1\n";
        cout << -1 << endl;
    }

    return 0;
}
