#include<stdio.h> 
#include "lex.h"
#include<string>
#include<iostream>
#include<fstream>
#include <cctype>
#include<map>
#include<vector>
#include<time.h>
#include<list>
using namespace std;


Lex getNextToken(istream& in, int& linenum){
	// Lex lex; //for returning?
	enum LexState{ START, INID, STRstate, INTstate, COMMENT/* others */ };
	LexState lexstate = START;
	string lexeme;
	char ch;
	lexeme = "";
	while(in.get(ch)) {
		// cout << "ch:" << ch << ", state:" << lexstate << endl;
		switch( lexstate) {
			case START: //NEW
				lexeme = "";
				if( ch == '\n' ) {
					linenum++;
					continue;
				}

				if( isspace(ch) )
					continue;

				if( isalpha(ch) ) {
					// lexeme += ch;
					lexstate = INID; //is a character
					in.putback(ch);
					continue;
				}

				if (isdigit(ch)){
					// lexeme += ch;
					lexstate = INTstate;
					in.putback(ch);
					continue;
				}

				if (ch == '"'){
					lexstate = STRstate; //beginning of a string
					continue;
				}

				if (ch == '/'){ //check if it is a comment
					if (char(in.peek()) == '/'){
						lexstate = COMMENT;
						in.get(ch);
						continue;
					}
				}

			case INID: //A LETTER FOLLOWED BY 0 OR MORE LETTERS
				if (ch == '.' || ch == '$' || ch == '_'){
					// linenum++;
					lexeme = ch;
					Lex lex(ERR, lexeme, linenum);
					return lex;
				}
				if (ch == '+'){
					Lex lex(PLUS, lexeme, linenum);
					return lex;
				}
				if (ch == '-'){
					Lex lex(MINUS, lexeme, linenum);
					return lex;
				}

				if (ch == '*'){
					Lex lex(STAR, lexeme, linenum);
					return lex;
				}
				if (ch == '/'){
					Lex lex(SLASH, lexeme, linenum);
					return lex;
				}

				if (ch == '!'){
					Lex lex(BANG, lexeme, linenum);
					return lex;
				}

				if (ch == '('){
					Lex lex(LPAREN, lexeme, linenum);
					return lex;
				}

				if (ch == ')'){
					Lex lex(RPAREN, lexeme, linenum);
					return lex;
				}

				if (ch == ';'){
					Lex lex(SC, lexeme, linenum);
					return lex;
				}
				
				if(ch == '\n' || isspace(ch) || in.peek() == -1 || in.peek() == 10){
					if (in.peek() == -1 || in.peek() == 10){
						lexeme += ch;
						Lex lex(ID, lexeme, linenum);
						return lex;
					}
					if (in.peek() == 10){
						linenum++;
						lexeme += ch;
						Lex lex(ID, lexeme, linenum);
						return lex;
					}
					// first check if the lexeme is a reserved keyword
					if (lexeme == "print"){
						Lex lex(PRINT, lexeme, linenum);
						return lex;
					}
					else if (lexeme == "let"){
						Lex lex(LET, lexeme, linenum);
						return lex;
					}
					else if (lexeme == "if"){
						Lex lex(IF, lexeme, linenum);
						return lex;
					}
					else if (lexeme == "loop"){
						Lex lex(LOOP, lexeme, linenum);
						return lex;
					}
					else if (lexeme == "begin"){
						Lex lex(BEGIN, lexeme, linenum);
						return lex;
					}
					else if (lexeme == "end"){
						Lex lex(END, lexeme, linenum);
						return lex;
					}
					else if (isspace(ch)){
						Lex lex(ID, lexeme, linenum);
						return lex;
					}
					else { // is \n
						linenum++;
						Lex lex(ID, lexeme, linenum);
						return lex;
					}
				}

				if (isalpha(ch) || isdigit(ch)){
					lexeme += ch;
					if (in.peek() == '.' || in.peek() == '+' || in.peek() == '-' || in.peek() == '*' || in.peek() == '/' || in.peek() == '!' || in.peek() == '(' || in.peek() == ')' || in.peek() == ';'){
						Lex lex(ID, lexeme, linenum);
						return lex;
					}
					else {
						continue;
					}
				}

				


			case STRstate: //when alpha is enclosed by ""
				if (ch == '\n'){ //if nextline character
					lexeme += ch;
					Lex lex(ERR, "\"" + lexeme, linenum);
					return lex;
				}
				if (ch == '\\'){ //special characters
					if (in.peek() == 'n'){//if a written "\n"
						in.get();
						lexeme += "\n";
						continue;
					}
					else {
						in.get(ch);
						lexeme += ch;
						continue;
					}
					// if (in.peek() == '\\'){
					// 	in.get();
					// 	lexeme += "\\";
					// 	continue;
					// }
				}
				if (ch != '"'){// if next character isn't a \n or "
					lexeme += ch;
					continue;
				}
				
				

				// is this needed?
				// if (ch == '\\'){
				// 	lexeme += '\\';
				// 	continue;
				// }

				if (ch == '"'){ //closing "
					Lex lex(STR, lexeme, linenum);
					return lex;
				}

				// if (ch == '\n'){ //if the string goes to next line without a closing "
				else {
					linenum++;
					lexeme += ch;
					Lex lex(ERR, lexeme, linenum);
					return lex;
				}

			case INTstate:
				if (isspace(ch)){ //if space, reset to START state
					Lex lex(INT, lexeme, linenum);
					return lex;
					// lexstate = START;
					// lexeme = "";
					// continue;
				}

				if (isdigit(ch)){
					lexeme += ch;
					if (in.peek() == '.'){
						Lex lex(INT, lexeme, linenum);
						return lex;
					}
					else {
						continue;
					}
				}

				if (ch == '.'){
					linenum++;
					Lex lex(ERR, ".", linenum);
					return lex;
				}

				if (ch == '\n' || in.peek() == -1){
					
					if (in.peek() == -1)
						lexeme += ch;
					// INTS.push_back(ch);
					lexeme = "";
					lexstate = START;
					continue;
				}

				else {
					// lexeme += ch;
					// linenum++;
					// Lex lex(ERR, lexeme, linenum);
					// return lex;
					in.putback(ch);
					Lex lex(INT, lexeme, linenum);
					return lex;
				}

			case COMMENT: //IGNORE ALL COMMENTS
				if (ch == '\n' || in.peek() == -1 || in.peek() == 10){
					if (ch == '\n')
						linenum++;
					lexstate = START;
					continue;
				}
				else {
					continue;
				}
			// case DONE:
			// 	return lex;

			// case ERR:
			// 	cout << "ERROR" << endl;
			// 	return lex;
		}
	}
	Lex lex(DONE, lexeme, linenum);
	return lex;
};

