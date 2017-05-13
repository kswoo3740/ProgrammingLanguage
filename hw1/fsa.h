// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>
#include <set>

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
  int state;
  int next_state;
  std::string str;
};


struct FSAdfaElement {
  std::set<int> state;
  std::set<int> next_states;
  char term;
};


struct FiniteStateAutomaton {
  // Make your own FSA struct here.
  std::set<int> start_state;
  std::vector<int> accept_states;
  std::vector<FSAdfaElement> elem;
};

void NFAtoDFA (FiniteStateAutomaton& fsa, std::vector<FSATableElement> elements);
std::set<int> possible_state (const std::vector<FSATableElement> elements, const char term, std::set<int> start_state);
void epsilon_state (const std::vector<FSATableElement> elements, std::set<int>& mid_states, int state);
bool check_build (const std::vector<FSATableElement>& elements, const std::vector<int>& accept_states);
void print_dfa (FiniteStateAutomaton& fsa);
void print_set (std::set<int> printing);

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

#endif //_PL_HOMEWORK_FSA_H_

