#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <list>
using namespace std;





/* ����������� ������������ ��� ������
* �������������
* ����������������� �����
* ���������� ��������
* �������������� ��������
* �����������
* �����
* �� ������������
*/
enum Token {
	identifier,
	reserved,
	logical,
	arithmetic,
	separator,
	number,
	unknown
};


/* ��������� ������
* ��� ������
* ��������
* ������� ������ ��� ������
*/
struct token {
	Token type;
	string value;
	int level;
};



vector <token> t;
vector <token> Tree;

int FindCout(vector <token> t, string str, int start, int end);
int EntryLast(vector <token> t, string str, int start, int end);
int EntryFirst(vector <token> t, string str, int start, int end);
int FindClose(vector <token> t, int first);
int FindCloseBool(vector <token> t, int first);
vector <token> Slice(vector <token> t, int start, int end);
vector <token> LexicalAnalyzer(string str);
bool Factor(vector <token> t, int i, int level, int pos);
bool Term(vector <token> t, int i, int leve, int posl);
bool ArifmExpr(vector <token> t, int i, int level, int pos);
bool LogicExpr(vector <token> t, int i, int level, int pos);
bool BoolExpr(vector <token> t, int i, int level, int pos);
bool Expr(vector <token> t, int i, int level, int pos);
bool CompoundExpr(vector <token> t, int i, int level, int pos);
bool Cmd(vector <token> t, int i, int level, int pos);
bool Condition(vector <token> t, int i, int level, int pos);
bool LoopPostcondition(vector <token> t, int i, int level, int pos);
bool Loop(vector <token> t, int i, int level, int pos);
bool LoopPrecondition(vector <token> t, int i, int level, int pos);
bool Parser(vector <token> t, int i, int level, int pos);
void print(vector <token> t);


string file = "Text.txt";

/* ����� ���������� ���������� �������� � �������
*  ������� ������: ������, �������, ������ ������� �������, ������ ���������� �������, ������� ��� �������
*  �������� ������: ���������� ���������
*/
int FindCout(vector <token> t, string str, int start, int end) {
	int count = 0;
	for (int i = start; i < end; i++) {
		if (t[i].value == str) count++;
	}
	return count;
}

/* ����� ������� ���������� ��������� �������� � �������� ���������
*  ������� ������: ������, �������, ������ ������� �������, ������ ���������� �������, ������� ��� �������
*  �������� ������: ������ ��������
*/
int EntryLast(vector <token> t, string str, int start, int end) {
	int index = -1;
	for (int i = start; i < end; i++) {
		if (t[i].value == str) index = i;
	}
	return index;
}

/* ����� ������� ������� ��������� �������� � �������� ���������
*  ������� ������: ������, �������, ������ ������� �������, ������ ���������� �������, ������� ��� �������
*  �������� ������: ������ ��������
*/
int EntryFirst(vector <token> t, string str, int start, int end) {
	int index = -1;
	for (int i = start; i < end; i++) {
		if (t[i].value == str) {
			index = i;
			break;
		}
	}
	return index;
}


/*������ ������������� } ������ ��� �������������
* ������� ������: ������, ������ �������������
* �������� ������: ������ �������������
*/
int FindClose(vector <token> t, int first) {
	int index = -1, count = 0;

	for (int i = first; i < t.size(); i++) {
		if (t[i].value == "{") count++;
		else if (t[i].value == "}") {
			count--;
			index = i;
		}
		if (count == 0) break;
	}
	return index;
}

/*������ ������������� ) ������ ��� �������������
* ������� ������: ������, ������ �������������
* �������� ������: ������ �������������
*/
int FindCloseBool(vector <token> t, int first) {
	int index = -1, count = 0;

	for (int i = first; i < t.size(); i++) {
		if (t[i].value == "(") count++;
		else if (t[i].value == ")") {
			count--;
			index = i;
		}
		if (count == 0) break;
	}
	return index;
}

