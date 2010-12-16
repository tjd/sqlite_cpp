#include "/usr/include/soci/soci.h"
#include "/usr/include/soci/soci-sqlite3.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>

using namespace soci;
using namespace std;

int main() {
  try {
    cout << "This a test of soci" << endl;
    session sql(sqlite3, "service=mydb user=john password=secret");

  } catch (exception const &e) {
    cerr << "Error: " << e.what() << '\n';
  }
}
