#include <iostream>
#include <string>

#include "exparser.h"

using namespace std;

int main()
{
  // string exp = "30 + 35 * 30";
  string exp = "30 + 30";
  vector<string> tokens{};
  NodePtr result = make_unique<Node>();
  try {
    tokens = get_tokens(exp);
    result = parse(tokens);
  } catch (string s) {
    cerr << s << '\n';
    return 1;
  }
  // for (auto t : tokens) cout << t << '\n';
  cout << result->pprint() << '\n';
  
  return 0;
}