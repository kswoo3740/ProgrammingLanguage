// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"
#include <iostream>
#include <string>

using namespace std;

bool makeGraph(const char* regexp, vector<FSATableElement>& elements, set<int>& state, const vector<char> alphabet, int& cursor, int first_state, int final_state) {
	char handle;
	int current_state = first_state;
  state.insert(final_state);
	state.insert(current_state);
	int next_state = state.size() + 1;
	int start_cursor = cursor;
  bool accept = true;
	//cout << "make" << "first : "<<first_state<<"   final : "<<final_state<<endl;

	while ((handle = regexp[cursor]) != '\0' && cursor < strlen(regexp)) {
		cursor++;
		if (handle == ANYCHAR) {
			// Input is '.'
			//cout << "Any Char!" << endl;
			FSATableElement elem;
			elem.state = current_state;
			elem.next_state = next_state;
			elem.str.append(alphabet.begin(), alphabet.end());
			elements.push_back(elem);

			if (regexp[cursor] == STAR) {
				//cout << "STAR" << endl;
				elem.str = "";
				elements.push_back(elem);
				elem.state = next_state;
				elem.next_state = current_state;
				elem.str = "";
				elements.push_back(elem);
				cursor++;
			}
			//for (int i = 0; i < elem.str.length(); i++) cout<< "str : "<< elem.str[i]<<endl;
		}
		else if (handle == LPAREN) {
			//cout << "LParen" << endl;
			// Input is '('
			accept = makeGraph(regexp, elements, state, alphabet, cursor, current_state, next_state);

      if (!accept) return false;

			if (regexp[cursor] == STAR) {
				//cout << "STAR" << endl;
				FSATableElement elem;
				elem.state = current_state;
				elem.next_state = next_state;
				elem.str = "";
				elements.push_back(elem);
				elem.state = next_state;
				elem.next_state = current_state;
				elem.str = "";
				elements.push_back(elem);
				cursor++;
			}
		}
		else if (handle == RPAREN) {
			//cout << "RParen" << endl;
			// Input is ')'
			// need more???
			if (regexp[cursor] == STAR && regexp[start_cursor - 1] == LPAREN) {
				//cout << "STAR" << endl;
				FSATableElement elem;
				elem.state = first_state;
				elem.next_state = final_state;
				elem.str = "";
				elements.push_back(elem);
				elem.state = final_state;
				elem.next_state = first_state;
				elements.push_back(elem);
				cursor++;
			}
			break;
		}
		else if (handle == LSET) {
			//cout << "LSet" << endl;
			// Input is '['
			FSATableElement elem;
			elem.state = current_state;
			elem.next_state = next_state;
      elem.str = "";
      elements.push_back(elem);
      state.insert(next_state);
      current_state = next_state;
      next_state = state.size() + 1;

      elem.state = current_state;
      elem.next_state = next_state;
			while ((handle = regexp[cursor++]) != RSET) {
				elem.str.push_back(handle);
			}
			elements.push_back(elem);

			if (regexp[cursor] == STAR) {
				elem.str = "";
				elements.push_back(elem);
				elem.state = next_state;
				elem.next_state = current_state;
				elem.str = "";
				elements.push_back(elem);
				cursor++;
			}
		}
		else if (handle == STAR) {
			// cout << "Star" << endl;
			// Input is '*' without any string for accept star
      accept = false;
		}
		else if (handle == OR) {
			//cout << "OR" << endl;
			// Input is '|'
			accept = makeGraph(regexp, elements, state, alphabet, cursor, first_state, current_state);

      if (!accept) return false;

			if (regexp[cursor - 1] == RPAREN) {
				if (regexp[cursor] == STAR && regexp[start_cursor - 1] == LPAREN) {
					//cout << "STAR" << endl;
					FSATableElement elem;
					elem.state = first_state;
					elem.next_state = final_state;
					elem.str = "";
					elements.push_back(elem);
					elem.state = final_state;
					elem.next_state = first_state;
					elements.push_back(elem);
					cursor++;
				}
				//cout << "end or" << endl;
				break;
			}
			else continue;
		}
		else {
			//cout << "character : " << handle << endl;
			// Input is character
			FSATableElement elem;
			elem.state = current_state;
			elem.next_state = next_state;
			elem.str.push_back(handle);
			elements.push_back(elem);

			if (regexp[cursor] == STAR) {
				//cout << "STAR" << endl;
				elem.str = "";
				elements.push_back(elem);
				elem.state = next_state;
				elem.next_state = current_state;
				elem.str = "";
				elements.push_back(elem);
				cursor++;
			}
		}
		current_state = next_state;
		state.insert(current_state);
		next_state = state.size() + 1;
	}
	if (current_state != final_state) {
		FSATableElement elem;
		elem.state = current_state;
		elem.next_state = final_state;
		elem.str = "";
		elements.push_back(elem);
		//cout << "current  : " << current_state << "final : " << final_state << endl;
		//cout << "end make" << endl;
	}
  return accept;
}

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
	vector<FSATableElement> elements;
	int current_state = 1;
	int next_state = 2;
	int cursor = 0;
	vector<char> alphabet;
	int paren_count = 0;
	int set_count = 0;
	vector<int> accept_states;
	accept_states.push_back(2);
  bool accept = true;

	//FiniteStateAutomaton fsa = new FiniteStateAutomaton;

	for (int i = 0; i < strlen(regexp); i++) {
		if (regexp[i] == LPAREN) paren_count++;
		else if (regexp[i] == RPAREN) paren_count--;
		if (paren_count < 0) return false;
	}

	if (paren_count != 0) return false;

	for (int i = 0; i < strlen(regexp); i++) {
		if (regexp[i] == LSET) set_count++;
		else if (regexp[i] == RSET) set_count--;
		if (set_count < 0) return false;
	}

	if (set_count != 0) return false;

	for (char alpha = 'a'; alpha <= 'z'; alpha++) alphabet.push_back(alpha);
	for (char alpha = 'A'; alpha <= 'Z'; alpha++) alphabet.push_back(alpha);
	for (char alpha = '0'; alpha <= '9'; alpha++) alphabet.push_back(alpha);
	alphabet.push_back('_');

	set<int> state;
	state.insert(1);
	state.insert(2);

	accept = makeGraph(regexp, elements, state, alphabet, cursor, current_state, next_state);

  if (!accept) return false;

	//cout << elements.size() << endl;
  /*
	for (int i = 0; i < elements.size(); i++) {
		cout << "state : " << elements[i].state << "next : " << elements[i].next_state << "str : " << elements[i].str << endl;
	}
	cout << "going to build" << endl;
  */
	//regexp_matcher->fsa->accept_states.assign(accept_states.begin(), accept_states.end());
	BuildFSA(elements, accept_states, &regexp_matcher->fsa);

	return true;
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
	return RunFSA(regexp_matcher.fsa, str);
}

void print_elem(vector<FSATableElement>& elem) {
	for (int i = 0; i < elem.size(); i++) {
		cout << "state : " << elem[i].state << endl;
		cout << "next_state : " << elem[i].next_state << endl;
		cout << "str : " << elem[i].str << endl;
	}
}
