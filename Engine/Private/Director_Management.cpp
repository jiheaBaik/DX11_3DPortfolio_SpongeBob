
#include "..\Public\Director_Management.h"

IMPLEMENT_SINGLETON(CDirector_Management)


CDirector_Management::CDirector_Management()
{
	int rc;
	char *err_msg = 0;

	rc = sqlite3_open("../../Reference/DataBase/SpongeBob_jhBaik_DB.db", &db);
	if (rc)
		sqlite3_close(db);


	if (rc != SQLITE_OK)
	{
		MSG_BOX("SQL errory");
		sqlite3_free(err_msg);
		sqlite3_close(db);

		return;
	}






}


void CDirector_Management::ManageMent()
{

	int rc;
	sqlite3_stmt *res3;

	string sqlTemp = "select * from Level_Boss1_Map";
	const char* sql = sqlTemp.c_str();


	rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

	while (sqlite3_step(res3) == SQLITE_ROW)
	{

		char			temp[MAX_PATH] = "";
		strcpy_s(temp, (char*)sqlite3_column_text(res3, 2));
		if (strcmp(temp, "Mesh_camera") == 0)
		{
			objInfo.iIndex = sqlite3_column_int(res3, 5);
			objInfo.iNumber = sqlite3_column_int(res3, 6);
			objInfo.fPos.x = (_float)sqlite3_column_double(res3, 7);
			objInfo.fPos.y = (_float)sqlite3_column_double(res3, 8);
			objInfo.fPos.z = (_float)sqlite3_column_double(res3, 9);
			objInfo.fScale.x = (_float)sqlite3_column_double(res3, 10);
			objInfo.fScale.y = (_float)sqlite3_column_double(res3, 11);
			objInfo.fScale.z = (_float)sqlite3_column_double(res3, 12);
			objInfo.fAxist.x = (_float)sqlite3_column_int(res3, 21);
			objInfo.fAxist.y = (_float)sqlite3_column_int(res3, 22);
			objInfo.fAxist.z = (_float)sqlite3_column_int(res3, 23);
			objInfo.fRotation = (_float)sqlite3_column_double(res3, 24);

			Direcotor_cameradesc.m_vFirstDirecotorCamera.push_back(objInfo);
		}
	}

}

void CDirector_Management::Free()
{
	
}
