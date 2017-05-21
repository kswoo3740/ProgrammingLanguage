// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <vector>
#include <stack>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {
  for (vector<LRTableElement>::const_iterator it = elements.begin(); it != elements.end(); it++) {
    if (it->action == GOTO) {
      lr_parser->goto_table.push_back(*it);
    } else {
      lr_parser->action_table.push_back(*it);
    }
  }

  lr_parser->rules = rules;

  return true;
}

bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
  stack<int> state_stack;
  stack<int> input;
  int action;

  state_stack.push(0);
  
  for (int i = 1; i <= str.length(); i++) {
    input.push(str[str.length() - i]);
  }

  while (true) {
    vector<LRTableElement>::const_iterator it;
    for (it = lr_parser.action_table.begin(); it != lr_parser.action_table.end(); it++) {
      if (it->state == state_stack.top() && it->symbol == input.top()) break;
    }
    //cout<<"state : "<<state_stack.top()<<"    symbol : "<<input.top()<<endl;

    if (it == lr_parser.action_table.end()) return false;

    action = it->action;

    switch (action) {
      case 1:
      {
        //cout<<"Shift"<<endl;
        state_stack.push(input.top());
        //cout<<"str : "<<input.top()<<endl;
        input.pop();
        state_stack.push(it->next_state);
        //cout<<"state : "<<it->next_state<<endl;
        break;
      }

      case 2:
      {
        //cout<<"reduce"<<endl;
        for (int i = 0; i < lr_parser.rules[it->next_state - 1].num_rhs; i++) {
          state_stack.pop();
          state_stack.pop();
          //cout<<"reduced!"<<endl;
        }

        vector<LRTableElement>::const_iterator it_goto;
        
        //cout<<"top : "<<state_stack.top()<<"   symbol : "<<lr_parser.rules[it->next_state - 1].lhs_symbol<<endl;
        for (it_goto = lr_parser.goto_table.begin(); it_goto != lr_parser.goto_table.end(); it_goto++) {
          if (it_goto->state == state_stack.top() && it_goto->symbol == lr_parser.rules[it->next_state - 1].lhs_symbol) break;
        }

        state_stack.push(lr_parser.rules[it->next_state - 1].lhs_symbol);
        if (it_goto != lr_parser.goto_table.end()) {
          state_stack.push(it_goto->next_state);
        } else return false;
        break;
      }

      case 3:
        return true;
    }
  }
  return false;
}

