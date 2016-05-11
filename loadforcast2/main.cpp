#include <iostream>
#include<list>
#include "dmdb.h"
#include "inifile.h"
#include "LoadForNoTem.h"
#include"MyDate.h"


using namespace std;
#define BUFF    1024
int ComDayNum = 30;
int main(int argc, char *argv[])
{

    if(argc != 3)
    {
        cout<<"Please input correct parameter like:"<<endl
           <<"    "<<argv[0]<<" StartTime "<<"EndTime "<<endl;
        exit(1);
    }


    int i,ColumnNum;
    string usr,pwd,ip;
    char sql_str[BUFF];



    CIniFile iniFile("./db.ini");
    usr = iniFile.GetStrValue("DB","usr");
    pwd = iniFile.GetStrValue("DB","pwd");
    ip = iniFile.GetStrValue("DB","ip");
    cout<<"usr:"<<usr<<" "<<"pwd:"<<pwd<<" "<<"ip:"<<ip<<endl;



    CDmDb dmdb(usr.c_str(),pwd.c_str(),ip.c_str(),"./log.txt");
    dmdb.init_database();



    CLoadForNoTem LoadForNoTem(argv[1],argv[2]);
    //get LDToyear
    cout<<"get LDToyear,start date:"<<LoadForNoTem.m_iToyearStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iToyearEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_str,"select     A.srq, "
            "sdl, "
           " smax,"
           " stype  "
     "from  "
             "lnjndb.psidp.sjz A,lnjndb.psidp.datetype B "
    " where  "
         " A.srq=B.srq "
         " and   A.srq>='%s'  "
        " and A.srq<='%s' "
        " and A.sid = 'syllsyp_fhkh'",LoadForNoTem.m_iToyearStart.GetStrDate().c_str(),
            LoadForNoTem.m_iToyearEnd.GetStrDate().c_str());



    ColumnNum = 4;
    dmdb.exec_select(sql_str,ColumnNum,"Get ToyearLD");
    for(i=0;i<dmdb.get_row_num();i++)
    {
        LoadForNoTem.AddLDToYear(dmdb.get_select_data(i,0),
                                 atof(dmdb.get_select_data(i,1).c_str()),
                                 atof(dmdb.get_select_data(i,2).c_str()),
                                 dmdb.get_select_data(i,3));
    }
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsToyear());
    cout<<endl<<endl<<endl;


    //get LDPast
    cout<<"get LDPast,start date:"<<LoadForNoTem.m_iPastStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iPastEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_str,"select     A.srq, "
            "sdl, "
           " smax,"
           " stype  "
     "from  "
             "lnjndb.psidp.sjz A,lnjndb.psidp.datetype B "
    " where  "
         " A.srq=B.srq "
         " and   A.srq>='%s'  "
        " and A.srq<='%s' "
        " and A.sid = 'syllsyp_fhkh'",LoadForNoTem.m_iPastStart.GetStrDate().c_str(),
            LoadForNoTem.m_iPastEnd.GetStrDate().c_str());

    ColumnNum = 4;
    dmdb.exec_select(sql_str,ColumnNum,"get LDPast");
    for(i=0;i<dmdb.get_row_num();i++)
    {
        LoadForNoTem.AddLDPast(dmdb.get_select_data(i,0),
                                 atof(dmdb.get_select_data(i,1).c_str()),
                                 atof(dmdb.get_select_data(i,2).c_str()),
                                 dmdb.get_select_data(i,3));
    }
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsPast());
    cout<<endl<<endl<<endl;


    //get LDComFuture
    cout<<"get LDComFuture,start date:"<<LoadForNoTem.m_iComFutureStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iComFutureEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_str,"select     A.srq, "
            "sdl, "
           " smax,"
           " stype  "
     "from  "
             "lnjndb.psidp.sjz A,lnjndb.psidp.datetype B "
    " where  "
         " A.srq=B.srq "
         " and   A.srq>='%d-01-01'  "
        " and A.srq<='%d-12-31' "
        " and A.sid = 'syllsyp_fhkh'",LoadForNoTem.m_iComFutureStart.GetIntYear(),
            LoadForNoTem.m_iComFutureEnd.GetIntYear());

    ColumnNum = 4;
    dmdb.exec_select(sql_str,ColumnNum,"get LDComFuture");
    for(i=0;i<dmdb.get_row_num();i++)
    {
        LoadForNoTem.AddLDToComFuture(dmdb.get_select_data(i,0),
                                 atof(dmdb.get_select_data(i,1).c_str()),
                                 atof(dmdb.get_select_data(i,2).c_str()),
                                 dmdb.get_select_data(i,3));
    }
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsComFuture());
    cout<<endl<<endl<<endl;



    //calculate Average acsent rate
    LoadForNoTem.CalAscRate();
    cout<<"PowerRate:"<<LoadForNoTem.GetPowerRate()<<"  "
       <<"EnergyRate:"<<LoadForNoTem.GetEnergyRate()<<endl;


    //get LDFuture
    cout<<"get LDComFuture,start date:"<<LoadForNoTem.m_iComFutureStart.GetStrDate().c_str()
       <<"end date:"<<LoadForNoTem.m_iComFutureEnd.GetStrDate().c_str()<<endl;

    sprintf(sql_str,"select * from lnjndb.psidp.datetype A where "
            "A.srq>='%s'  "
            " and A.srq<='%s' " ,LoadForNoTem.m_iFutureStart.GetStrDate().c_str(),
            LoadForNoTem.m_iFutureEnd.GetStrDate().c_str());

    ColumnNum = 4;
    dmdb.exec_select(sql_str,ColumnNum,"get LDComFuture");
    for(i=0;i<dmdb.get_row_num();i++)
    {
        LoadForNoTem.AddLDToFuture(dmdb.get_select_data(i,0),
                                 0,
                                0,
                                 dmdb.get_select_data(i,1));
    }
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsFuture());
    cout<<endl<<endl<<endl;


    map<CMyDate,CLoadDay>::iterator Iter = LoadForNoTem.m_lLDsFuture.begin();
    map<CMyDate,CLoadDay>::iterator End = LoadForNoTem.m_lLDsFuture.end();
    CMyDate TmpDate;

    sprintf(sql_str,"delete lnjndb.psidp.ycz where rq>='%s' and rq<='%s'",
            LoadForNoTem.m_iFutureStart.GetStrDate().c_str(),
            LoadForNoTem.m_iFutureEnd.GetStrDate().c_str());
    dmdb.exec_sql(sql_str);


    for(;Iter != End;Iter++)
    {
        TmpDate = LoadForNoTem.CalPreciseComDay(Iter->first);
        double Energy = LoadForNoTem.m_lLDsComFuture[TmpDate].GetEnergy()*LoadForNoTem.m_fEnergyRate;
        double MaxPower = LoadForNoTem.m_lLDsComFuture[TmpDate].GetMaxPower()*LoadForNoTem.m_fPowerRate;
        cout<<"Date:"<<Iter->first.GetStrDate().c_str()<<",Energy:"<<Energy<<",MaxPower:"<<MaxPower<<endl<<endl;
        Iter->second.SetEnergy(Energy);
        Iter->second.SetMaxPower(MaxPower);
        sprintf(sql_str,"insert into lnjndb.psidp.ycz (rq,sid,dl,smax)values"
                "('%s','syllsyp_fhkh','%f','%f')",Iter->first.GetStrDate().c_str(),
                Iter->second.GetEnergy(),Iter->second.GetMaxPower());
        dmdb.exec_sql(sql_str);
    }


    //calculate forcast




    cout<<endl<<"after done"<<endl;
    LoadForNoTem.PrintLDs(LoadForNoTem.GetLDsFuture());
    cout<<endl<<endl<<endl;

    return 0;
}

