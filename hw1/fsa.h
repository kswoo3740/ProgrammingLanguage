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
  int state;
  std::set<int> next_states;
  char term;
};

struct FiniteStateAutomaton {
  // Make your own FSA struct here.
  std::vector<int> accept_states;
  std::vector<FSAdfaElement> elem;
};

std::vector<int> translate (const FiniteStateAutomaton& fsa, int current_state, char terminal);
std::vector<int> translate (const FiniteStateAutomaton& fsa, std::vector<int> states, char terminal);

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

#endif //_PL_HOMEWORK_FSA_H_

