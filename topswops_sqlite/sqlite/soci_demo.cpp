#include "/usr/include/soci/soci.h"
#include "/usr/include/soci/soci-sqlite3.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>
#include<algorithm>

using namespace soci;
using namespace std;

string int_to_str(int n) {
  if (n == 0) return "0";
  string s;
  while (n > 0) {
    int last_digit = n % 10;
    s += string(1, last_digit + '0');
    n = (n - last_digit) / 10;
  }
  reverse(s.begin(), s.end());
  return s;
}

string init_perm(int n) {
  if (n <= 0) return "";
  string s = "1";
  for(int i = 2; i <= n; ++i) 
    s += ", " + int_to_str(i);
  return s;
}

int main() {
  try {
    const string db_name("dbtest.db");
    session sql(sqlite3, db_name);

    cout << "Creating topswops table if necessary ..." << endl;
    sql << "CREATE TABLE IF NOT EXISTS topswops(n INTEGER PRIMARY KEY, score INTEGER, perm TEXT, date TEXT)";

    cout << "Inserting some test values ..." << endl;
    // perform the insertions in a single transaction (for speed)
    sql.begin();
    for(int i = 2; i <= 97; ++i) {
      string p = init_perm(i);
      cout << p << endl;
      sql << "INSERT OR REPLACE INTO topswops(n, score, perm, date) VALUES(" 
          << i << ", " << i*i << ", '" << p << "', date('now'))";
    }
    sql.commit();

    cout << "Selecting and displaying contents of topswops table ..." << endl;
    for(int i = 2; i <= 97; ++i) {
      int score = -1;
      sql << "SELECT score FROM topswops WHERE n = " << i, into(score);
      string perm;
      sql << "SELECT perm FROM topswops WHERE n = " << i, into(perm);
      string date;
      sql << "SELECT date FROM topswops WHERE n = " << i, into(date);
      cout << "(" << date << ") n = " << i << ", score = " << score 
           << ", perm = " << perm << endl;
    }

    int sum = 0;
    sql << "SELECT sum(score) FROM topswops", into(sum);
    cout << "Total sum = " << sum << endl;

    //    vector<int> ns, scores;
    //    sql << "select n from topswops", into(ns);
    //sql << "select scores from topswops", into(scores);


//     row r;
//     sql << "select * from topswops", into(r);
//     cout << r.size() << " rows retrieved" << endl;
//     for(int i = 0; i < r.size(); ++i) {
//       const column_properties& props = r.get_properties(i);
//       cout << "properties: " << props.get_name() << endl;
//       int n = r.get<int>(i);
//       int score = r.get<int>(i);
//       cout << "n = " << n << ", score = " << score << endl;
//     }
// 
  } catch (exception const &e) {
    cerr << "Error: " << e.what() << '\n';
  }
}
