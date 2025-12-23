#pragma once
#include <string>
#include <stack>
#include <stdexcept>
#include "stack.h"
#include <iostream>

using namespace std;
enum TokenType {
	TOKEN_NUMBER,             // Число, и вещественное
	TOKEN_OPERATOR,          // Оператор: +-*/
	TOKEN_LEFT_PAREN,        // Левая скобка (
	TOKEN_RIGHT_PAREN,        //Правая скобка )
	TOKEN_UNARY_MINUS,        // Унарный минус
	TOKEN_END,           // конец 
	TOKEN_SPACE           // проьел
};
enum SolutionState {
	STATE_START,             // Начало 
	STATE_NEED_OPERAND,       // Нужно число, переменная или скобка
	STATE_NEED_OPERATOR,      // Нужен оператор или закрытая скобка )
	STATE_CORRECT_END         // Все хорошо, конец
};


// автомат для действия с parse
enum ParseAction {
	// Для чисел:
	PARSE_START_NUMBER,     // первая цифра числа
	PARSE_CONTINUE_NUMBER,  // продолжить число (цифры/точка)
	PARSE_END_NUMBER,       // Завершить число и добавить в лексемы

	// Для операторов и скобок:
	PARSE_ADD_OPERATOR,     // Добавить оператор (+ - * /) в лексемы
	PARSE_ADD_PAREN_OPEN,   // Добавить '(' в lexems
	PARSE_ADD_PAREN_CLOSE,  // Добавить ')' в lexems
	PARSE_ADD_UNARY_MINUS,  // Добавить унарный минус '~' в лексемы

	PARSE_SKIP_SPACE,       // Пропустить пробел
	PARSE_ERROR             // Ошибка, Но такого не должно быть, так как у нас есть проверка строки другим автоматом
};



class SyntaxAutomat {
private:
	SolutionState logicTable[4][6];   // 4 состояние 6 токенов
	ParseAction parseActionTable[4][6];  // что делать в parse
	void initLogicTable() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 6; j++) {
				logicTable[i][j] = STATE_CORRECT_END; //error
			}
		}
		// STATE_START начало
		logicTable[STATE_START][TOKEN_NUMBER] = STATE_NEED_OPERATOR; // Число
		logicTable[STATE_START][TOKEN_LEFT_PAREN] = STATE_NEED_OPERAND;
		logicTable[STATE_START][TOKEN_UNARY_MINUS] = STATE_NEED_OPERAND;

		// STATE_NEED_OPERAND после (
		logicTable[STATE_NEED_OPERAND][TOKEN_NUMBER] = STATE_NEED_OPERATOR;
		logicTable[STATE_NEED_OPERAND][TOKEN_LEFT_PAREN] = STATE_NEED_OPERAND;
		logicTable[STATE_NEED_OPERAND][TOKEN_UNARY_MINUS] = STATE_NEED_OPERAND;

		//STATE_NEED_OPERAATOR после числа или переменной
		logicTable[STATE_NEED_OPERATOR][TOKEN_OPERATOR] = STATE_NEED_OPERAND;
		logicTable[STATE_NEED_OPERATOR][TOKEN_RIGHT_PAREN] = STATE_NEED_OPERATOR;
		logicTable[STATE_NEED_OPERATOR][TOKEN_END] = STATE_CORRECT_END;
		logicTable[STATE_NEED_OPERATOR][TOKEN_NUMBER] = STATE_NEED_OPERATOR;
	}
	void initParseActionTable() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 6; j++) {
				parseActionTable[i][j] = PARSE_ERROR;
			}
		}

		// В начале выражения:
		parseActionTable[STATE_START][TOKEN_NUMBER] = PARSE_START_NUMBER;       // Цифра - начинаем число
		parseActionTable[STATE_START][TOKEN_LEFT_PAREN] = PARSE_ADD_PAREN_OPEN; // ( - добавляем
		parseActionTable[STATE_START][TOKEN_UNARY_MINUS] = PARSE_ADD_UNARY_MINUS; // Унарный минус - ~

		// Нужен операнд (после оператора или '('):
		parseActionTable[STATE_NEED_OPERAND][TOKEN_NUMBER] = PARSE_START_NUMBER;       // Число - начинаем
		parseActionTable[STATE_NEED_OPERAND][TOKEN_LEFT_PAREN] = PARSE_ADD_PAREN_OPEN; // ( - добавляем
		parseActionTable[STATE_NEED_OPERAND][TOKEN_UNARY_MINUS] = PARSE_ADD_UNARY_MINUS; // Унарный минус

		// Ждем оператор (после числа или ')'):
		parseActionTable[STATE_NEED_OPERATOR][TOKEN_OPERATOR] = PARSE_ADD_OPERATOR;    // оператор - добавляем
		parseActionTable[STATE_NEED_OPERATOR][TOKEN_RIGHT_PAREN] = PARSE_ADD_PAREN_CLOSE; // ) - добавляем
		parseActionTable[STATE_NEED_OPERATOR][TOKEN_NUMBER] = PARSE_CONTINUE_NUMBER;


		// Пробелы везде пропускаем (TOKEN_END - пробел)
		parseActionTable[STATE_START][TOKEN_END] = PARSE_SKIP_SPACE;
		parseActionTable[STATE_NEED_OPERAND][TOKEN_END] = PARSE_SKIP_SPACE;
		parseActionTable[STATE_NEED_OPERATOR][TOKEN_END] = PARSE_SKIP_SPACE;
	}

	//Проверка скобок
	bool CheckCorrectParen(const string& s) {
		TStack<char> paren(s.length());
		for (char c : s) {
			if (c == '(') {
				paren.Push(c);
			}
			else if (c == ')') {
				if (paren.isEmpty())
					return false;
				paren.Pop();
			}
		}
		return paren.isEmpty();
	}
	