/* ���� �������
* ������� ������: ������,������ ������� ������� (������), ������ ���������� ������� (�������)
* �������� ������: ������
*/
vector <token> Slice(vector <token> t, int start, int end) {
	vector <token> temp;
	if (end == t.size()) end = t.size() - 1;
	if (start < 0) start = 0;

	for (int i = start; i < end + 1; i++) {
		temp.push_back(t[i]);
	}

	return temp;
}

/* ����������� ����������
*  ������� ������: ������
*  �������� ������: ������ �������
*/
vector <token> LexicalAnalyzer(string str) {

	int level = 1;
	token token;
	token.type = unknown;
	token.level = level;
	string s = " ";
	bool flag = true;

	// ��� ���������� ���������
	smatch m;
	string delim(" ");
	size_t prev = 0;
	size_t next;
	size_t delta = delim.length();
	regex regex_identifier("([A-Z]|[a-z])([A-Z]|[a-z]|[0-9])*");
	regex regex_reserved("(\W|^)while|if|else|for|do(\W|$)");
	regex regex_separator("[=]|[,]|[.]|[;]|[:]|[(]|[)]|[{]|[}]");
	regex regex_logical("(\W|^)~|�|�|<|>|<=|>|>=|==|!=|&|&&|[\^]|\|\||\||(\W|$)");
	regex regex_arithmetic("[*]|[/]|[%]|[+]|[-]");
	regex regex_number("[1-9]{1}([0-9])*");

	//��������� ������ �� ������� �� ������ 
	while ((next = str.find(delim, prev)) != string::npos) {
		token.value = str.substr(prev, next - prev);
		t.push_back(token);
		prev = next + delta;
	}

	//����������� ���� ������ 
	for (int i = 0; i < t.size(); i++) {
		if (regex_match(t.at(i).value, regex_reserved)) t.at(i).type = reserved;
		else if (regex_match(t.at(i).value, regex_identifier)) t.at(i).type = identifier;
		else if (regex_match(t.at(i).value, regex_separator)) t.at(i).type = separator;
		else if (regex_match(t.at(i).value, regex_number)) t.at(i).type = number;
		else if (regex_match(t.at(i).value, regex_arithmetic)) t.at(i).type = arithmetic;
		else if (regex_match(t.at(i).value, regex_logical)) t.at(i).type = logical;
		else flag = 0;
	}

	Tree = t;


	if (flag && Parser(t, 0, level, 0)) {
		for (int i = 0; i < t.size(); i++) {
			cout << "token [" << i << "]: " << Tree[i].type << " - " << Tree[i].value << endl;
		}
		cout << "Success" << endl;
		print(t);
	}
	else cout << "Error" << endl;
	return t;
}


/*�������������� ����������
* Factor - ���������
* Term - ���������
* ArifmExpr - �������������� ���������
* LogicExpr - ���������� ���������
* BoolExpr - ������ ���������
* Expr - ���������
* Cmd - ������������
* Condition - ������� if {..} else if {..} else {..}
* LoopPostcondition - ���� � ������������ do {..} while;
* Loop - ���� � ��������� for (;;) {..}
* LoopPrecondition - - ���� � ������������ while {..}
* Parser - �������������� ����������
*/


bool Factor(vector <token> t, int i, int level, int pos) {
	if (i > t.size()) return false;
	else if ((t.at(i).type == identifier) || (t.at(i).type == number)) {
		Tree[pos + i].level = level;
		return true;
	}
	else  return false;

}

bool Term(vector <token> t, int i, int level, int pos) {
	int index = EntryFirst(t, "*", i, t.size());
	if (index == -1) index = EntryFirst(t, "/", i, t.size());
	if (index == -1) index = EntryFirst(t, "%", i, t.size());

	if (i > t.size()) return false;
	else  if (index != -1 && index < t.size()) {
		Tree[pos + index].level = level;
		level++;
		return Term(Slice(t, i, index - 1), 0, level, pos) && Term(Slice(t, index + 1, t.size()), 0, level, pos + index + 1);
	}
	else if (t.size() == 1) { return Factor(t, i, level, pos); }
	else return false;
}

