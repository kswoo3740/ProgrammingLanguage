// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"
#include <iostream>

using namespace std;


void printRegExp (RegExp* expr) {
  if (expr->token_type == CHAR) {
    cout<<expr->character;
  } else if (expr->token_type == ANY) {
    cout<<ANYCHAR;
  } else if (expr->token_type == STAR_T) {
    printRegExp (expr->elem[0][0]);
    cout<<STAR;
  } else {
    if (expr->token_type == PAREN) {
      cout<<LPAREN;
    } else if (expr->token_type == SET) {
      cout<<LSET;
    }

    for (int i = 0; i <= expr->count_or; i++) {
      if (i != 0) cout<<OR;
      for (int j = 0; j < expr->elem[i].size(); j++) {
        printRegExp (expr->elem[i][j]);
      }
    }

    if (expr->token_type == PAREN) {
      cout<<RPAREN;
    } else if (expr->token_type == SET) {
      cout<<RSET;
    }
  }
}
   
int makeGraph (RegExp& reg_expr, vector<FSATableElement>& fsa_elem, set<char> alphabet, int start_state, int next_state) {
  cout<<"make"<<endl;
  int current_state = next_state;

  if (reg_expr.token_type == REGEXP || reg_expr.token_type == PAREN) {
    for (int i = 0; i <= reg_expr.count_or; i++) {
      //current_state++;
      FSATableElement start_elem;
      start_elem.state = start_state;
      start_elem.next_state = current_state;
      start_elem.str = "\0";
      fsa_elem.push_back(start_elem);

      for (int j = 0; j < reg_expr.elem[i].size(); j++) {
        if (reg_expr.elem[i][j]->token_type == CHAR) {
          // Token is character
          cout<<"Character token"<<endl;
          FSATableElement elem;
          elem.state = current_state;
          elem.str[0] = reg_expr.elem[i][j]->character;
          elem.next_state = ++current_state;
          fsa_elem.push_back(elem);
        } else if (reg_expr.elem[i][j]->token_type == ANY) {
          // Token is '.'
          cout<<", token"<<endl;
          for (set<char>::iterator it = alphabet.begin(); it != alphabet.end(); it++) {
            FSATableElement elem;
            elem.state = current_state;
            elem.next_state = ++current_state;
            elem.str[0] = *it;
            fsa_elem.push_back(elem);
            elem.state = current_state;
            elem.next_state = ++current_state;
            elem.str = "\0";
            fsa_elem.push_back(elem);
          }
        } else if (reg_expr.elem[i][j]->token_type == PAREN) {
          // Token is '('
          cout<<"lparen token"<<endl;
          int temp = current_state;
          FSATableElement elem;
          elem.state = temp;
          elem.next_state = ++temp;
          elem.str = "\0";
          fsa_elem.push_back(elem);
          current_state = makeGraph(*reg_expr.elem[i][j], fsa_elem, alphabet, temp, temp + 1);
          elem.state = ++temp;
          elem.next_state = ++current_state;
          elem.str = "\0";
        } else if (reg_expr.elem[i][j]->token_type == SET) {
          // Token is '['
          cout<<"lset token"<<endl;
          for (int k = 0; k < reg_expr.elem[i][j]->elem[0].size(); k++) {
            FSATableElement elem;
            elem.state = current_state;
            elem.next_state = ++current_state;
            elem.str[0] = reg_expr.elem[i][j]->elem[0][k]->character;
            fsa_elem.push_back(elem);
            elem.state = current_state;
            elem.next_state = ++current_state;
            elem.str = "\0";
            fsa_elem.push_back(elem);
          }
        } else if (reg_expr.elem[i][j]->token_type == STAR_T) {
          // Token is '*'
          cout<<"Star token"<<endl;
          int temp = current_state;
          FSATableElement elem;
          elem.state = temp;
          elem.next_state = ++temp;
          elem.str = "\0";
          fsa_elem.push_back(elem);
          current_state = makeGraph(*reg_expr.elem[i][j]->elem[0][0], fsa_elem, alphabet, temp, temp + 1);
          elem.state = temp;
          elem.next_state = temp + 1;
          elem.str = "\0";
          fsa_elem.push_back(elem);
          elem.state = temp + 1;
          elem.next_state = temp;
          elem.str = "\0";
          fsa_elem.push_back(elem);
          elem.state = temp + 1;
          elem.next_state = ++current_state;
          elem.str = "\0";
        }
      }

      FSATableElement elem;
      elem.state = current_state;
      elem.next_state = next_state;
      elem.str = "\0";
      fsa_elem.push_back(elem);
    }
  } else if (reg_expr.token_type == CHAR) {
    cout<<"character token"<<endl;
    FSATableElement elem;
    elem.state = start_state;
    elem.next_state = next_state;
    elem.str[0] = reg_expr.character;
    fsa_elem.push_back(elem);
    current_state = next_state;
  } else if (reg_expr.token_type == ANY) {
    cout<<". token"<<endl;
    for (set<char>::iterator it = alphabet.begin(); it != alphabet.end(); it++) {
      FSATableElement elem;
      elem.state = start_state;
      elem.next_state = next_state;
      elem.str[0] = *it;
      fsa_elem.push_back(elem);
    }
    current_state = next_state;
  } else if ( reg_expr.token_type == SET) {
    cout<<"set token"<<endl;
    for (int k = 0; k < reg_expr.elem[0].size(); k++) {
      FSATableElement elem;
      elem.state = start_state;
      elem.next_state = next_state;
      elem.str[0] = reg_expr.elem[0][k]->character;
      fsa_elem.push_back(elem);
    }
    current_state = next_state;
  }
  return current_state;
}


          
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
  RegExp start_expr;
  set<char> alphabet;
  char handle;
  int cursor = 0;
  cout<<"BouildRegExp"<<endl;

  start_expr.token_type = REGEXP;
  start_expr.character = '\0';
  start_expr.elem = vector<vector<RegExp*> >(1);
  start_expr.parent = NULL;
  start_expr.count_or = 0;
  
  handle = regexp[cursor];

  while (handle != '\0') {
    if (handle != ANYCHAR && handle != LPAREN && handle != RPAREN && handle != LSET && handle != RSET && handle != STAR && handle != OR)
      alphabet.insert(handle);

    handle = regexp[++cursor];
  }
  cout<<"handle"<<endl;

  for (char alpha = 'a'; alpha <= 'z'; alpha++) alphabet.insert(alpha);
  for (char alpha = 'A'; alpha <= 'Z'; alpha++) alphabet.insert(alpha);
  for (char num = '0'; num <= '9'; num++) alphabet.insert(num);

  cursor = 0;

  RegExp *current_expr = &start_expr;

  handle = regexp[cursor++];

  while (handle != '\0') {
    cout<<"handle in"<<endl;
    if (handle == ANYCHAR) {
      cout<<"handle ."<<endl;
      RegExp *child_expr = new RegExp;
      child_expr->token_type = ANY;
      child_expr->character = '\0';
      child_expr->elem = vector<vector<RegExp*> >(1);
      child_expr->elem[0].resize(0);
      child_expr->parent = NULL;
      child_expr->count_or = 0;
      current_expr->elem[current_expr->count_or].push_back(child_expr);
    } else if (handle == LPAREN) {
      cout<<"handle ("<<endl;
      RegExp* child_expr = new RegExp;
      child_expr->token_type = ANY;
      child_expr->character = '\0';
      child_expr->elem = vector<vector<RegExp*> >(1);
      child_expr->elem[0].resize(0);
      child_expr->parent = current_expr;
      child_expr->count_or = 0;
      current_expr->elem[current_expr->count_or].push_back(child_expr);
      current_expr = child_expr;
    } else if (handle == RPAREN) {
      cout<<"handle )"<<endl;
      if (current_expr->token_type != PAREN) return false;
      current_expr = current_expr->parent;
    } else if (handle == LSET) {
      cout<<"handle ["<<endl;
      RegExp* child_expr = new RegExp;
      child_expr->token_type = ANY;
      child_expr->character = '\0';
      child_expr->elem = vector<vector<RegExp*> >(1);
      child_expr->elem[0].resize(0);
      child_expr->parent = current_expr;
      child_expr->count_or = 0;
      current_expr->elem[current_expr->count_or].push_back(child_expr);
      current_expr = child_expr;
    } else if (handle == RSET) {
      cout<<"handle ]"<<endl;
      if (current_expr->token_type != SET) return false;
      current_expr = current_expr->parent;
    } else if (handle == STAR) {
      cout<<"handle *"<<endl;
      if (current_expr->elem[0].size() == 0) return false;

      RegExp* child_expr = new RegExp;
      child_expr->token_type = STAR_T;
      child_expr->character = '\0';
      child_expr->elem = vector<vector<RegExp*> >(1);
      child_expr->elem[0].resize(0);
      child_expr->parent = NULL;
      child_expr->count_or = 0;
      child_expr->elem[0].push_back(current_expr->elem[current_expr->count_or].back());
      current_expr->elem[current_expr->count_or].pop_back();

      current_expr->elem[current_expr->count_or].push_back(child_expr);
    } else if (handle == OR) {
      cout<<"handle |"<<endl;
      current_expr->count_or++;
      vector<RegExp*> next;
      current_expr->elem.push_back(next);
    } else {
      cout<<"handle character"<<endl;
      RegExp* child_expr = new RegExp;
      child_expr->token_type = CHAR;
      child_expr->character = handle;
      child_expr->elem = vector<vector<RegExp*> >(1);
      child_expr->elem[0].resize(0);
      child_expr->parent = NULL;
      child_expr->count_or = 0;
      current_expr->elem[current_expr->count_or].push_back(child_expr);
    }
    handle = regexp[cursor++];
  }

  vector<FSATableElement> fsa_elem;
  set<int> accept_states;
  int size;
  accept_states.insert(1);

  makeGraph(start_expr, fsa_elem, alphabet, 1, 2);

  cout<<"goto make"<<fsa_elem.size()<<endl;
  // accept state 찾는 함수 작성

  vector<int> accept_state = vector<int>(accept_states.begin(), accept_states.end());
  cout<<"accept : "<<accept_state.size()<<endl;
  regexp_matcher->fsa = new FiniteStateAutomaton();
  cout<< "before FSA"<<endl;
  print_elem(fsa_elem);
  
  BuildFSA (fsa_elem, accept_state, regexp_matcher->fsa);

  return true;
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
  return RunFSA(*regexp_matcher.fsa, str);
}

void print_elem (vector<FSATableElement>& elem) {
  for (int i = 0; i < elem.size(); i++) {
    cout<<"state : "<<elem[i].state<<endl;
    cout<<"next_state : "<<elem[i].next_state<<endl;
    cout<<"str : "<<elem[i].str<<endl;
  }
}
