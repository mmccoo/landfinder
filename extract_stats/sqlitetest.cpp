#include <stdio.h>
#include <iostream>
#include <sqlite3.h> 
#include <SQLiteCpp/SQLiteCpp.h>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
  const char* dbfile = "../Cowlitz/parcels.db";

  // Open a database file in readonly mode
  SQLite::Database    dbobj(dbfile);  // SQLITE_OPEN_READONLY
  std::cout << "SQLite database file '" << dbobj.getFilename().c_str() << "' opened successfully\n";

  // Test if the 'test' table exists
  bool bExists = dbobj.tableExists("Parcels");
  std::cout << "SQLite table 'test' exists=" << bExists << "\n";

  SQLite::Statement   query(dbobj, "SELECT PARCNO, ACCOUNTNO, Coordinates FROM Parcels limit 2");

  const boost::regex e("(\\-?\\d+(\\.\\d+)?),(\\-?\\d+(\\.\\d+)?)\\s*");

  // Loop to execute the query step by step, to get rows of result
  while (query.executeStep()) {
    std::cout << 
      query.getColumn(0).getText() << " " << 
      query.getColumn(1).getText() << " " << 
      query.getColumn(2).getText() << std::endl;

    std::vector<std::string> points;
    std::string s1(query.getColumn(2).getText());
    split(points, s1, boost::is_any_of(" "), boost::token_compress_on ); 
    std::cout << " size " << points.size() << std::endl;
  }
  return 0;
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("../Cowlitz/parcels.db", &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   const char* sql = "SELECT * from Parcels limit 5";
   const char* data = "Callback function called";
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
}
