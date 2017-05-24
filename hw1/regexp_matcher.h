// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_
#include "fsa.h"

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
  FiniteStateAutomaton fsa;
  //RegExp* expr;
};

const char ANYCHAR = '.';
const char LPAREN = '(';
const char RPAREN = ')';
const char LSET = '[';
const char RSET = ']';
const char STAR = '*';
const char OR = '|';

void print_elem (std::vector<FSATableElement>& elem);

bool makeGraph (const char* regexp, std::vector<FSATableElement>& elements, std::set<int>& state, const std::vector<char> alphabet, int& cursor, int first_state, int final_state);

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

