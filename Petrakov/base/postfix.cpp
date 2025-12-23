#include "postfix.h"
#include "stack.h"
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <cctype>
#include "Automat.h"
using namespace std;

TPostfix::TPostfix(string infx) : infix(infx) {
	priority = {
		{'+', 1}, {'-', 1},
		{'*', 2}, {'/', 2},
		{'~', 3}
	};
    if (infix.empty())
        throw invalid_argument("Empty expression");
    if (!automat.checkCorrectStr(infix))
        throw invalid_argument("Invalid expression syntax");
	ToPostfix();
}
TPostfix::TPostfix() : infix(""), postfix("") {
    priority = {
        {'+', 1}, {'-', 1},
        {'*', 2}, {'/', 2},
        {'~', 3}
    };
}
void TPostfix::ToPostfix() {
    Parse();

    TStack<string> stOp(lexems.size());

    for (const string& l : lexems) {
        char firstChar = l[0];

        // Число - в строку
        if (isdigit(firstChar) || (firstChar == '-' && l.length() > 1)) {
            postfix += l + " ";
        }
        // ( - в стек
        else if (firstChar == '(') {
            stOp.Push(l);
        }
        // ) - выталкиваем до (
        else if (firstChar == ')') {
            while (!stOp.isEmpty() && stOp.Top() != "(") {
                postfix += stOp.Pop() + " ";
            }
            if (!stOp.isEmpty()) stOp.Pop(); // Удаляем '(' из стека
        }
        // Оператор и ~
        else if (firstChar == '+' || firstChar == '-' ||
            firstChar == '*' || firstChar == '/' ||
            firstChar == '~') {

            // Оператор с вершины стека
            char stackTop = stOp.isEmpty() ? '\0' : stOp.Top()[0];

            // Автомат, куда нам нужен этот оператор
            SyntaxAutomat::OperatorAction action = automat.getOperatorAction(firstChar, stackTop);

            // Дейтсвие оператора
            if (action == SyntaxAutomat::ACTION_POP_THEN_PUSH) {
                while (!stOp.isEmpty() && stOp.Top() != "(") {
                    char topOp = stOp.Top()[0];
                    if (automat.canPopOperator(firstChar, topOp)) {
                        postfix += stOp.Pop() + " ";
                    }
                    else {
                        break;
                    }
                }
            }
            
            // Если автомат пуш, то добавляем
            stOp.Push(l);
        }
    }

    // Вытаскиваем операторы, которые остались в постфикс
    while (!stOp.isEmpty()) {
        postfix += stOp.Pop() + " ";
    }

    // Убираем пробел в конце
    if (!postfix.empty() && postfix.back() == ' ') {
        postfix.pop_back();
    }
}
void TPostfix::Parse() {
    string curNumb;
    SolutionState curState = STATE_START;

    for (size_t i = 0; i < infix.length(); i++) {
        char c = infix[i];
        if (c == ' ') continue;

        TokenType type = automat.getTokenType(c, i, infix);
        ParseAction action = automat.getParseAction(curState, type);
        SolutionState nextState = automat.getNextState(curState, type);

        // норм переход?
        if (nextState == STATE_CORRECT_END && type != TOKEN_END) {
            throw invalid_argument("Syntax error at pos: " + to_string(i));
        }

        switch (action) {
        case PARSE_START_NUMBER:
            curNumb = c;

            // Однозначное?
            if (i == infix.length() - 1 ||
                !automat.isNumberContinuing(c, infix[i + 1])) {
                lexems.push_back(curNumb);
                curNumb.clear(); 
            }
            break;

        case PARSE_CONTINUE_NUMBER:
            curNumb += c;

            // Закончилось?
            if (i == infix.length() - 1 ||
                !automat.isNumberContinuing(c, infix[i + 1])) {
                lexems.push_back(curNumb);
                curNumb.clear();   
            }
            break;

        case PARSE_END_NUMBER:
            if (!curNumb.empty()) {
                lexems.push_back(curNumb); 
                curNumb.clear();   
            }
            break;

        case PARSE_ADD_OPERATOR:
            // Если до этого было число, то дособерем его
            if (!curNumb.empty()) {
                lexems.push_back(curNumb);
                curNumb.clear();
            }
            lexems.push_back(string(1, c));
            break;

        case PARSE_ADD_PAREN_OPEN:
            // Если до этого было число, то дособерем его
            if (!curNumb.empty()) {
                lexems.push_back(curNumb);
                curNumb.clear();
            }
            lexems.push_back("(");
            break;

        case PARSE_ADD_PAREN_CLOSE:
            // Если до этого было число, то дособерем его
            if (!curNumb.empty()) {
                lexems.push_back(curNumb);
                curNumb.clear();
            }
            lexems.push_back(")");
            break;

        case PARSE_ADD_UNARY_MINUS:
            // Если до этого было число, то дособерем его
            if (!curNumb.empty()) {
                lexems.push_back(curNumb);
                curNumb.clear();
            }
            lexems.push_back("~");
            break;

        case PARSE_SKIP_SPACE:
            // Если до этого было число, то дособерем его
            if (!curNumb.empty()) {
                lexems.push_back(curNumb);
                curNumb.clear();
            }
            break;

        case PARSE_ERROR:
            // ⭐ Автомат сказал: "Ошибка!"
            throw invalid_argument("Parse error at pos " + to_string(i));
            break;
        }
        // Обновляем состояние
        curState = nextState;
    }
    // Последнее число, если оно есть
    if (!curNumb.empty()) {
        lexems.push_back(curNumb);
    }
}