bool ArifmExpr(vector <token> t, int i, int level, int pos) {
	int index = EntryFirst(t, "+", i, t.size());
	if (index == -1) index = EntryFirst(t, "-", i, t.size());
	if (i > t.size()) return false;
	else  if (index != -1 && index < t.size()) {
		Tree[pos + index].level = level;
		level++;
		return ArifmExpr(Slice(t, i, index - 1), 0, level, pos) && ArifmExpr(Slice(t, index + 1, t.size()), 0, level, pos + index + 1);
	}
	else  return Term(t, i, level, pos);
}

bool LogicExpr(vector <token> t, int i, int level, int pos) {

	int index = EntryFirst(t, "&", i, t.size());
	if (index == -1) index = EntryFirst(t, "|", i, t.size());
	if (index == -1) index = EntryFirst(t, "^", i, t.size());
	if (index == -1) index = EntryFirst(t, "<<", i, t.size());
	if (index == -1) index = EntryFirst(t, ">>", i, t.size());
	if (i > t.size()) return false;
	else  if (index != -1 && index < t.size()) {
		Tree[pos + index].level = level;
		level++;
		return LogicExpr(Slice(t, i, index - 1), 0, level, pos) && LogicExpr(Slice(t, index + 1, t.size()), 0, level, pos + index + 1);
	}
	else if (t.size() == 1) return Factor(t, i, level, pos);
	else return false;
}

bool BoolExpr(vector <token> t, int i, int level, int pos) {
	int index = EntryFirst(t, "<", i, t.size());
	if (index == -1) index = EntryFirst(t, ">", i, t.size());
	if (index == -1) index = EntryFirst(t, "<=", i, t.size());
	if (index == -1) index = EntryFirst(t, ">=", i, t.size());
	if (index == -1) index = EntryFirst(t, "==", i, t.size());
	if (index == -1) index = EntryFirst(t, "!=", i, t.size());
	if (index == -1) index = EntryFirst(t, "&&", i, t.size());
	if (index == -1) index = EntryFirst(t, "||", i, t.size());
	if (i > t.size()) return false;
	else  if (index != -1 && index < t.size()) {
		Tree[pos + index].level = level;
		level++;
		return BoolExpr(Slice(t, i, index - 1), 0, level, pos) && BoolExpr(Slice(t, index + 1, t.size()), 0, level, pos + index + 1);
	}
	else if (t.size() == 1) return Factor(t, i, level, pos);
	else return false;
}

bool Expr(vector <token> t, int i, int level, int pos) {
	bool logic = false;
	bool arifm = false;
	for (int i = 0; i < t.size(); i++) {
		if (t[i].type == logical) {
			logic = true;
			break;
		}
		if (t[i].type == arithmetic) {
			arifm = true;
			break;
		}
	}
	if (i > t.size()) return false;
	else if (logic && arifm) return false;
	else if (logic) return LogicExpr(t, i, level, pos);
	else if (arifm) return ArifmExpr(t, i, level, pos);
}

bool CompoundExpr(vector <token> t, int i, int level, int pos) {
	int count = FindCout(t, "(", i, t.size());
	if (count > 0) {
		return CompoundExpr(Slice(t, EntryFirst(t, "(", i, t.size()) + 1, FindCloseBool(t, EntryFirst(t, "(", i, t.size())) - 1), i, level, pos);
	}
	else return Expr(t, i, level, pos);

}

bool Cmd(vector <token> t, int i, int level, int pos) {
	if (i > t.size()) return false;
	else if (t[i].type == identifier && t[i + 1].value == "=" && t[t.size() - 1].value == ";") {
		Tree[pos + i].level = level - 1;
		Tree[pos + i + 1].level = level;
		int end = t.size() - 1;
		Tree[pos + end].level = level;
		int index = EntryFirst(t, "=", i, t.size());
		return CompoundExpr(Slice(t, index + 1, EntryFirst(t, ";", i, t.size()) - 1), 0, ++level, pos + index + 1);

	}
	else return false;
}

