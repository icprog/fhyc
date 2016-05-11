#ifndef LOADFOR_H
#define LOADFOR_H
#include<string>
#include <iostream>
#include<list>
#include<map>
#include"LoadDay.h"
#include "LoadForNoTem.h"
#include"MyDate.h"
using std::string;
using std::list;
using std::pair;
using std::map;
using std::cout;
using std::endl;

extern int ComDayNum;
class CLoadForNoTem
{
  public:
    map<CMyDate,CLoadDay> m_lLDsFuture,m_lLDsComFuture;
    map<CMyDate,CLoadDay> m_lLDsPast,m_lLDsToyear;
    CLoadDay m_iLoadDay;
    list<double> m_fPowerAscRate,m_fEnergyAscRate;

    static double m_fPowerRate;
    static double m_fEnergyRate;


public:

    CMyDate m_iFutureStart,m_iFutureEnd,m_iComFutureStart,m_iComFutureEnd;
    CMyDate m_iPastStart,m_iPastEnd,m_iToyearStart,m_iToyearEnd;

    CLoadForNoTem(){}

    CLoadForNoTem(string Start,string End);

    map<CMyDate,CLoadDay> GetLDsFuture(){return m_lLDsFuture;}
    map<CMyDate,CLoadDay> GetLDsComFuture(){return m_lLDsComFuture;}
    map<CMyDate,CLoadDay> GetLDsPast(){return m_lLDsPast;}
    map<CMyDate,CLoadDay> GetLDsToyear(){return m_lLDsToyear;}

    static double GetPowerRate(){return m_fPowerRate;}
    static double GetEnergyRate(){return m_fEnergyRate;}

    void CalAscRate();
    void CalFutureLoad();

    static CMyDate CalComDay(CMyDate Date);



    void AddThirdType(string &Date,string &Type)
    {
        CMyDate tmp(Date);
        if(7 == CMyDate::CalWeekDay(tmp))
        {
            Type=Type+"7";
        }
        else if(6 == CMyDate::CalWeekDay(tmp))
        {
            Type=Type+"6";
        }
        else
            Type=Type+"0";
    }
    void AddLDToComFuture(string Date,double MaxPower,double Energy,
                          string Type)
    {
        AddThirdType(Date,Type);
        m_lLDsComFuture.insert(pair<CMyDate,CLoadDay>(CMyDate(Date),CLoadDay(Date,MaxPower,Energy,Type)));
    }

    void AddLDToFuture(string Date,double MaxPower,double Energy,
                       string Type)
    {
        AddThirdType(Date,Type);
        m_lLDsFuture.insert(pair<CMyDate,CLoadDay>(CMyDate(Date),CLoadDay(Date,MaxPower,Energy,Type)));
    }

    void AddLDPast(string Date,double MaxPower,double Energy,
                   string Type)
    {
        AddThirdType(Date,Type);
        m_lLDsPast.insert(pair<CMyDate,CLoadDay>(CMyDate(Date),CLoadDay(Date,MaxPower,Energy,Type)));
    }

    void AddLDToYear(string Date,double MaxPower,double Energy,
                     string Type)
    {
        AddThirdType(Date,Type);
        m_lLDsToyear.insert(pair<CMyDate,CLoadDay>(CMyDate(Date),CLoadDay(Date,MaxPower,Energy,Type)));
    }

    void PrintLDs(map<CMyDate,CLoadDay> LoadDays);

    CMyDate CalPreciseComDay(CMyDate);
};

#endif // LOADFOR_H
