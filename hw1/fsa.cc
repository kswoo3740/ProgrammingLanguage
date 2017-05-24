// PL homework: hw1
// fsa.cc

#include <iostream>
#include <queue>
#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

void NFAtoDFA(FiniteStateAutomaton& fsa, std::vector<FSATableElement> elements) {
	// Change NFA to DFA
	queue<set<int> > states_queue;
	vector<FSAdfaElement> states;
	FSAdfaElement dfa_state;
	set<int> state;
	set<char> alphabet;

	state.insert(1); // Insert start state
	epsilon_state(elements, state, 1);  // Find all possible start state
	fsa.start_state = state;
	states_queue.push(state);



	for (vector<FSATableElement>::iterator it = elements.begin(); it != elements.end(); it++) {
		// To check every aceeptable alphabet save every possible alphabet
		alphabet.insert(it->str[0]);
	}

	while (!states_queue.empty()) {
		state = states_queue.front();
		for (set<char>::const_iterator it = alphabet.begin(); it != alphabet.end(); it++) {
			// Remove the epsilon from NFA
			if (*it != '\0') {
				dfa_state.next_states = possible_state(elements, *it, state);
				dfa_state.state = state;
				dfa_state.term = *it;

				if (dfa_state.next_states.empty()) continue;

				vector<FSAdfaElement>::iterator it2;
				for (it2 = fsa.elem.begin(); it2 != fsa.elem.end(); it2++) {
					if (it2->state == dfa_state.state && it2->next_states == dfa_state.next_states && it2->term == dfa_state.term) break;
				}

				if (it2 == fsa.elem.end()) {
					fsa.elem.push_back(dfa_state);
					states_queue.push(state);
					state = dfa_state.next_states;
					states_queue.push(state);
				}
			}
		}
		states_queue.pop();
	}
}

set<int> possible_state(const vector<FSATableElement> elements, const char term, set<int> start_states) {
	// Find all possible state of states can move by term
	set<int> possible_states;
	set<int> mid_states;

	for (set<int>::iterator it = start_states.begin(); it != start_states.end(); it++) {
		// Find all possible state which can move by epsilon
		epsilon_state(elements, mid_states, *it);
	}

	for (set<int>::iterator it = start_states.begin(); it != start_states.end(); it++) {
		for (vector<FSATableElement>::const_iterator it = elements.begin(); it != elements.end(); it++) {
			set<int> temp;
			// Check epsilon state after add other possible states
			epsilon_state(elements, temp, it->state);

			if (it->str[0] == '\0') continue;

			if (mid_states.find(it->state) != mid_states.end()) {
				if (term == it->str[0]) {
					if (mid_states != temp) {
						temp = possible_state(elements, term, temp);
						possible_states.insert(temp.begin(), temp.end());
					}
					possible_states.insert(it->next_state);
				}
			}
		}
	}

	for (set<int>::iterator it = possible_states.begin(); it != possible_states.end(); it++) {
		epsilon_state(elements, possible_states, *it);
	}

	return possible_states;
}

void epsilon_state(const vector<FSATableElement> elements, set<int>& mid_states, int state) {
	// Find all possible states move by epsilon
	mid_states.insert(state);
	for (vector<FSATableElement>::const_iterator it = elements.begin(); it != elements.end(); it++) {
		if ((it->str.length() == 0) && (it->state == state) && (mid_states.find(it->next_state) == mid_states.end())) {
			mid_states.insert(it->next_state);
			epsilon_state(elements, mid_states, it->next_state);
		}
	}
}

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
	// Implement this function.
	set<int> current_state = fsa.start_state;

	if (str == "\0") {
		// If string is null
		set<int>::const_iterator it;
		for (it = current_state.begin(); it != current_state.end(); it++) {
			for (vector<int>::const_iterator it2 = fsa.accept_states.begin(); it2 != fsa.accept_states.end(); it2++) {
				if (*it == *it2) return true;
			}
		}
		return false;
	}

	for (int i = 0; i < str.length(); i++) {
		// If string is not null
		vector<FSAdfaElement>::const_iterator it;
		for (it = fsa.elem.begin(); it != fsa.elem.end(); it++) {
			if (it->state == current_state && it->term == str[i]) {
				current_state = it->next_states;
				break;
			}
		}
		if (it == fsa.elem.end()) return false;
	}
	set<int>::const_iterator it;
	for (it = current_state.begin(); it != current_state.end(); it++) {
		// Check last state is accept state
		for (vector<int>::const_iterator it2 = fsa.accept_states.begin(); it2 != fsa.accept_states.end(); it2++) {
			if (*it == *it2) return true;
		}
	}
	return false;
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
	const std::vector<int>& accept_states,
	FiniteStateAutomaton* fsa) {
	// Implement this function.

	// If cannot reach to accept states then return false

	vector<FSATableElement> split_elements;

	// Set accept_states of fsa
	fsa->accept_states.assign(accept_states.begin(), accept_states.end());

	// If FSATableElement has more than one char in str split it
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i].str.length() <= 1) {
			split_elements.push_back(elements[i]);
		}
		else {
			for (int j = 0; j < elements[i].str.length(); j++) {
				FSATableElement temp;
				temp.state = elements[i].state;
				temp.next_state = elements[i].next_state;
				temp.str.push_back(elements[i].str[j]);
				split_elements.push_back(temp);
			}
		}
	}

	NFAtoDFA(*fsa, split_elements);
	//print_dfa(*fsa);

	LOG << "num_elements: " << elements.size()
		<< ", accept_states: " << accept_states.size() << endl;
	return true;
}

void print_dfa(FiniteStateAutomaton& fsa) {
	cout << "dfa : " << endl;
	for (vector<FSAdfaElement>::iterator it = fsa.elem.begin(); it != fsa.elem.end(); it++) {
		print_set(it->state);
		print_set(it->next_states);
		cout << it->term << endl;
	}
}

void print_set(set<int> printing) {
	cout << "{ ";
	for (set<int>::iterator it = printing.begin(); it != printing.end(); it++)
		cout << *it << " ";
	cout << "}" << endl;
}
