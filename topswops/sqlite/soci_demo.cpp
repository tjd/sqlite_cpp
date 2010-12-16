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
    const string db_name("dbtest.db");
    session sql(sqlite3, db_name);

    cout << "Creating topswops table if necessary ..." << endl;
    sql << "CREATE TABLE IF NOT EXISTS topswops(n INTEGER PRIMARY KEY, score INTEGER)";

    cout << "Inserting some test values ..." << endl;
    // BEGIN ... COMMIT perform the insertions in a single transaction
    // (for speed)
    sql.begin();
    for(int i = 2; i <= 97; ++i) {
      sql << "INSERT OR REPLACE INTO topswops(n, score) VALUES(" << i 
          << ", 0)";
    }
    sql.commit();

    cout << "Selecting and displaying contents of topswops table ..." << endl;
    for(int i = 2; i <= 97; ++i) {
      int score = -1;
      sql << "SELECT score FROM topswops WHERE n = " << i, into(score);
      cout << "n = " << i << ", score = " << score << endl;
    }

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