ostream& operator<<(ostream& out, const Lex& tok){
	switch(tok.GetToken()){
		case PRINT:
			out << "PRINT" << endl;
			return out;
		case LET:
			out << "LET" << endl;
			return out;
		case IF:
			out << "IF" << endl;
			return out;
		case LOOP:
			out << "LOOP" << endl;
			return out;
		case BEGIN:
			out << "BEGIN" << endl;
			return out;
		case END:
			out << "END" << endl;
			return out;
		case ERR:
			out << "Error on line " << tok.GetLinenum()+1 << " (" << tok.GetLexeme() << ")" << endl;
			return out;
		case DONE:
			out << "DONE" << endl;
			return out;
		case ID:
			out << "ID(" << tok.GetLexeme() << ")" << endl;
			return out;
		case INT:
			out << "INT(" << tok.GetLexeme() << ")" << endl;
			return out;
		case STR:
			out << "STR(" << tok.GetLexeme() << ")" << endl;
			return out;
		case PLUS:
			out << "PLUS" << endl;
			return out;
		case MINUS:
			out << "MINUS" << endl;
			return out;
		case STAR:
			out << "STAR" << endl;
			return out;
		case SLASH:
			out << "SLASH" << endl;
			return out;
		case BANG:
			out << "BANG" << endl;
			return out;
		case LPAREN:
			out << "LPAREN" << endl;
			return out;
		case RPAREN:
			out << "RPAREN" << endl;
			return out;
		case SC:
			out << "SC" << endl;
			return out;

	}
	return out;
}