bool Condition(vector <token> t, int i, int level, int pos) {
	if (i > t.size()) return false;
	else if (t[i].value == "if" &&											/*���� ����������� ����������������� ����� if*/
		t[i + 1].value == "(" &&											/*� ��������� ������ "(" */
		t[EntryFirst(t, "{", 0, t.size()) - 1].value == ")" &&			    /*� ����� ������ "{" ���� ")"*/
		EntryLast(t, "else", 0, t.size()) == -1 &&						    /*� ���� ��� "else"*/
		t[t.size() - 1].value == "}")										/*� ��������� ������ "}"*/
	{
		Tree[pos + i].level = level;
		Tree[pos + i + 1].level = level;
		Tree[pos + EntryFirst(t, "{", 0, t.size()) - 1].level = level;
		Tree[pos + EntryFirst(t, "{", 0, t.size())].level = level;
		int end = t.size() - 1;
		Tree[pos + end].level = level;
		return BoolExpr(Slice(t, EntryFirst(t, "(", 0, t.size()) + 1, EntryFirst(t, "{", 0, t.size()) - 2), i, ++level, pos + i + 2) &&			     /*������ ���������*/
			Parser(Slice(t, EntryFirst(t, "{", 0, t.size()) + 1, EntryLast(t, "}", 0, t.size()) - 1), i, ++level, pos + EntryFirst(t, "{", 0, t.size()) + 1);							 /*������������������ � "{ .. }"*/
	}
	else if (t[i].value == "if" &&										    /*���� ����������� ����������������� ����� if*/
		t[i + 1].value == "(" &&										    /*� ��������� ������ "(" */
		t[EntryFirst(t, "{", 0, t.size()) - 1].value == ")" &&              /*� ����� ������ "{" ���� ")"*/
		EntryLast(t, "else", 0, t.size()) != -1 &&                          /*� ���� ���� �� ���� "else"*/
		t[EntryFirst(t, "else", 0, t.size()) - 1].value == "}" &&           /*� ����� ������ "else" ���� "}" */
		t[t.size() - 1].value == "}") 										/*� ��������� ������ "}"*/
	{
		Tree[pos + i].level = level;
		Tree[pos + i + 1].level = level;
		Tree[pos + EntryFirst(t, "{", 0, t.size()) - 1].level = level;
		Tree[pos + EntryFirst(t, "else", 0, t.size()) - 1].level = level;
		int end = t.size() - 1;
		Tree[pos + end].level = level;
		return BoolExpr(Slice(t, EntryFirst(t, "(", 0, t.size()) + 1, EntryFirst(t, "{", 0, t.size()) - 2), i, ++level, pos + i + 2) &&       /*������ ���������*/
			Parser(Slice(t, EntryFirst(t, "{", 0, t.size()) + 1, EntryFirst(t, "else", 0, t.size()) - 2), i, ++level, pos + EntryFirst(t, "{", 0, t.size()) + 1) &&              /*������������������ ����� ������� if */
			Condition(Slice(t, EntryFirst(t, "else", 0, t.size()) + 1, t.size() - 1), i, ++level, pos + EntryFirst(t, "else", 0, t.size()) + 1);                                      /*��������� ����� �������*/
	}
	else return Parser(Slice(t, EntryFirst(t, "{", 0, t.size()) + 1, EntryLast(t, "}", 0, t.size()) - 1), i, ++level, pos + EntryFirst(t, "{", 0, t.size()) + 1);            /*������������������ � "{ .. }" ����� else*/
}

bool LoopPrecondition(vector <token> t, int i, int level, int pos) {
	if (i > t.size()) return false;
	else if (t[i].value == "while" &&										/*���� ����������� ����������������� ����� while*/
		t[i + 1].value == "(" &&											/*� ��������� ������ "(" */
		t[EntryFirst(t, "{", 0, t.size()) - 1].value == ")" &&			    /*� ����� ������ "{" ���� ")"*/
		t[t.size() - 1].value == "}") {										/*� ��������� ������ "}"*/

		Tree[pos + i].level = level;
		Tree[pos + i + 1].level = level;
		Tree[pos + EntryFirst(t, "{", 0, t.size()) - 1].level = level;
		Tree[pos + t.size() - 1].level = level;
		return BoolExpr(Slice(t, EntryFirst(t, "(", 0, t.size()) + 1, EntryFirst(t, "{", 0, t.size()) - 2), i, ++level, pos + EntryFirst(t, "(", 0, t.size()) + 1) &&			     /*������ ���������*/
			Parser(Slice(t, EntryFirst(t, "{", 0, t.size()) + 1, FindClose(t, EntryFirst(t, "{", 0, t.size())) - 1), i, ++level, pos + EntryFirst(t, "{", 0, t.size()) + 1);			 /*������������������ � "{ .. }"*/
	}
	else return false;
}


