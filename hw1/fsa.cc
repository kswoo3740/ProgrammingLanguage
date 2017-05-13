// PL homework: hw1
// fsa.cc

#include <iostream>
#include <queue>
#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

void NFAtoDFA (FiniteStateAutomaton& fsa, std::vector<FSATableElement> elements) {
  queue<set<int> > states_queue;
  vector<FSAdfaElement> states;
  FSAdfaElement dfa_state;
  set<int> state;
  set<char> alphabet;

  state.insert(1);
  epsilon_state (elements, state, 1);
  //cout<<"start state : ";
  //print_set(state);
  fsa.start_state = state;
  states_queue.push(state);

 
  
  //cout<< "elements: " <<endl;
  for (vector<FSATableElement>::iterator it = elements.begin(); it != elements.end(); it++) {
    alphabet.insert(it->str[0]);
    //cout<<it->str[0]<<endl;
  }
  
  
  //cout<<"alphabt: ";
  for (set<char>::iterator it = alphabet.begin(); it != alphabet.end(); it++) {
    //cout<<*it<<" ";
  }
  //cout<<endl;

  while (!states_queue.empty()) {
    state = states_queue.front();
    for (set<char>::const_iterator it = alphabet.begin(); it != alphabet.end(); it++) {
      //cout<<"chul ryuk : "<<*it<<endl;
      if (*it != '\0') {
        //cout<<"string : "<<*it<<"   state :";
        //print_set(state);
        //cout<<"go to"<<endl;
        dfa_state.next_states = possible_state(elements, *it, state);
        dfa_state.state = state;
        dfa_state.term = *it;

        //print_set(dfa_state.next_states);
        if (dfa_state.next_states.empty()) continue;

        vector<FSAdfaElement>::iterator it2;
        for (it2 = fsa.elem.begin(); it2 != fsa.elem.end(); it2++) {
          if (it2->state == dfa_state.state && it2->next_states == dfa_state.next_states && it2->term  == dfa_state.term) break;
        }

        if (it2 == fsa.elem.end()) {
          fsa.elem.push_back(dfa_state);
          states_queue.push(state);
          state = dfa_state.next_states;
          states_queue.push(state);
        }
      }
      //cout<<"chul end"<<endl;
    }
    //cout<<"dequeue"<<endl;
    states_queue.pop();
  }
}

set<int> possible_state (const vector<FSATableElement> elements, const char term, set<int> start_states) {
  set<int> possible_states;
  set<int> mid_states;

  //cout<< "term : "<<term<<endl;
  //cout<< "hi"<<endl;
  for (set<int>::iterator it = start_states.begin(); it != start_states.end(); it++) {
    epsilon_state (elements, mid_states, *it);
  }
  //cout<<"bye"<<endl;
//  cout<<"possible state string : "<< term <<endl;
//  print_set(mid_states);

  for (set<int>::iterator it = start_states.begin(); it != start_states.end(); it++) {
    for (vector<FSATableElement>::const_iterator it = elements.begin(); it != elements.end(); it++) {
      set<int> temp;
      epsilon_state(elements, temp, it->state);
      //cout<<"str : ";
      //cout<<it->str[0]<<endl;
      if (it->str[0] == '\0') continue;
      if (mid_states.find(it->state) != mid_states.end()) {
        if (term == it->str[0]) {
          if (mid_states != temp) {
            //cout<<"in temp"<<endl;
            temp = possible_state(elements, term, temp);
            //print_set(temp);
            possible_states.insert(temp.begin(), temp.end());
            //print_set(possible_states);
          }
      //cout<<"temp_state "<<it->state<<endl;
            //print_set(temp);
          possible_states.insert(it->next_state);

        }
      }
    }
  }
  
  return possible_states;
}

void epsilon_state (const vector<FSATableElement> elements, set<int>& mid_states, int state) {
  mid_states.insert(state);
  for (vector<FSATableElement>::const_iterator it = elements.begin(); it != elements.end(); it++) {
    if (it->str.length() == 0 && it->state == state && mid_states.find(it->next_state) == mid_states.end()) {
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

  //cout<<"run state"<<endl;
  for (int i = 0; i < str.length(); i++) {
    // If string is not null
    vector<FSAdfaElement>::const_iterator it;
    for (it = fsa.elem.begin(); it != fsa.elem.end(); it++) {
      if (it->state == current_state && it->term == str[i]) {
        //print_set(current_state);
        current_state = it->next_states;
        break;
      }
    }
    if (it == fsa.elem.end()) return false;
  }
  //print_set(current_state);
  set<int>::const_iterator it;
  for (it = current_state.begin(); it != current_state.end(); it++) {
    for (vector<int>::const_iterator it2 = fsa.accept_states.begin(); it2 != fsa.accept_states.end(); it2++) {
      if (*it == *it2) return true;
    }
  }
  return false;
}

/*
bool check_build (const std::vector<FSATableElement>& elements, const std::vector<int>& accept_states) {
  // Check this nfa can reach to accept state 
  queue<int> state_queue;
  vector<int> states;

  for (int i = 0; i < accept_states.size(); i++) {
    cout<< accept_states[i]<<endl;
  }
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

  return buildable;
}
*/


bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function.
  // bool buildable = check_build (elements, accept_states);

  // If cannot reach to accept states then return false
  // if (!buildable) return false;

  vector<FSATableElement> split_elements;

  // Set accept_states of fsa
  fsa->accept_states.assign(accept_states.begin(), accept_states.end());

  // If FSATableElement has more than one char in str split it
  for (int i = 0; i < elements.size(); i++) {
    if (elements[i].str.length() <= 1) {
      split_elements.push_back(elements[i]);
    } else {
      for (int j = 0; j < elements[i].str.length(); j++) {
        FSATableElement temp;
        temp.state = elements[i].state;
        temp.next_state = elements[i].next_state;
        temp.str[0] = elements[i].str[j];
        split_elements.push_back(temp);
      }
    }
  }

  /*
  cout<<"accept_state : ";
  for (int i = 0; i < fsa->accept_states.size(); i++) {
    cout<<fsa->accept_states[i]<<" ";
  }
  cout<<endl;;
  cout<<elements.size()<<endl;
  for (int i = 0; i < elements.size(); i++) {
    cout<<elements[i].str<<endl;
  }
  */
  NFAtoDFA(*fsa, split_elements);
  print_dfa (*fsa);

  LOG << "num_elements: " << elements.size()
      << ", accept_states: " << accept_states.size() << endl;
  return true;
}

void print_dfa (FiniteStateAutomaton& fsa) {
  cout<<"dfa : "<<endl;
  for (vector<FSAdfaElement>::iterator it = fsa.elem.begin(); it != fsa.elem.end(); it++) {
    print_set(it->state);
    print_set(it->next_states);
    cout<<it->term<<endl;
  }
}
    
void print_set (set<int> printing) {
  cout<< "{ ";
  for (set<int>::iterator it = printing.begin(); it != printing.end(); it++) 
    cout<<*it<<" ";
  cout<<"}"<<endl;
}