public:
	SyntaxAutomat() {
		initLogicTable(); 
		initParseActionTable();
	}
	TokenType getTokenType(char c, int pos, const string& s) {
		switch (c) {
		case '0': case '1': case '2': case '3':case '4': case '5':case '6': case '7':case '8': case '9':case '.':
			return TOKEN_NUMBER;
		case '(':
			return TOKEN_LEFT_PAREN;
		case ')':
			return TOKEN_RIGHT_PAREN;
		case '+': case '*': case '/':
			return TOKEN_OPERATOR;
		case '-':
			// Вначале - унарный
			if (pos == 0) return TOKEN_UNARY_MINUS;
			// После ( - унарный
			if (s[pos - 1] == '(') return TOKEN_UNARY_MINUS;
			// После другого оператора - унарный
			if (pos > 0) {
				char leftC = s[pos - 1];
				// - после оператора - унарный
				if (leftC == '+' || leftC == '-' || leftC == '*' || leftC == '/')
					return TOKEN_UNARY_MINUS;
				// Пробел перед -
				if (leftC == ' ') {
					// Идем до не пробела
					for (int i = pos - 2; i >= 0; i--) {
						if (s[i] != ' ') {
							char okC = s[i];
							if (okC == '(' || okC == '+' || okC == '-' || okC == '*' || okC == '/') {
								return TOKEN_UNARY_MINUS;
							}
							else return TOKEN_OPERATOR;
						}
					}
					return TOKEN_UNARY_MINUS;
				}
				if (leftC == 'C')
					return TOKEN_UNARY_MINUS;
				return TOKEN_OPERATOR;
			}
		}
		if (c == ' ') return TOKEN_SPACE;

		return TOKEN_END;
	}

	bool checkCorrectStr(const string& s) {
		// Скобки
		if (!CheckCorrectParen(s)) return false;
		// Пустота
		if (s.empty()) return false;
		// лишнии символы
		for (size_t i = 0; i < s.length(); i++) {
			char c = s[i];
			if (!isdigit(c) && c != '.' && c != '+' && c != '-' &&
				c != '*' && c != '/' && c != '(' && c != ')' && c != ' ') {
				throw invalid_argument(
					string("Invalid character: '") + c +
					"' at pos " + to_string(i)
				);
			}
		}
		// Остальная проверка автоматов
		SolutionState curState = STATE_START;
		for (int i = 0; i < s.length(); i++) {
			char c = s[i];
			if (c == ' ') continue;
			TokenType type = getTokenType(c, i, s);
			SolutionState nextState = logicTable[curState][type];
			if (nextState == STATE_CORRECT_END && type != TOKEN_END) return false;
			curState = nextState;
			// Скипаем цифры или точки, если у нас цифра
			if (type == TOKEN_NUMBER) {
				while (i + 1 < s.length() && (isdigit(s[i + 1]) || s[i + 1] == '.'))
					i++;
			}
		}
		if (curState == STATE_NEED_OPERAND) {
			return false;
		}
		curState = logicTable[curState][TOKEN_END];
		return curState == STATE_CORRECT_END;
	}
	SolutionState getNextState(SolutionState cur, TokenType type) {
		return logicTable[cur][type];
	}
	

	// Operator
	enum OperatorAction {
		ACTION_PUSH,           // Просто поместить в стек
		ACTION_POP_THEN_PUSH,  // Вытолкнуть старый, потом поместить новый
		ACTION_ERROR           // Хз что делать
	};

	int getOperatorPriority(char op) {
		switch (op) {
		case '+': case '-': return 1;
		case '*': case '/': return 2;
		case '~': return 3; // унарный минус
		default: return 0;
		}
	}

	bool canPopOperator(char curOp, char stackOp) {
		if (curOp == '~') return false;
		if (stackOp == '(' || stackOp == '\0') return false;

		int curPriority = getOperatorPriority(curOp);
		int stackPriority = getOperatorPriority(stackOp);

		return curPriority < stackPriority;
	}


	OperatorAction getOperatorAction(char curOp, char stackTop) {
		if (stackTop == '\0' || stackTop == '(') {
			return ACTION_PUSH;
		}

		if (canPopOperator(curOp, stackTop)) {
			return ACTION_POP_THEN_PUSH;
		}

		return ACTION_PUSH;
	}


	// Parse
	ParseAction getParseAction(SolutionState state, TokenType type) {
		return parseActionTable[state][type];
	}

	//ПРодолжение числа
	bool isNumberContinuing(char curChar, char nextChar) {
		if (isdigit(curChar)) {
			return isdigit(nextChar) || nextChar == '.';
		}
		if (curChar == '.') {
			return isdigit(nextChar);
		}
		return false;
	}
};