bool LoopPostcondition(vector <token> t, int i, int level, int pos) {
	int index = FindClose(t, EntryFirst(t, "{", 0, t.size()));
	if (i > t.size()) return false;
	else if (t[i].value == "do" &&												 /*���� ����������� ����������������� ����� while*/
		t[i + 1].value == "{" &&												 /*� ��������� ������ "{" */
		t[t.size() - 1].value == ";" &&											 /*� ��������� ������ ";" */
		t[index].value == "}" &&												 /*� ����� "while" ������ "}" */
		t[index + 1].value == "while" &&									   	 /* � ���� ���� ����������������� ����� while*/
		t[index + 2].value == "(" &&											 /*� ����� while ���� "(" */
		t[t.size() - 2].value == ")") {											 /*� ����� ";" ������ ")" */

		Tree[pos + i].level = level;
		Tree[pos + i + 1].level = level;
		Tree[pos + t.size() - 1].level = level;
		Tree[pos + index].level = level;
		Tree[pos + index + 1].level = level;
		Tree[pos + index + 2].level = level;
		Tree[pos + t.size() - 2].level = level;
		return BoolExpr(Slice(t, index + 3, t.size() - 3), i, ++level, pos + index + 3) &&			     /*������ ���������*/
			Parser(Slice(t, i + 2, index - 1), i, ++level, pos + i + 2);                              /*������������������ � "{ .. }"*/
	}
	else return false;

}


bool Loop(vector <token> t, int i, int level, int pos) {
	int index = EntryFirst(t, ")", i, t.size());
	token sep;
	sep.type = separator;
	sep.value = ";";
	t.insert(t.begin() + index, sep);

	index = EntryFirst(t, ")", i, t.size());
	int count = FindCout(t, ";", EntryFirst(t, "(", i, t.size()), index - 1);
	int f_sep = EntryFirst(t, ";", i, index); // ������ ��������� ";"

	if (i > t.size()) return false;
	else if (t[i].value == "for" &&												/*���� ����������� ����������������� ����� for*/
		t[i + 1].value == "(" &&												/*� ��������� ������ "("*/
		count == 2 &&
		t[index + 1].value == "{" &&											/*� ��������� ������ ����� ")" "{"*/
		t[t.size() - 1].value == "}")											/*� ��������� ������ "}"*/
	{
		Tree[pos + i].level = level;
		Tree[pos + i + 1].level = level;
		Tree[pos + index + 1].level = level;

		Tree[pos + t.size() - 2].level = level;
		++level;
		return Cmd(Slice(t, EntryFirst(t, "(", 0, index) + 1, f_sep), i, level, pos + 1) &&					/*����������������� �� "( .. ;"*/
			BoolExpr(Slice(t, f_sep + 1, EntryFirst(t, ";", f_sep + 1, index) - 1), i, level, pos + f_sep + 1) &&		/*������ ���������*/
			Cmd(Slice(t, EntryFirst(t, ";", f_sep + 1, index) + 1, index - 1), i, level, pos + EntryFirst(t, ";", f_sep + 1, index) + 1) &&			/*����������������� �� "; .. )"*/
			Parser(Slice(t, index + 2, t.size() - 2), i, level, pos + index + 2);
	}										/*������������������ � "{ .. }"*/
	else return false;
}

