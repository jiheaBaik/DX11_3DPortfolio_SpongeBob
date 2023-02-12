#include "..\Public\DatabaseManager.h"

IMPLEMENT_SINGLETON(CDatabaseManager)

CDatabaseManager::CDatabaseManager()
{
}

static int select_callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void execute_sql(sqlite3 *db, string zSql, sqlite3_callback xCallback, void *pArg) {

	int  rc;
	char *pzErrMsg = 0;
	rc = sqlite3_exec(db, zSql.c_str(), xCallback, pArg, &pzErrMsg);
	if (rc != SQLITE_OK) {
		MSG_BOX("SQL error");
		sqlite3_free(pzErrMsg);
	}
}

void connect(sqlite3 **db) {
	int  rc;
	rc = sqlite3_open("test.db", db);
	if (rc) {
		MSG_BOX("Can't open database");
		exit(0);
	}
	else {
			MSG_BOX("Opened database successfully");
	}
}

void CDatabaseManager::Free()
{
}