double TPostfix::Calculate() {
    TStack<double> st(postfix.length());

    string token;
    size_t pos = 0;

    while (pos < postfix.length()) {
        // Пробелы - скип
        while (pos < postfix.length() && postfix[pos] == ' ') {
            pos++;
        }
        if (pos >= postfix.length()) break;

        // Токен
        size_t start = pos;
        while (pos < postfix.length() && postfix[pos] != ' ') {
            pos++;
        }
        token = postfix.substr(start, pos - start);

        char firstChar = token[0];

        // Число
        if (isdigit(firstChar) || (firstChar == '-' && token.length() > 1)) {
            st.Push(stod(token));
        }
        // Унарный минус
        else if (firstChar == '~') {
            if (st.isEmpty()) throw runtime_error("Error operand calc");
            double operand = st.Pop();
            st.Push(-operand);
        }
        // Операторы
        else {
            if (st.GetSize() < 2) throw runtime_error("Not enough operands for operator: " + token);

            double rightOperand = st.Pop();
            double leftOperand = st.Pop();
            double result = 0.0;

            switch (firstChar) {
            case '+': result = leftOperand + rightOperand; break;
            case '-': result = leftOperand - rightOperand; break;
            case '*': result = leftOperand * rightOperand; break;
            case '/':
                if (rightOperand == 0.0) throw runtime_error("Division by zero");
                result = leftOperand / rightOperand;
                break;
            default: throw runtime_error("Unknown operator: " + token);
            }

            st.Push(result);
        }
    }

    if (st.GetSize() != 1) {
        throw runtime_error("Error in structure str");
    }

    return st.Pop();
}




string TPostfix::GetInfix(){
    return infix;
}

string TPostfix::GetPostfix(){
    return postfix;
}

vector<string> TPostfix::GetOperands(){
    vector<string> op;
    map<string, bool> seen;

    for (const string& lexem : lexems) {
        if (isdigit(lexem[0]) || (lexem[0] == '-' && lexem.length() > 1)) {
            if (!seen[lexem]) {
                op.push_back(lexem);
                seen[lexem] = true;
            }
        }
    }

    return op;
}

vector<string> TPostfix::GetLexems(){
    return lexems;
}

bool TPostfix::Validate() {
    return automat.checkCorrectStr(infix);
}