bool Parser(vector <token> t, int i, int level, int pos) {
	int index;
	if (i > t.size()) return false;
	else if (t[i].type == identifier) {
		index = EntryFirst(t, ";", 0, t.size());
		if (index != -1 && index + 1 < t.size()) {
			++level;
			return  Cmd(Slice(t, i, index), i, level, pos) && Parser(Slice(t, index + 1, t.size()), i, level, pos + index + 1);


		} /*���� ��� �� ��������� �������*/
		else return Cmd(Slice(t, i, index), i, level, pos);
	}
	else if (t[i].value == "if") {
		/*���������� ������� ��������� "}" ������������� �������� if */
		index = FindClose(t, EntryFirst(t, "{", i, t.size()));
		/*���� ��� �� ��������� �������*/
		while ((index + 1 < t.size()) && !(t[index + 1].type == identifier || t[index + 1].value == "while" || t[index + 1].value == "do" || t[index + 1].value == "for" || t[index + 1].value == "if")) // ���� �� �������� ����� ������� � �� ����� ������
		{
			/*���������� ������� ��������� "}" ������������� �������� if */
			index = FindClose(t, EntryFirst(t, "{", index + 1, t.size()));
		}
		if (index + 1 < t.size()) {
			++level;
			return Condition(Slice(t, 0, index), i, level, pos) && Parser(Slice(t, index + 1, t.size()), i, level, pos + index + 1);
		}
		else return Condition(Slice(t, 0, index), i, level, pos);
	}
	else if (t[i].value == "while") {
		/*���������� ������� ��������� "}" ������������� �������� while */
		index = FindClose(t, EntryFirst(t, "{", 0, t.size()));
		if (index + 1 < t.size()) {
			++level;
			return LoopPrecondition(Slice(t, 0, index), i, level, pos) && Parser(Slice(t, index + 1, t.size()), i, level, pos + index + 1); /*���� ��� �� ��������� �������*/
		}
		else return LoopPrecondition(Slice(t, 0, index), i, level, pos);
	}
	else if (t[i].value == "do") {
		/*���������� ������� ��������� ";" ������������� �������� do */
		index = EntryFirst(t, ";", FindClose(t, EntryFirst(t, "{", 0, t.size())), t.size());
		if (index + 1 < t.size()) {
			++level;
			return LoopPostcondition(Slice(t, 0, index), i, level, pos) && Parser(Slice(t, index + 1, t.size()), i, level, pos + index + 1); /*���� ��� �� ��������� �������*/
		}
		else return LoopPostcondition(Slice(t, 0, index), i, level, pos);
	}
	else if (t[i].value == "for") {
		/*���������� ������� ��������� "}" ������������� �������� for */
		index = FindClose(t, EntryFirst(t, "{", 0, t.size()));
		if (index + 1 < t.size()) {
			++level;
			return Loop(Slice(t, 0, index), i, level, pos) && Parser(Slice(t, index + 1, t.size()), i, level, pos + index + 1); /*���� ��� �� ��������� �������*/
		}
		else return Loop(Slice(t, 0, index), i, level, pos);
	}
	else return false;

}

/*������ �� �����
* ������� ������: ��� �����
* �������� ������: ������ ��������
*/
string getStringFromFile(string name) {
	string line;
	string code = "";
	ifstream in(name);
	if (in.is_open())
	{
		while (getline(in, line))
		{
			code = code + line + " ";
		}
	}
	in.close();
	return code;
}


/*����������� �������������� ������*/
void print(vector <token> t) {
	for (int i = 0; i < Tree.size(); i++) {
		for (int j = 0; j < Tree[i].level; j++) {
			cout << " ";
		}
		cout << Tree[i].value << endl;
	}
}

int main() {
	string str;

	str = getStringFromFile(file);

	//str = "f = s - d % 6 + 3 ; a = b + c - d * e ; ";
	//str = "if ( c && c ) { d = d + d ; } else if ( c && c ) { if ( c && c ) { d = d + d ; } } ";
	//str = "while ( c && c ) { d = d + d ; } ";
	//str = "do { d = d + d ; } while ( c && c ) ; ";
	//str = "for ( i = h + 3 ; i < d ; i = i + 2 ) { d = d + d ; } ";

	LexicalAnalyzer(str);

	system("pause");
	return 0;



}

