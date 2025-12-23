#include <iostream>
#include <string>
#include "postfix.h"
#include <locale.h>
using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");
    string expression;
    // Получаем выражение от пользователя
    cout << "Введите арифметическое выражение: ";
    getline(cin, expression);

    // Используем конструктор с параметром
    TPostfix postfix(expression);

    double res;

    setlocale(LC_ALL, "Russian");
    cout << "Арифметическое выражение: " << postfix.GetInfix() << endl;
    cout << "Постфиксная форма: " << postfix.GetPostfix() << endl;

    try {
        res = postfix.Calculate();
        cout << "Результат: " << res << endl;
    }
    catch (const exception& e) {
        cout << "Ошибка вычисления: " << e.what() << endl;
    }

    return 0;
}
