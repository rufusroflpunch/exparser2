#include <iostream>
#include <string>

#include "exparser.h"

using namespace std;

int main()
{
  string exp = "30 + 35 * 30";
  vector<string> tokens{};
  try {
    tokens = get_tokens(exp);
  } catch (string s) {
    cout << s;
    return 1;
  }
  for (auto t : tokens) cout << t << '\n';
  
  return 0;
}