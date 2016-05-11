#include <iostream>
#include<list>
#include "occi.h"
#include "inifile.h"
#include "LoadForNoTem.h"
#include"MyDate.h"

using namespace oracle::occi;
using namespace std;
#define BUFF    1024

int main(int argc, char *argv[])
{


    //1.define variebles and get the main arguements.
    string usr,pwd,dbname;
    string Start_tmp,End_tmp;
    string sid;
    string HistoryTable,ResultTable;
    char sql_temp[512];

    if(5 == argc)
    {
        Start_tmp = string(argv[1]).substr(0,7)+"-01";
        char tmp[20];
        sprintf(tmp,"%s-%d",string(argv[1]).substr(0,7).c_str(),CMyDate::CalMonthDays(argv[1]));
        End_tmp = string(tmp);
        sid = string(argv[2]);
        HistoryTable =  string(argv[3]);
        ResultTable = string(argv[4]);
        cout<<"start time:"<<Start_tmp<<",end time:"<<End_tmp<<",sid:"<<sid<<
                ",history table:"<<HistoryTable<<",result table:"<<ResultTable<<endl;
    }
    else
    {
        cout<<"Please input correct parameter like:"<<endl;
        cout<<argv[0]<<" YYYY-MM sid history_table result_table"<<endl;
        exit(1);
    }







    //2.read the database authentication
    CIniFile iniFile("./db.ini");
    usr = iniFile.GetStrValue("DB","usr");
    pwd = iniFile.GetStrValue("DB","pwd");
    dbname = iniFile.GetStrValue("DB","dbname");
    cout<<"usr:"<<usr<<" "<<"pwd:"<<pwd<<" "<<"dbname:"<<dbname<<endl;

    Environment *env;
    Connection *conn;
    Statement *stmt;

    env = Environment::createEnvironment (Environment::DEFAULT);
    conn = env->createConnection (usr, pwd, dbname);

    CLoadForNoTem LoadForNoTem(Start_tmp,End_tmp);




    //3.get datetype
    cout<<"get LDFuture,start date:"<<LoadForNoTem.m_iFutureStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iFutureEnd.GetStrDate().c_str()<<endl;

    map<CMyDate,string> TypeMap;
    string sqlStmt;
    ResultSet *rset;



    sqlStmt = "select SDATE,STYPE from DATETYPE A where "
            "A.SDATE>= :x "
            " and A.SDATE<= :y";

    stmt = conn->createStatement (sqlStmt);
    try{
        stmt->setString(1,LoadForNoTem.m_iPastStart.GetStrDate());
        stmt->setString(2,LoadForNoTem.m_iFutureEnd.GetStrDate());
        rset = stmt->executeQuery ();

    while (rset->next ())
    {
        string temp2 = rset->getString(2);
        string temp1 = rset->getString(1);
        TypeMap[CMyDate(temp1)]= temp2;
    }
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayAllRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
    }


    map<CMyDate,string>::iterator iter1 = TypeMap.begin();
    map<CMyDate,string>::iterator iterEnd1 = TypeMap.end();
    while(iter1!=iterEnd1)
    {

        cout<<iter1->first.GetStrDate()<<", "<<iter1->second.c_str()<<endl;
        iter1++;
    }
    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);

    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsFuture());
    cout<<endl<<endl<<endl;


    //4.get LDToyear data
    cout<<"get LDToyear,start date:"<<LoadForNoTem.m_iToyearStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iToyearEnd.GetStrDate().c_str()<<endl;
    sprintf(sql_temp,"select  A.SDATE, "
           "MWH, "
          " MAX_MW "
    "from  "
            "%s A "
   " where  "
        " A.SDATE>= :x  "
       " and A.SDATE<= :y "
       " and A.SID = :z",HistoryTable.c_str());

    sqlStmt = string(sql_temp);
    cout<<"sql:"<<sql_temp<<endl;
    stmt = conn->createStatement (sqlStmt);

    try{
        stmt->setString(1,LoadForNoTem.m_iToyearStart.GetStrDate());
        stmt->setString(2,LoadForNoTem.m_iToyearEnd.GetStrDate());
        stmt->setString(3,sid);
        rset = stmt->executeQuery ();

    while (rset->next ())
    {
        string str1 = rset->getString(1);
        double str2 = rset->getDouble(2);
        double str3 = rset->getDouble(3);
        map<CMyDate,string>::iterator iter = TypeMap.find(CMyDate(str1));
        if(iter != TypeMap.end())
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsToyear,str1,
                                str2,str3,iter->second);
            cout<<"set A DateType:"<<iter->first.GetStrDate()<<", "<<iter->second<<endl;
        }
        else
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsToyear,rset->getString(1),
                                    rset->getDouble(2),rset->getDouble(3),"");
        }
    }
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayAllRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
    }

    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);

    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsToyear());
    cout<<endl<<endl<<endl;



    //5.get LDPast data
    cout<<"get LDPast,start date:"<<LoadForNoTem.m_iPastStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iPastEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_temp,"select A.SDATE, "
           "MWH, "
          " MAX_MW "
    "from  "
            "%s A "
   " where  "
        " A.SDATE>= :x  "
       " and A.SDATE<= :y "
       " and A.SID = :z",HistoryTable.c_str());

    sqlStmt = string(sql_temp);
    stmt = conn->createStatement (sqlStmt);
    try{
        stmt->setString(1,LoadForNoTem.m_iPastStart.GetStrDate());
        stmt->setString(2,LoadForNoTem.m_iPastEnd.GetStrDate());
        stmt->setString(3,sid);
        rset = stmt->executeQuery ();

    while (rset->next ())
    {
        map<CMyDate,string>::iterator iter = TypeMap.find(CMyDate(rset->getString(1)));
        if(iter != TypeMap.end())
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsPast,rset->getString(1),
                               rset->getDouble(2),rset->getDouble(3),iter->second);
            cout<<"set A DateType:"<<iter->first.GetStrDate()<<", "<<iter->second<<endl;
        }
        else
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsPast,rset->getString(1),
                                    rset->getDouble(2),rset->getDouble(3),"");
        }
    }
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayAllRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
    }

    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);

    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsPast());
    cout<<endl<<endl<<endl;





    //6.get LDComFuture data
    cout<<"get LDComFuture,start date:"<<LoadForNoTem.m_iComFutureStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iComFutureEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_temp,"select A.SDATE, "
            "MWH, "
           " MAX_MW "
     "from  "
             "%s A "
    " where  "
         "  A.SDATE>= :x "
        " and A.SDATE<= :y "
        " and A.SID = :z ",HistoryTable.c_str());

    sqlStmt = string(sql_temp);

    stmt = conn->createStatement (sqlStmt);
    try{
        stmt->setString(1,LoadForNoTem.m_iComFutureStart.GetStrDate().c_str());
        stmt->setString(2,LoadForNoTem.m_iComFutureEnd.GetStrDate().c_str());
        stmt->setString(3,sid);
        rset = stmt->executeQuery ();

    while (rset->next ())
    {
        map<CMyDate,string>::iterator iter = TypeMap.find(CMyDate(rset->getString(1)));
        if(iter != TypeMap.end())
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsComFuture,rset->getString(1),
                                      rset->getDouble(2),rset->getDouble(3),iter->second);
            cout<<"set A DateType:"<<iter->first.GetStrDate()<<", "<<iter->second<<endl;
        }
        else
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsComFuture,rset->getString(1),
                                    rset->getDouble(2),rset->getDouble(3),"");
        }
    }
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayAllRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
    }

    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);

    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsComFuture());
    cout<<endl<<endl<<endl;


    //7.calculate Average acsent rate
    LoadForNoTem.CalAscRate();
    cout<<"PowerRate:"<<LoadForNoTem.GetPowerRate()<<"  "
       <<"EnergyRate:"<<LoadForNoTem.GetEnergyRate()<<endl;


    //8.set the Date Type of Future Days


    map<CMyDate,CLoadDay>::iterator IterTemp = LoadForNoTem.m_lLDsFuture.begin();
    map<CMyDate,CLoadDay>::iterator IterTempEnd = LoadForNoTem.m_lLDsFuture.end();

    for(;IterTemp!=IterTempEnd;IterTemp++)
    {
        map<CMyDate,string>::iterator IterType = TypeMap.find(IterTemp->first);
        if(IterType != TypeMap.end())
            IterTemp->second.SetType(IterType->second);
        else
        {
//            cout<<"error in LoadForNoTem.SetFutureDay Type,file:"<<__FILE__<<",line"<<__LINE__<<endl;
//            exit(1);
        }
    }

    IterTemp = LoadForNoTem.m_lLDsComFuture.begin();
    IterTempEnd = LoadForNoTem.m_lLDsComFuture.end();

    for(;IterTemp!=IterTempEnd;IterTemp++)
    {
        map<CMyDate,string>::iterator IterType = TypeMap.find(IterTemp->first);
        if(IterType != TypeMap.end())
            IterTemp->second.SetType(IterType->second);
        else
        {
//            cout<<"error in LoadForNoTem.SetFutureDay Type,file:"<<__FILE__<<",line"<<__LINE__<<endl;
//            exit(1);
        }
    }


    IterTemp = LoadForNoTem.m_lLDsPast.begin();
    IterTempEnd = LoadForNoTem.m_lLDsPast.end();

    for(;IterTemp!=IterTempEnd;IterTemp++)
    {
        map<CMyDate,string>::iterator IterType = TypeMap.find(IterTemp->first);
        if(IterType != TypeMap.end())
            IterTemp->second.SetType(IterType->second);
        else
        {
//            cout<<"error in LoadForNoTem.SetFutureDay Type,file:"<<__FILE__<<",line"<<__LINE__<<endl;
//            exit(1);
        }
    }

    IterTemp = LoadForNoTem.m_lLDsToyear.begin();
    IterTempEnd = LoadForNoTem.m_lLDsToyear.end();

    for(;IterTemp!=IterTempEnd;IterTemp++)
    {
        map<CMyDate,string>::iterator IterType = TypeMap.find(IterTemp->first);
        if(IterType != TypeMap.end())
            IterTemp->second.SetType(IterType->second);
        else
        {
//            cout<<"error in LoadForNoTem.SetFutureDay Type,file:"<<__FILE__<<",line"<<__LINE__<<endl;
//            exit(1);
        }
    }
    //9.calculate forcast


    //9.1 delete datas which we want to forcast
    sprintf(sql_temp,"delete FROM %s where SDATE>='%s' and SDATE<='%s' and sid = '%s'",ResultTable.c_str(),
            LoadForNoTem.m_iFutureStart.GetStrDate().c_str(),LoadForNoTem.m_iFutureEnd.GetStrDate().c_str(),
            sid.c_str());
    cout<<"sql:"<<sql_temp<<endl;

    sqlStmt = sql_temp;
    stmt = conn->createStatement (sqlStmt);
    try{
//        stmt->setString(1,LoadForNoTem.m_iFutureStart.GetStrDate());
//        stmt->setString(2,LoadForNoTem.m_iFutureEnd.GetStrDate());

        cout<<"start:"<<LoadForNoTem.m_iFutureStart.GetStrDate()<<" end:"
           <<LoadForNoTem.m_iFutureEnd.GetStrDate()<<endl;


        stmt->executeUpdate();

    }catch(SQLException ex)
    {
     cout<<"Exception thrown for delete history data"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
    }


    conn->terminateStatement (stmt);

    //9.2 calculate forcast data
    map<CMyDate,CLoadDay>::iterator Iter = LoadForNoTem.m_lLDsFuture.begin();
    map<CMyDate,CLoadDay>::iterator End = LoadForNoTem.m_lLDsFuture.end();
    CMyDate TmpDate;


    for(;Iter != End;Iter++)
    {

        TmpDate = LoadForNoTem.CalPreciseComDay(Iter->first);
        double Energy = LoadForNoTem.m_lLDsComFuture[TmpDate].GetEnergy()*LoadForNoTem.m_fEnergyRate;
        double MaxPower = LoadForNoTem.m_lLDsComFuture[TmpDate].GetMaxPower()*LoadForNoTem.m_fPowerRate;
        cout<<"Date:"<<Iter->first.GetStrDate().c_str()<<",Energy:"<<Energy<<",MaxPower:"<<MaxPower<<endl<<endl;
        Iter->second.SetEnergy(Energy);
        Iter->second.SetMaxPower(MaxPower);

        sqlStmt = "insert into FORCAST (SDATE,SID,MWH,MAX_MW) values "
                "( :x, :y, :z, :w)";

        stmt = conn->createStatement (sqlStmt);
        try{
            stmt->setString(1,Iter->first.GetStrDate());
            stmt->setString(2,sid);
            stmt->setDouble(3,Iter->second.GetEnergy());
            stmt->setDouble(4,Iter->second.GetMaxPower());
            stmt->executeUpdate();

        }catch(SQLException ex)
        {
         cout<<"Exception thrown for insert data"<<endl;
         cout<<"Error number: "<<  ex.getErrorCode() << endl;
         cout<<ex.getMessage() << endl;
        }

    }

    cout<<endl<<"after done"<<endl;
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsFuture());
    cout<<endl<<endl<<endl;

    env->terminateConnection (conn);
    Environment::terminateEnvironment (env);

    return 0;
}

