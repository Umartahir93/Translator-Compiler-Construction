#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <cstdio>
#include<stack>
#include<set>
#include<algorithm>
#include<queue>

using namespace std;

class Parser{
private:
	struct LEX_ANAL{
		string TOK;
		string LEX;
	};
	LEX_ANAL lookAhead;
	int tabs;
	int CURRENT_TOKEN;
	string t1;
	int temp_count;
	stack<int> IN_LOOP;
	stack<string> equalizer;
	set<string> symbols;
	stack<string> callee;
	stack<string> bracket;
	vector<queue<string>> container;
	queue<string> loop_container;
	queue<int> LOOP_COUNT;
	queue<int> LOOP_ENDER;
	stack<string> backPatcher;
	stack<int> n_count;
	int loc_count;
	string IO_ERROR, SYNTAX_ERROR, ASSIGN_ERROR;
	string IDENTIFIERS;
	string KEYWORDS[6];
	ofstream st;
	int ti;
	ofstream trans;
	ofstream pars;
	ifstream LEX;
	//Translator
	int n; //line counter
	int a; //Code Segment
	int b; //Function Segment
	bool counter;
	string bf;
	string bt;
	bool inB;
	int func_count;
	bool in_loop;
	int looper;
public:
	Parser(){
		inB = false;
		looper = 0;
		func_count = 0;
		temp_count = 0;
		in_loop = false;
		lookAhead.TOK = "";
		lookAhead.LEX = "";
		tabs = 0;
		ti = 0;
		CURRENT_TOKEN = 0;
		n = 1;
		a = 0;
		b = 0;
		IO_ERROR = "[IO ERROR]";
		SYNTAX_ERROR = "[SYNTAX ERROR]";
		ASSIGN_ERROR = "[ASSIGN ERROR]";
		KEYWORDS[0] = "GETS"; KEYWORDS[1] = "IF"; KEYWORDS[2] = "ELSE"; KEYWORDS[3] = "WHILE"; KEYWORDS[4] = "RETURN";
		KEYWORDS[5] = "PRINT";
		pars.open("parse_tree.txt");
		st.open("symbol_table.txt");
		LEX.open("lex_output.txt");
		trans.open("thress_address_code.txt");
		counter = false;
		loc_count = 0;
	}
	string newTmp(){
		temp_count++;
		return "t" + to_string(temp_count);
	}
	void startParser(){
		//start the work
		DL();
	}
	bool SYMBOL_IDENTIFIER(string lex) {
		int checker = 0;
		int i = 0;
		for (int i = 0; i < 6; ++i) {
			if (lex == KEYWORDS[i]) {
				return true;
			}
		}
		return false;
	}
	bool isEQUALS(string TOK){
		if (TOK == "="){
			return true;
		}
	}
	bool isOperator(string TOK){
		if (TOK == "-" || TOK == "+" || TOK == "*" || TOK == "/"){
			return true;
		}
		return false;
	}
	bool isRO(){
		if (lookAhead.TOK == "RO"){
			if (lookAhead.LEX == "GE" || lookAhead.LEX == "GT" || lookAhead.LEX == "LE" || lookAhead.LEX == "LT" ||
				lookAhead.LEX == "EE" || lookAhead.LEX == "NE"){
				if (lookAhead.LEX == "GE")
					lookAhead.LEX = ">=";
				else if (lookAhead.LEX == "GT")
					lookAhead.LEX = ">";
				else if (lookAhead.LEX == "LE")
					lookAhead.LEX = "<=";
				else if (lookAhead.LEX == "LT")
					lookAhead.LEX = "<";
				else if (lookAhead.LEX == "EE")
					lookAhead.LEX = "==";
				else if (lookAhead.LEX == "NE")
					lookAhead.LEX = "!=";
				return true;
			}
		}
		return false;
	}
	void match(string token, std::istream &LEX){
		if (lookAhead.TOK == token){
			lookAhead = nextTok(LEX);
		}
		else{
			pars << SYNTAX_ERROR << endl;
		}

	}
	LEX_ANAL nextTok(std::istream &LEX){
		if (CURRENT_TOKEN == 0){

			string temp;
			getline(LEX, temp);
			//Sort out TOKENS and LEXEMES
			string TOKEN, LEXEME;
			bool seenTab = false;
			for (int i = 0; (unsigned)i < temp.size(); i++){
				if (!seenTab && temp[i] != '\t'){
					TOKEN += temp[i];
				}
				else if (temp[i] == '\t' || temp[i] == ' '){
					seenTab = true;
					continue;
				}
				else{
					LEXEME += temp[i];
				}
			}
			//pars << TOKEN << endl << LEXEME << endl;
			LEX_ANAL t1;
			t1.TOK = TOKEN;
			t1.LEX = LEXEME;
			CURRENT_TOKEN++;
			return t1;
		}
		else{
			string temp;
			getline(LEX, temp);
			//Sort out TOKENS and LEXEMES
			string TOKEN, LEXEME;
			bool seenTab = false;
			for (int i = 0; i < temp.size(); i++){
				if (!seenTab && temp[i] != '\t'){
					TOKEN += temp[i];
				}
				else if (temp[i] == '\t' || temp[i] == ' '){
					seenTab = true;
					continue;
				}
				else{
					LEXEME += temp[i];
				}
			}
			//pars << TOKEN << endl << LEXEME << endl;
			LEX_ANAL t1;
			t1.TOK = TOKEN;
			t1.LEX = LEXEME;
			CURRENT_TOKEN++;
			return t1;
		}
	}
	string backpatch(string one, int two){
		string tempo = one + to_string(two);
		return tempo;
	}
	void printTabs(){
		for (int i = 0; i < tabs; i++){
			pars << '\t';
		}
	}
	bool IS_IN_AO(string a){
		string NONLETTERS[4] = { "*", "/", "+", "-" };
		for (int i = 0; i < 4; i++){
			if (a == NONLETTERS[i]){
				return true;
			}
			
		}
		return false;
	}
	bool IS_IN_NL(char a){
		string NONLETTERS[20] = { "=", ",", "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "{", "}", "[", "]", "(", ")", " ", "\t", "\n" };
		for (int i = 0; i < 20; i++){
			for (int j = 0; j < NONLETTERS[i].size(); j++){
				if (a == NONLETTERS[i][j]){
					return true;
				}
			}
		}
		return false;
	}
	bool IS_IN_D(char a){
		if (a > 48 && a < 59){
			return true;
		}
		return false;
	}
	bool IS_IN_D(string& s){
		bool has_only_digits = true;
		for (size_t n = 0; n < s.length(); n++)
		{
			if (!isdigit(s[n]))
			{
				has_only_digits = false;
				return false;
			}
		}
		return true;
	}
	bool IS_IN_DT(string st){
		string WORD[] = { "int", "char" };
		for (int i = 0; i < 2; i++){
			if (st == WORD[i]){
				return true;
			}
		}
		return false;
	}
	bool IS_COMMENT(LEX_ANAL st){
		if (st.TOK == "#"){
			printTabs();
			pars << "Comment" << "\t" << lookAhead.LEX << endl;
			return true;
		}
		return false;
	}
	void DL(){
		ifstream LEX("lex_output.txt");
		//read first pair
		lookAhead = nextTok(LEX);
		//Declaration List
		printTabs();
		pars << "Declaration List" << endl;
		tabs++;
		bool SYNTAX_ERROR = true;
		//If a Syntax Error is found, the whole program will shut at the exact moment and that will be it!
		while (!LEX.eof() && SYNTAX_ERROR){
			while (IS_COMMENT(lookAhead)){
				match(lookAhead.TOK, LEX);
			}
			if (lookAhead.TOK == "DEF"){
				printTabs();
				pars << "Declaration" << endl;
				tabs++;
				printTabs();
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				SYNTAX_ERROR = FD(LEX); //Function Declaration
				if (SYNTAX_ERROR){
					while (IS_COMMENT(lookAhead)){
						match(lookAhead.TOK, LEX);
					}
					if (lookAhead.TOK == "{"){
						printTabs();
						pars << "Compound Statement" << endl;
						tabs++;
						printTabs();
						pars << lookAhead.TOK << endl;
						printTabs();
						pars << "Local Statements List" << endl;
						tabs++;
						match(lookAhead.TOK, LEX);
						while (lookAhead.TOK != "}" && SYNTAX_ERROR){
							int ls = 0;
							SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX);
							if (lookAhead.TOK == ""){
								lookAhead.TOK = "}";
								break;
							}
						}
						if (lookAhead.TOK == "}"){
							tabs--;
							tabs--;
							printTabs();
							pars << lookAhead.TOK << endl;
							match(lookAhead.TOK, LEX);
							if (lookAhead.TOK == "}")
								SYNTAX_ERROR = false;
							else
								SYNTAX_ERROR = true;
							b = a;
							trans << to_string(n)<< "\tendFunc" << endl;
							n++;
						}
					}
				}
			}
			else if (lookAhead.TOK == "INT" || lookAhead.TOK == "CHAR"){
				printTabs();
				pars << "Declaration" << endl;
				tabs++;
				printTabs();
				pars << lookAhead.TOK << endl;
				string temp = lookAhead.TOK;
				match(lookAhead.TOK, LEX);
				SYNTAX_ERROR = VD(temp, LEX); //Variable Declaration
			}
		}
		if (!SYNTAX_ERROR){
			printTabs();
			pars << "SYNTAX ERROR" << endl;
		}
	}
	bool FD(std::istream &LEX){
		printTabs();
		if (IS_COMMENT(lookAhead)){
			return false;
		}
		if (lookAhead.TOK == "ID"){
			pars << lookAhead.LEX << endl;
			st << lookAhead.LEX << "\t" << "FUN" << "\t" << b << "\t" << "POINTER" << endl;
			//Put function RA in front 
			string name = lookAhead.LEX;
			func_count = 0;
			//string temp = to_string(n) + "\t"+ "beginFunc \t" + lookAhead.LEX + "\t" + to_string(b);
			//trans << temp << endl;
			//n++;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "("){
				printTabs();
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				printTabs();
//				trans << to_string(n) << "\tfuncParameters" << endl;
				//n++;
				//pars << "Parameter List" << endl;
				tabs++;
				if (lookAhead.TOK == ")"){
					printTabs();
					pars << "VOID" << endl;
					tabs--;
					printTabs();
					pars << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);
				}
				else if (VDL(LEX)){
					if (lookAhead.TOK == ")"){
						string temp = to_string(n) + "\t" + name + " , " + to_string(func_count);
						func_count = 0;
						n++;
						trans << temp << endl;
						tabs--;
						printTabs();
						pars << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);	
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
		return true;
	}
	bool VDL(std::istream &LEX){
		if (lookAhead.TOK == "INT" || lookAhead.TOK == "CHAR"){
			printTabs();
			pars << lookAhead.TOK << endl;
			string temp = lookAhead.TOK;
			match(lookAhead.TOK, LEX);
			VD(temp, LEX);
			if (lookAhead.TOK == ","){
				printTabs();
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				return VDL(LEX);
			}
			else
				return true;
		}
		return false;
	}
	bool VD(string tok, std::istream &LEX){
		if (!in_loop){
			if (BR(LEX)){
				if (lookAhead.TOK == "ID"){
					symbols.insert(lookAhead.LEX);
					//translator
					func_count++;
					string temp = to_string(n) + "\t" + tok + "\t" + lookAhead.LEX;
					trans << temp << endl;
					n++;
					printTabs();
					pars << lookAhead.LEX << endl;
					string id = lookAhead.LEX;
					match(lookAhead.TOK, LEX);

					st << id << "\t" << tok << "\t" << a << "\t" << "null" << endl;

					if (tok == "INT"){
						a += 4;
					}
					else if (tok == "CHAR"){
						a += 1;
					}
					if (lookAhead.TOK == "="){
						printTabs();
						pars << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						if (lookAhead.TOK == "NUM"){
							printTabs();
							pars << lookAhead.LEX << endl;

							return true;
						}
						else if (lookAhead.TOK == "CHAR"){
							printTabs();
							pars << lookAhead.LEX << endl;
							return true;
						}
						else if (lookAhead.TOK == "ID"){
							printTabs();
							pars << lookAhead.LEX << endl;
							return true;
						}
						else
							return false;
					}
				}
				else
					return false;
			}
			else if (lookAhead.TOK == "ID"){
				symbols.insert(lookAhead.LEX);
				//translator
				string temp = to_string(n) + "\t" + tok + "\t" + lookAhead.LEX;
				func_count++;
				trans << temp << endl;
				n++;
				printTabs();
				pars << lookAhead.LEX << endl;
				string id = lookAhead.LEX;
				match(lookAhead.TOK, LEX);
				st << id << "\t" << tok << "\t" << a << "\t" << "null" << endl;
				if (tok == "INT"){
					a += 4;
				}
				else if (tok == "CHAR"){
					a += 1;
				}
				if (lookAhead.TOK == "="){
					printTabs();
					pars << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);

					if (lookAhead.TOK == "NUM"){
						printTabs();
						pars << lookAhead.LEX << endl;
						return true;
					}
					else if (lookAhead.TOK == "CHAR"){
						printTabs();
						pars << lookAhead.LEX << endl;
						return true;
					}
					else if (lookAhead.TOK == "ID"){
						printTabs();
						pars << lookAhead.LEX << endl;
						return true;
					}
					else
						return false;
				}
			}
			else
				return false;
		}
		else{
			if (BR(LEX)){
				if (lookAhead.TOK == "ID"){
					symbols.insert(lookAhead.LEX);
					//translator
					func_count++;
					string name = lookAhead.LEX;
					printTabs();
					pars << lookAhead.LEX << endl;
					string id = lookAhead.LEX;
					match(lookAhead.TOK, LEX);
					st << id << "\t" << tok << "\t" << a << "\t" << "null" << endl;
					if (tok == "INT"){
						a += 4;
					}
					else if (tok == "CHAR"){
						a += 1;
					}
					if (lookAhead.TOK == "="){
						printTabs();
						pars << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						string t;
						int c = 0;
						if (E(name, t,c,LEX)){
							printTabs();
							pars << lookAhead.LEX << endl;
							return true;
						}
						else
							return false;
					}
					string temp = to_string(n) + "\t" + tok + "\t" + name;
					loop_container.push(temp);
					n++;
				}
				else
					return false;
			}
			else if (lookAhead.TOK == "ID"){
				symbols.insert(lookAhead.LEX);
				//translator
				string temp = to_string(n) + "\t" + tok + "\t" + lookAhead.LEX;
				loop_container.push(temp);
				n++;
				printTabs();
				pars << lookAhead.LEX << endl;
				string id = lookAhead.LEX;
				match(lookAhead.TOK, LEX);
				st << id << "\t" << tok << "\t" << a << "\t" << "null" << endl;
				if (tok == "INT"){
					a += 4;
				}
				else if (tok == "CHAR"){
					a += 1;
				}
				if (lookAhead.TOK == "="){
					printTabs();
					pars << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);

					if (lookAhead.TOK == "NUM"){
						printTabs();
						pars << lookAhead.LEX << endl;
						return true;
					}
					else if (lookAhead.TOK == "CHAR"){
						printTabs();
						pars << lookAhead.LEX << endl;
						return true;
					}
					else if (lookAhead.TOK == "ID"){
						printTabs();
						pars << lookAhead.LEX << endl;
						return true;
					}
					else
						return false;
				}
			}
			else
				return false;
		}
	}
	bool BR(std::istream &LEX){
		if (lookAhead.TOK == "["){
			printTabs();
			pars << lookAhead.TOK << endl;
			//t1 += lookAhead.TOK;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "ID"){
				printTabs();
				pars << lookAhead.LEX << endl;
				t1 += lookAhead.TOK;
				bracket.push(lookAhead.LEX);
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "]"){
					printTabs();
					pars << lookAhead.TOK << endl;
					//t1 += lookAhead.TOK;
					match(lookAhead.TOK, LEX);
					return true;
				}
				else
					return false;
			}
			else if (lookAhead.TOK == "NUM"){
				printTabs();
				pars << lookAhead.LEX << endl;
				t1 += lookAhead.TOK;
				bracket.push(lookAhead.LEX);
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "]"){
					printTabs();
					pars << lookAhead.TOK << endl;
					//t1 += lookAhead.TOK;
					match(lookAhead.TOK, LEX);
					return true;
				}
				else
					return false;
			}
			else if(lookAhead.TOK == "]"){
				printTabs();
				pars << lookAhead.TOK << endl;
				//t1 += lookAhead.TOK;
				match(lookAhead.TOK, LEX);
				return true;
			}
			else
				return false;
		}
	}
	bool IS(bool &SYNTAX_ERROR, std::istream &LEX){
		bool wow = false;
		while (!LEX.eof() && SYNTAX_ERROR && !wow){
			if (lookAhead.TOK == "WHILE"){
				//tabs--;
				tabs++;
				callee.push("WHILE");
				printTabs();
				pars << "While Statement" << endl;
				ti = n;
				LOOP_COUNT.push(n); // for ending loop
				IN_LOOP.push(1);
				tabs++;
				printTabs();
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				if (CS(LEX)){
					string te = to_string(ti) + "\t" +backpatch(bt, n);
					//string end = to_string(ti) + "\t" + "GOTO  ";
					//LOOP_COUNT.push(ti); //End GOTO statement
					if (!in_loop){
						loop_container.push("END THE LOOP");
						loop_container.push(te);
						//loop_container.push(end);
						in_loop = true;
					}
					else{
						loop_container.push("END THE LOOP");
						loop_container.push(te);
						//loop_container.push(end);
					}
					while (IS_COMMENT(lookAhead)){
						match(lookAhead.TOK, LEX);
					}
				}
				else
					return false;
				if (lookAhead.TOK == "{"){
					IN_LOOP.push(1);
					printTabs();
					pars << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					while (lookAhead.TOK != "}" && SYNTAX_ERROR){
						SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX);
						if (lookAhead.TOK == ""){
							lookAhead.TOK = "}";
							break;
						}
					}
					if (SYNTAX_ERROR && lookAhead.TOK == "}"){
						tabs--;
						printTabs();
						pars << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						IN_LOOP.pop();
					}
					else
						return false;
				}
				else{
					SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX);
					trans << "GOTO " << to_string(ti) << endl;
					in_loop = false;
				}
			}
			else if (lookAhead.TOK == "IF"){
				tabs++;
				callee.push("IF");
				printTabs();
				pars << "If Statement" << endl;
				ti = n;
				LOOP_COUNT.push(n); // LoopBegin GOTO Statement
				IN_LOOP.push(1);
				tabs++;
				printTabs();
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				if (CS(LEX)){
					string te = to_string(ti) + "\t" + backpatch(bt, n);
					//LOOP_COUNT.push(ti + 1); //End GOTO statement
					if (in_loop){
						loop_container.push("END THE LOOP");
						loop_container.push(te);
					}
					else{
						trans << te << endl;
						in_loop = true;
					}
					while (IS_COMMENT(lookAhead)){
						match(lookAhead.TOK, LEX);
					}
				}
				else
					return false;
				if (lookAhead.TOK == "{"){
					IN_LOOP.push(1);
					printTabs();
					pars << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					while (lookAhead.TOK != "}" && SYNTAX_ERROR){
						SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX);
					}
					if (SYNTAX_ERROR && lookAhead.TOK == "}"){
						tabs--;
						string tF = to_string(n) + "\tGOTO  ";
						loop_container.push(tF);
						printTabs();
						pars << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						IN_LOOP.pop();
					}
					else
						return false;
				}
				else{
					SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX);
					//trans << "GOTO " << to_string(ti) << endl;
					in_loop = false;
				}
				if (lookAhead.TOK == "ELSE"){
					int tempELSE = n;
					IN_LOOP.pop();
					if (IN_LOOP.empty() && !in_loop){
						string ans = to_string(LOOP_COUNT.front()) + "\tGOTO  " + to_string(n + 1);
						trans << ans << endl;
						LOOP_COUNT.pop();
						n++;
						string s = loop_container.front();
						while (s != "END THE LOOP"){
							trans << s << endl;
							loop_container.pop();
							s = loop_container.front();
						}
						trans << to_string(n) << "\tGOTO  " << endl;
					}
					printTabs();
					pars << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					if (lookAhead.TOK == "ID"){
						return SYNTAX_ERROR = S(LEX);
					}
					//Multi Lined IF Statement
					else if (lookAhead.TOK == "{"){
						IN_LOOP.push(1);
						printTabs();
						pars << lookAhead.TOK << endl;
						tabs++;
						match(lookAhead.TOK, LEX);
						while (lookAhead.TOK != "}" && SYNTAX_ERROR){
							printTabs();
							//tabs++;
							SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX);
						}
						if (SYNTAX_ERROR && lookAhead.TOK == "}"){
							tabs--;
							printTabs();
							pars << lookAhead.TOK << endl;
							match(lookAhead.TOK, LEX);
							IN_LOOP.pop();
							return SYNTAX_ERROR;
						}
						else
							return false;
					}
				}
			}
			else if (lookAhead.TOK == "}" && !IN_LOOP.empty() && IN_LOOP.top() == 1){
				LOOP_ENDER.push(n);
				IN_LOOP.pop();
				if (IN_LOOP.size() == 2){
					in_loop = false;
				}
				if (!in_loop){
					if (!IN_LOOP.empty()){
						int c = LOOP_COUNT.front();
						LOOP_COUNT.pop();
						string s = loop_container.front();
						if (s == "END THE LOOP"){
							loop_container.pop();
							s = loop_container.front();
							loop_container.pop();
							if (s.find("IF")){
								trans << s << endl;
								c++;
								s = to_string(c) + "\t" + "GOTO  " + to_string(n);
								trans << s << endl;
								c++;
 							}
						}
						while (!loop_container.empty()){
							if (s == "END THE LOOP"){
								int c1 = LOOP_ENDER.front();
								LOOP_COUNT.pop();
								LOOP_ENDER.pop();
								s = loop_container.front();
								loop_container.pop();
								if (s.find("IF")){
									trans << s << endl;
									c++;
									s = to_string(c) + "\t" + "GOTO  " + to_string(c1);
									trans << s << endl;
									c++;
									s = loop_container.front();
									loop_container.pop();
								}
							}
							else{
								s = loop_container.front();
								loop_container.pop();
								if (s != "END THE LOOP"){
									trans << s << endl;
									c++;
								}
							}

						}
						//loop_container.pop();
						match(lookAhead.TOK, LEX);
					}
				}
				else{
					match(lookAhead.TOK, LEX);
				}
			}
			else{
				printTabs();
				pars << "Statement" << endl;
				SYNTAX_ERROR = S(LEX);
				wow = !SYNTAX_ERROR;
			}
		}
		return SYNTAX_ERROR;
	}
	bool S(std::istream &LEX){
		while (IS_COMMENT(lookAhead)){
			match(lookAhead.TOK, LEX);
		}
		if (lookAhead.TOK == "INT" || lookAhead.TOK == "CHAR"){
			tabs++;
			printTabs();
			pars << lookAhead.TOK << endl;
			string temp = lookAhead.TOK;
			match(lookAhead.TOK, LEX);
			if (VD(temp, LEX)){
				while (lookAhead.TOK == ","){
					tabs--;
					printTabs();
					pars << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);
					tabs++;
					VD(temp, LEX);
				}
			}
			else
				return false;
		}
		else if (lookAhead.TOK == "ID"){
			int count = 0;
			string temp = lookAhead.LEX;
			if (FP(temp, count, LEX), lookAhead.TOK == "="){
				printTabs();	
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				int i = 0;
				string ans = "";
				if (E(temp, ans,i, LEX)){
					tabs--;
					//tabs--;
					return true;
				}
				else
					return false;
			}
		}
		else if (lookAhead.TOK == "PRINT"){
			int count = 0;
			string temp = lookAhead.LEX;
			printTabs();
			pars << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			int i = 0;
			string ans = "";
			if (E(temp, ans, i, LEX)){
				string t = to_string(n) + "\tprint" + ans;
				if (in_loop){
					loop_container.push(t);
				}
				else{
					trans << t << endl;
				}
				n++;
				tabs--;
				//tabs--;
				return true;
			}
			else
				return false;
		}
		else if (lookAhead.TOK == "GETS"){
			int count = 0;
			string temp = lookAhead.LEX;
			printTabs();
			pars << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			int i = 0;
			string ans = "";
			if (E(temp, ans, i, LEX)){
				string t = to_string(n) + "\tgets" + ans;
				if (in_loop){
					loop_container.push(t);
				}
				else{
					trans << t << endl;
				}
				n++;
				tabs--;
				//tabs--;
				return true;
			}
			else
				return false;
		}
		else if (lookAhead.TOK == "RETURN"){
			int count = 0;
			string temp = lookAhead.LEX;
			printTabs();
			pars << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			int i = 0;
			string ans = "";
			if (E(temp, ans, i, LEX)){
				string t = to_string(n) + "\tret" + ans;
				if (in_loop){
					loop_container.push(t);
				}
				else{
					trans << t << endl;
				}
				n++;
				tabs--;
				//tabs--;
				return true;
			}
			else
				return false;
		}
	}
	
	//Need in-fix/post-fix expression handling
	bool E(string &iniName, string &temp,int& count, std::istream &LEX){
		if (lookAhead.TOK == "ID"){
			string v;
			string name = iniName;
			if (TP(name, v, count, LEX)){
				if (EP(name, v, count, LEX)){
					temp = to_string(n) +"\t"+ iniName + " = " +v;
					inB = false;
					if (in_loop){
						loop_container.push(temp);
						n++;
					}
					else{
						trans << temp << endl;
						n++;
					}
					return true;
				}
				temp = v;
			}
		}
		else if (lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			string s;
			string name = iniName;
			if (TP(name, s, count, LEX)){
				if (EP(name, s, count, LEX)){
					temp = to_string(n) + "\t" + iniName + " = " + s;
					if (in_loop){
						loop_container.push(temp);
						n++;
					}
					else{
						trans << temp << endl;
						n++;
					}
					return true;
				}
				temp = s;
			}
		}
	}
	bool E1(string &iniName, string &temp, int& count, std::istream &LEX){
		string tE = newTmp();
		if (lookAhead.TOK == "ID"){
			string v;
			string name = iniName;
			if (TP(name, v, count, LEX)){
				if (EP(name, v, count, LEX)){
					temp = to_string(n) + "\t" + tE + " = " + v;
					if (in_loop){
						loop_container.push(temp);
						n++;
					}
					else{
						trans << temp << endl;
						n++;
					}
					temp = tE;
					return true;
				}
				temp = tE;
			}
		}
		else if (lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			string s;
			string name = iniName;
			if (TP(name, s, count, LEX)){
				if (EP(name, s, count, LEX)){
					temp = to_string(n) + "\t" + iniName + " = " + s;
					if (in_loop){
						loop_container.push(temp);
						n++;
					}
					else{
						trans << temp << endl;
						n++;
					}
					temp = tE;
					return true;
				}
				temp = tE;
			}
		}
	}
	bool TP(string &iniName, string &temp, int& count, std::istream &LEX){
		if (lookAhead.TOK == "ID"){
			string s;
			if (F(iniName,s, count, LEX)){
				if (TPP(iniName,s, count, LEX)){
					temp = s;
					return true;
				}
				temp = s;
			}
			else
				return false;
		}
		else if (lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			string s;
			if (F(iniName,s, count, LEX)){
				if (TPP(iniName,s, count, LEX)){
					temp = s;
					return true;
				}
				temp = s;
			}
			else
				return false;

		}
	}
	bool EP(string &iniName, string temp, int& count, std::istream &LEX){
		if (lookAhead.TOK == "+"){
			string tt = temp;
			string tE = newTmp();
			temp_count--;
			string TT = tE;
			printTabs();
			pars << lookAhead.TOK << endl;
			match("+", LEX);
			string s;
			if (TP(temp ,s, count, LEX)){
				if (EP(temp, s, count, LEX)){
					string t1 = to_string(n) + "\t" + tt + " = "+ iniName + " + " + tE;
					if (in_loop){
						loop_container.push(t1);
						n++;
						temp = s;
					}
					else{
						trans << t1 << endl;
						n++;
						temp = s;
					}
					return true;
				}
				return true;
			}

			else
				return false;

		}
		else if (lookAhead.TOK == "-"){
			string tt = temp;
			string tE = newTmp();
			temp_count--;
			string TT = tE;
			printTabs();
			pars << lookAhead.TOK << endl;
			match("-", LEX);
			string s;
			if (TP(temp, s, count, LEX)){
				if (EP(temp, s, count, LEX)){
					string t1 = to_string(n) + "\t" + tt + " = " + iniName + " - " + tE;
					if (in_loop){
						loop_container.push(t1);
						n++;
						temp = s;
					}
					else{
						trans << t1 << endl;
						n++;
						temp = s;
					}
					return true;
				}
				return true;
			}
			else
				return false;
		}
		else if(!inB){
			string t1 = to_string(n) + "\t" + temp + " = " + iniName;
			if (in_loop){
				loop_container.push(t1);
				n++;
				//temp = iniName;
				return true;
			}
			else{
				trans << t1 << endl;
				n++;
				return true;
			}

		}
	}
	bool TPP(string &iniName, string &temp, int count, std::istream &LEX){
		if (lookAhead.TOK == "*"){
			string tt = temp;
			string tE = newTmp();
			temp_count--;
			string TT = tE;
			printTabs();
			pars << lookAhead.TOK << endl;
			match("*", LEX);
			string s;
			if (TP(temp, s, count, LEX)){
				if (EP(temp, s, count, LEX)){
					string t1 = to_string(n) + "\t" + tt + " = " + tE + " * " + iniName;
					if (in_loop){
						loop_container.push(t1);
						n++;
						temp = s;
					}
					else{
						trans << t1 << endl;
						n++;
						temp = s;
					}
					return true;
				}
				string t1 = to_string(n) + "\t" + tE + " = " + temp;
				if (in_loop){
					loop_container.push(t1);
					n++;
					temp = s;
				}
				else{
					trans << t1 << endl;
					n++;
					temp = s;
				}
				t1 = to_string(n) + "\t" + tt + " = " + tE + " * " + iniName;
				if (in_loop){
					loop_container.push(t1);
					n++;
					temp = s;
				}
				else{
					trans << t1 << endl;
					n++;
					temp = s;
				}
				return true;

			}

			else
				return false;

		}
		else if (lookAhead.TOK == "/"){
			string tt = temp;
			string tE = newTmp();
			temp_count--;
			string TT = tE;
			printTabs();
			pars << lookAhead.TOK << endl;
			match("/", LEX);
			string s;
			if (TP(temp, s, count, LEX)){
				if (EP(temp, s, count, LEX)){
					string t1 = to_string(n) + "\t" + tt + " = " + tE + " / " + iniName;
					if (in_loop){
						loop_container.push(t1);
						n++;
						temp = s;
					}
					else{
						trans << t1 << endl;
						n++;
						temp = s;
					}
					return true;
				}
				string t1 = to_string(n) + "\t" + tE + " = " + temp;
				if (in_loop){
					loop_container.push(t1);
					n++;
					temp = s;
				}
				else{
					trans << t1 << endl;
					n++;
					temp = s;
				}
				t1 = to_string(n) + "\t" + tt + " = " + tE + " / " + iniName;
				if (in_loop){
					loop_container.push(t1);
					n++;
					temp = s;
				}
				else{
					trans << t1 << endl;
					n++;
					temp = s;
				}
				return true;
			}
			else
				return false;
		}
		return false;
	}
	bool F(string &iniName, string& temp, int& count, std::istream &LEX){
		string tF = newTmp();
		if (lookAhead.TOK == "ID"){
			string st = lookAhead.LEX;
			string name = lookAhead.LEX;
			string ans = st;
			//n++;
			printTabs();
			pars << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "("){ // Functions being called
				string la = newTmp();
				tabs++;
				printTabs();
				ans = ans + "\t";
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				string s;
				bool o = OPL(s, count, LEX);
				if (o && lookAhead.TOK == ")"){
					ans = to_string(n) + "\t" + "call" + "\t" + st + "," + to_string(func_count) + "," + la;
					tabs++;
					printTabs();
					match(lookAhead.TOK, LEX);
					pars << lookAhead.LEX << endl;
					tabs--;
					tabs--;
					n++;
					if (in_loop){
						loop_container.push(ans);
						iniName = iniName + "(" + s + ")";
						temp = tF;
					}
					else{
						trans << ans << endl;
						temp = tF;
					}
					return true;
				}
				else
					return false;
			}
			if (lookAhead.TOK == "["){ // Array 
				string ans1 = to_string(n) + "\t" + tF + " = " + name + "[";
				string s;
				tabs++;
				printTabs();
				pars << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				string n2 = name;
				bool i = E1(n2, s, count, LEX);
				inB = true;
				if (i && lookAhead.TOK == "]"){
					ans1 += s;
					ans1 += "]";
					tabs--;
					printTabs();
					pars << lookAhead.LEX << endl;
					match(lookAhead.TOK, LEX);
					tabs--;
					if (in_loop){
						loop_container.push(ans1);
						temp = tF;
					}
					else{
						trans << ans << endl;
						temp = tF;
					}
					return true;
				}
				return false;
			}
			iniName = ans;
			temp = tF;
			return true;
		}
		else if (lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			tabs++;
			printTabs();
			pars << lookAhead.LEX << endl;
			tabs--;
			if (IS_IN_D(iniName)){
				temp = lookAhead.LEX;
				match(lookAhead.TOK, LEX);
				return true;
			}
			else{
				iniName = lookAhead.LEX;
				temp = tF;
				//temp = lookAhead.LEX;
				match(lookAhead.TOK, LEX);
				return true;
			}
		}
		else if (lookAhead.TOK == "("){
			return true;
		}
	}
	bool OPL(string& temp, int count, std::istream &LEX){
		match(lookAhead.TOK, LEX);
		if (lookAhead.TOK == "]"){
			return true;
		}
		else{
			string s;
			func_count = 0;
			bool p = PL(s, count, LEX);
			return p;
		}
	}
	bool PL(string &temp, int count, std::istream &LEX){
		string s;
		if (E(lookAhead.LEX,s, count, LEX)){
			func_count++;
			if (lookAhead.TOK == ","){
				tabs++;
				printTabs();
				pars << lookAhead.TOK << endl;
				bool b = PL(s, count, LEX);
				if (in_loop){
					temp = to_string(n) + "\t" + "Param\t" + s;
					n++;
					loop_container.push(temp);
				}
				else{
					temp = to_string(n) + "\t" + "Param\t" + s;
					n++;
					trans << temp << endl;
				}
				tabs--;
				return b;

			}
		}
	}
	bool FP(string& temp, int& count, std::istream &LEX){
		if (lookAhead.TOK == "ID"){
			string temp1 = lookAhead.LEX;
			temp = temp1;
			tabs++;
			printTabs();
			pars << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "="){
				return true;
			}
			else if(lookAhead.TOK == "["){
				printTabs();
				pars << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				string ans;
				string n2 = temp1;
				bool b = E1(n2, ans, count, LEX);
				if (b && lookAhead.TOK == "]"){
					temp += "["+ans;
					temp += "]";
					tabs--;
					printTabs();
					pars << lookAhead.LEX << endl;
					match(lookAhead.TOK, LEX);
					tabs--;
					return true;
				}
				else
					return false;
			}
		}
	}
	bool CS(std::istream &LEX){
		int t = n;
		bt.clear();
		bf.clear();
		string temp = "IF  ";
		string t4;
		string tC = lookAhead.LEX;
		int t1 = 0;
		if (lookAhead.TOK == "ID" || lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			printTabs();
			pars << lookAhead.LEX << endl;
			string name = lookAhead.LEX;
			match(lookAhead.TOK, LEX);
			if (isRO()){
				t4 = name;
				printTabs();
				pars << lookAhead.LEX << endl;
				temp += t4 + "  " + lookAhead.LEX + "  ";
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "ID" || lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
					printTabs();
					pars << lookAhead.LEX << endl;
					t4 = lookAhead.LEX;
					if (BR(LEX)) {
						string t6 = to_string(t) + "\t" + "t5 := 4 * " + bracket.top();
						bracket.pop();
						container[looper].push(t6);
						ti++;
						string t7 = to_string(t) + "t6 := " + t4 + " + t5";
						t4 = "t6";
						ti++;
					}
					temp += t4 + "  GOTO ";
					bt = temp;
					n++;
					loc_count++;
					int f = ti;
					bf = "GOTO ";
					n++;
					backPatcher.push(bf);
					match(lookAhead.TOK, LEX);
					return true;
				}
				else
					return false;
			}
			else if (BR(LEX)){
				t4 = "IF  ";
				string t2 = tC + "[" +bracket.top()+ "]";
				bracket.pop();
				n++;
				//loop_container.push(t2);
				t4 += t2;
				if (isRO()){
					printTabs();
					pars << lookAhead.LEX << endl;
					t4 += "  " + lookAhead.LEX + "  ";
					match(lookAhead.TOK, LEX);
					if (lookAhead.TOK == "ID" || lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
						printTabs();
						t2 = lookAhead.LEX;
						pars << lookAhead.LEX << endl;
						match(lookAhead.TOK, LEX);
						if (BR(LEX)) {
							t2 = t2 + "[" + bracket.top() + "]";
							bracket.pop();
							n++;
							//loop_container.push(t2);
							t4 += t2;
						}
						else{
							t4 += t2;
						}
						temp.clear();
						temp += t4 + "  GOTO ";
						bt = temp;
						//n++;
						int f = ti;
						bf = to_string(n) + "\t" + "GOTO ";
						//n++;
						backPatcher.push(bf);	
						return true;
					}
					else
						return false;
				}
			}
		}
		return false;
	}
};