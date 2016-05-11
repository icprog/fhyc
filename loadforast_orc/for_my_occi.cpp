
#include <iostream>
#include<list>
#include "occi.h"
#include "inifile.h"
#include "LoadForNoTem.h"
#include"MyDate.h"
#include"my_occi.h"

using namespace oracle::occi;
using namespace std;
#define BUFF    512

int main(int argc, char *argv[])
{


    //1.define variebles and get the main arguements.
    string usr,pwd,dbname;
    string Start_tmp,End_tmp;
    string sid;
    string HistoryTable,ResultTable;
    char sql_temp[512];
    long RowNum = 0,i;

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


    CMyOCCI MyOCCI((text*)usr.c_str(),(text*)pwd.c_str(),(text*)dbname.c_str());


    CLoadForNoTem LoadForNoTem(Start_tmp,End_tmp);




    //3.get datetype
    cout<<"get LDFuture,start date:"<<LoadForNoTem.m_iFutureStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iFutureEnd.GetStrDate().c_str()<<endl;

    map<CMyDate,string> TypeMap;
    string sqlStmt;
    ResultSet *rset;



    sprintf(sql_temp,"select SDATE,STYPE from DATETYPE A "
            "where A.SDATE>= '%s'  and A.SDATE<= '%s'",
            LoadForNoTem.m_iPastStart.GetStrDate().c_str(),
            LoadForNoTem.m_iFutureEnd.GetStrDate().c_str());

    cout<<sql_temp<<endl;

    RowNum = MyOCCI.exec_select(sql_temp,2,"get date type from database");

    for(i=0; i<RowNum; i++)
    {
        TypeMap[CMyDate(MyOCCI.GetStr(0,i))]= MyOCCI.GetStr(1,i);
    }

//    map<CMyDate,string>::iterator iter = TypeMap.begin();
//    map<CMyDate,string>::iterator end = TypeMap.end();
//    for(; iter != end;iter++)
//    {
//        cout<<iter->first.GetStrDate()<<", "<<iter->second.c_str()<<endl;
//    }


    //4.set the Date Type of Future Days


    map<CMyDate,CLoadDay>::iterator IterTemp = LoadForNoTem.m_lLDsFuture.begin();
    map<CMyDate,CLoadDay>::iterator IterTempEnd = LoadForNoTem.m_lLDsFuture.end();

    for(;IterTemp!=IterTempEnd;IterTemp++)
    {
        map<CMyDate,string>::iterator IterType = TypeMap.find(IterTemp->first);
        if(IterType != TypeMap.end())
        {
            IterTemp->second.SetType(IterType->second);
            IterTemp->second.AddLastType();
        }
        else
        {
//            cout<<"error in LoadForNoTem.SetFutureDay Type,file:"<<__FILE__<<",line"<<__LINE__<<endl;
//            exit(1);
        }
    }

    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsFuture());
    cout<<endl<<endl<<endl;

    IterTemp = LoadForNoTem.m_lLDsComFuture.begin();
    IterTempEnd = LoadForNoTem.m_lLDsComFuture.end();

    for(;IterTemp!=IterTempEnd;IterTemp++)
    {
        map<CMyDate,string>::iterator IterType = TypeMap.find(IterTemp->first);
        if(IterType != TypeMap.end())
        {
            IterTemp->second.SetType(IterType->second);
            IterTemp->second.AddLastType();
        }
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
        {
            IterTemp->second.SetType(IterType->second);
            IterTemp->second.AddLastType();
        }
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
        {
            IterTemp->second.SetType(IterType->second);
            IterTemp->second.AddLastType();
        }
        else
        {
//            cout<<"error in LoadForNoTem.SetFutureDay Type,file:"<<__FILE__<<",line"<<__LINE__<<endl;
//            exit(1);
        }
    }



    //5.get LDToyear data
    cout<<"get LDToyear,start date:"<<LoadForNoTem.m_iToyearStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iToyearEnd.GetStrDate().c_str()<<endl;
    sprintf(sql_temp,"select  SDATE, "
           "MWH, "
          " MAX_MW "
    "from  "
            "%s "
   " where  "
            " SDATE>= '%s'  "
            " and SDATE<= '%s' "
       " and SID = '%s'",
            HistoryTable.c_str(),
            LoadForNoTem.m_iToyearStart.GetStrDate().c_str(),
            LoadForNoTem.m_iToyearEnd.GetStrDate().c_str(),
            sid.c_str());


    cout<<"sql:"<<sql_temp<<endl;


    RowNum = MyOCCI.exec_select(sql_temp,3,"get LDToyear data");

    for(i=0; i<RowNum; i++)
    {
        string str1 = MyOCCI.GetStr(0,i);
        double str2 = MyOCCI.GetDouble(1,i);
        double str3 = MyOCCI.GetDouble(2,i);

        map<CMyDate,string>::iterator iter = TypeMap.find(CMyDate(str1));
        if(iter != TypeMap.end())
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsToyear,str1,
                                str2,str3,iter->second);
            cout<<"set A DateType:"<<iter->first.GetStrDate()<<", "<<iter->second<<endl;
        }
        else
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsToyear,str1,
                                    str2,str3,"");
        }
    }

    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsToyear());
    cout<<endl<<endl<<endl;



    //6.get LDPast data
    cout<<"get LDPast,start date:"<<LoadForNoTem.m_iPastStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iPastEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_temp,"select SDATE, "
           "MWH, "
          " MAX_MW "
    "from  "
            "%s  "
   " where  "
            " SDATE>= '%s'  "
       " and SDATE<= '%s' "
       " and SID = '%s'",
            HistoryTable.c_str(),
            LoadForNoTem.m_iPastStart.GetStrDate().c_str(),
            LoadForNoTem.m_iPastEnd.GetStrDate().c_str(),
            sid.c_str());

    RowNum = MyOCCI.exec_select(sql_temp,3,"get LDPast data");

    for(i=0; i<RowNum; i++)
    {
        string str1 = MyOCCI.GetStr(0,i);
        double str2 = MyOCCI.GetDouble(1,i);
        double str3 = MyOCCI.GetDouble(2,i);
        map<CMyDate,string>::iterator iter = TypeMap.find(CMyDate(str1));
        if(iter != TypeMap.end())
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsPast,str1,
                               str2,str3,iter->second);
            cout<<"set A DateType:"<<iter->first.GetStrDate()<<", "<<iter->second<<endl;
        }
        else
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsPast,str1,
                                    str2,str3,"");
        }
    }


    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsPast());
    cout<<endl<<endl<<endl;




    //7.get LDComFuture data
    cout<<"get LDComFuture,start date:"<<LoadForNoTem.m_iComFutureStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iComFutureEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_temp,"select SDATE, "
            "MWH, "
           " MAX_MW "
     "from  "
             "%s "
    " where  "
            "  SDATE>= '%s' "
            " and SDATE<= '%s' "
            " and SID = '%s' ",
            HistoryTable.c_str(),
            LoadForNoTem.m_iComFutureStart.GetStrDate().c_str(),
            LoadForNoTem.m_iComFutureEnd.GetStrDate().c_str(),
            sid.c_str());



    RowNum = MyOCCI.exec_select(sql_temp,3,"get LDComFuture data");
    for(i=0; i<RowNum; i++)
    {
        string str1 = MyOCCI.GetStr(0,i);
        double str2 = MyOCCI.GetDouble(1,i);
        double str3 = MyOCCI.GetDouble(2,i);

        map<CMyDate,string>::iterator iter = TypeMap.find(CMyDate(str1));
        if(iter != TypeMap.end())
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsComFuture,str1,
                                      str2,str3,iter->second);
            cout<<"set A DateType:"<<iter->first.GetStrDate()<<", "<<iter->second<<endl;
        }
        else
        {
            LoadForNoTem.SetLoadDay(LoadForNoTem.m_lLDsComFuture,str1,
                                    str2,str3,"");
        }
    }


    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsComFuture());
    cout<<endl<<endl<<endl;

    //8.calculate Average acsent rate
    LoadForNoTem.CalAscRate();
    cout<<"PowerRate:"<<LoadForNoTem.GetPowerRate()<<"  "
       <<"EnergyRate:"<<LoadForNoTem.GetEnergyRate()<<endl;




    //9.calculate forcast


    //9.1 delete datas which we want to forcast
    sprintf(sql_temp,"delete FROM %s where SDATE>='%s' and SDATE<='%s' and sid = '%s'",ResultTable.c_str(),
            LoadForNoTem.m_iFutureStart.GetStrDate().c_str(),LoadForNoTem.m_iFutureEnd.GetStrDate().c_str(),
            sid.c_str());
    cout<<"sql:"<<sql_temp<<endl;


    cout<<"start:"<<LoadForNoTem.m_iFutureStart.GetStrDate()<<" end:"
       <<LoadForNoTem.m_iFutureEnd.GetStrDate()<<endl;

    RowNum = MyOCCI.exec_sql(sql_temp);

    cout<<RowNum<<" rows have been deleted"<<endl;




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

        sprintf(sql_temp,"insert into FORCAST (SDATE,SID,MWH,MAX_MW) values "
                "( '%s', '%s', '%f', '%f')",
                Iter->first.GetStrDate().c_str(),
                sid.c_str(),
                Iter->second.GetEnergy(),
                Iter->second.GetMaxPower());

        MyOCCI.exec_sql(sql_temp);

    }

    cout<<endl<<"after done"<<endl;
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsFuture());
    cout<<endl<<endl<<endl;


    return 0;
}

