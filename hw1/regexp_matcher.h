// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_
#include "fsa.h"

enum TokenType {
  REGEXP,
  CHAR,
  ANY,
  SET,
  PAREN,
  STAR_T,
};

struct RegExp {
  TokenType token_type;
  char character;
  std::vector< std::vector<RegExp*> > elem;
  RegExp* parent;
  int count_or;
};

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
  FiniteStateAutomaton* fsa;
  RegExp* expr;
};

const char ANYCHAR = '.';
const char LPAREN = '(';
const char RPAREN = ')';
const char LSET = '[';
const char RSET = ']';
const char STAR = '*';
const char OR = '|';

void printRegExp (RegExp* expr);
void print_elem (std::vector<FSATableElement>& elem);

int makeGraph (RegExp& reg_expr, std::vector<FSATableElement>& fsa_elem, std::set<char> alphabet, int start_state, int next_state);

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

