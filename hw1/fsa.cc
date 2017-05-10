// PL homework: hw1
// fsa.cc

#include <iostream>
#include <queue>
#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

vector<int> translate (const FiniteStateAutomaton& fsa, int current_state, char terminal) {
  vector<int> states;
  vector<int> terminal_stack;

  terminal_stack.push_back(current_state);

  int state;
  while (!terminal_stack.empty()) {
    state = terminal_stack.back();
    terminal_stack.pop_back();
    for (vector<FSAdfaElement>::const_iterator it = fsa.elem.begin(); it != fsa.elem.end(); it++) {
      if (it->state == state) {
        if (it->term == '\0') {
            cout<<"test\n";
          for (vector<FSAdfaElement>::const_iterator it2 = fsa.elem.begin(); it2 != fsa.elem.end(); it2++) 
            for (set<int>::iterator it3 = it2->next_states.begin(); it3 != it2->next_states.end(); it3++)
              terminal_stack.push_back(*it3);
        } else if (it->term == terminal) {
          for (set<int>::iterator it2 = it->next_states.begin(); it2 != it->next_states.end(); it2++) 
            states.push_back(*it2);
        }
      }
    }
  }
    
    return states;
}

vector<int> translate (const FiniteStateAutomaton& fsa, vector<int> states, char terminal) {
  vector<int> new_states;
  vector<int>::iterator it;

  for (it = states.begin(); it != states.end(); it++) {
    vector<int> temp = translate(fsa, *it, terminal);
    new_states.insert(new_states.end(), temp.begin(), temp.end());
  }

  return new_states;
}

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
  // Implement this function.
  vector<int> dfa = translate(fsa, 1, str[0]);

  for (int i = 1; i < str.length(); i++) {
    dfa = translate(fsa, dfa, str[i]);
  }

  for (vector<int>::const_iterator it = fsa.accept_states.begin(); it != fsa.accept_states.end(); it++) {
    for (vector<int>::iterator it2 = dfa.begin(); it2 != dfa.end(); it2++) {
      if (*it == *it2) return true;
    }
  }

  return false;
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function.
  queue<int> state_queue;
  vector<int> states;

  state_queue.push(1);
  
  while (!state_queue.empty()) {
    int state = state_queue.front();
    state_queue.pop();
      
    bool exist = false;
    for (vector<int>::iterator it = states.begin(); it != states.end(); it++) 
      if (*it == state) exist = true; 

    if (!exist) {
      states.push_back(state);

      for (vector<FSATableElement>::const_iterator it = elements.begin(); it != elements.end(); it++) 
        if (it->state == state)
          state_queue.push(it->next_state);
    }
  }

  bool buildable = false;
  for (vector<int>::const_iterator it = accept_states.begin(); it != accept_states.end(); it++) {
    for (vector<int>::iterator it2 = states.begin(); it2 != states.end(); it2++) {
      if (*it == *it2) {
          buildable = true;
          break;
      }
    }
    if (buildable) break;
  }

  if (!buildable) return false;

  // If FSATableElement has more than one char in str seperate it
  for (int i = 0; i < elements.size(); i++) {
    if (elements[i].str.length() <= 1) {
      FSAdfaElement temp;
      temp.state = elements[i].state;
      temp.next_states.insert(elements[i].next_state);
      temp.term = elements[i].str[0];
      fsa->elem.push_back(temp);
    } else {
      for (int j = 0; j < elements[i].str.length(); j++) {
        FSAdfaElement temp;
        temp.state = elements[i].state;
        temp.next_states.insert(elements[i].next_state);
        temp.term = elements[i].str[j];
        fsa->elem.push_back(temp);
      }
    }
  }

  // Set final state of FSA
  fsa->accept_states.assign(accept_states.begin(), accept_states.end());

  LOG << "num_elements: " << elements.size()
      << ", accept_states: " << accept_states.size() << endl;
  return true;
}

