#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<list>
#include"LoadForNoTem.h"
using std::string;
using std::cout;
using std::endl;
using std::list;
double CLoadForNoTem::m_fPowerRate = 0.0;
double CLoadForNoTem::m_fEnergyRate = 0.0;



CLoadForNoTem::CLoadForNoTem(string Start,string End)
{
    CMyDate s(Start),e(End);

    int DayNum = CMyDate::CalDifference(s,e);

    m_iFutureStart = s;
    m_iFutureEnd = e;

    m_iToyearEnd = CMyDate::AddDayNum(m_iFutureStart, -1);
    m_iToyearStart = CMyDate::AddDayNum(m_iToyearEnd, -ComDayNum);

    m_iPastStart = CalComDay(m_iToyearStart);
    m_iPastEnd = CMyDate::AddDayNum(m_iPastStart, ComDayNum);

    m_iComFutureStart = CalComDay(m_iFutureStart);
    m_iComFutureEnd = CMyDate::AddDayNum(m_iComFutureStart, DayNum);

}

CMyDate CLoadForNoTem::CalComDay(CMyDate Date)
{
    int WeekSeq,WeekDay;
    CMyDate ret;

    WeekDay = CMyDate::CalWeekDay(Date);
    WeekSeq = CMyDate::CalWeekSeq(Date);
    ret = CMyDate::CalDateFromWeekDay(Date.GetIntYear()-1,WeekSeq,WeekDay);
    return ret;
}
void CLoadForNoTem::PrintLDs(map<CMyDate,CLoadDay> LoadDays)
{
    map<CMyDate,CLoadDay>::iterator iter,End;
    iter = LoadDays.begin();
    End = LoadDays.end();

    for(;iter!=End;iter++)
    {
        cout<<"Date:"<<iter->first.GetStrDate().c_str()<<"  "
           <<"MaxPower:"<<(*iter).second.GetMaxPower()<<"  "
          <<"Energy:"<<(*iter).second.GetEnergy()<<"  "
            <<"Type:"<<(*iter).second.GetType()<<endl;
    }
}

void CLoadForNoTem::CalAscRate()
{
    int Num = 0;
    double EnergyRateSum = 0,PowerRateSum = 0;
    map<CMyDate,CLoadDay>::iterator PastIter = m_lLDsPast.begin();
    map<CMyDate,CLoadDay>::iterator PastIterEnd = m_lLDsPast.end();
    map<CMyDate,CLoadDay>::iterator ToyearIter = m_lLDsToyear.begin();
    map<CMyDate,CLoadDay>::iterator ToyearIterEnd = m_lLDsToyear.end();

    if(m_lLDsPast.size()!=m_lLDsToyear.size())
    {
        cout<<"CalPowerRate Error because of size"<<endl;
        exit(1);
    }
    for(; PastIter!=PastIterEnd; PastIter++,ToyearIter++)
    {

        if(PastIter->second.GetType().substr(0,2) == "00" && ToyearIter->second.GetType().substr(0,2) == "00")
        {
            Num++;
            PowerRateSum += ToyearIter->second.GetMaxPower()/(PastIter->second.GetMaxPower());
            EnergyRateSum += ToyearIter->second.GetEnergy()/(PastIter->second.GetEnergy());
        }
    }
    CLoadForNoTem::m_fPowerRate = PowerRateSum/Num;
    CLoadForNoTem::m_fEnergyRate = EnergyRateSum/Num;

}
////计算三个时间段
//void CLoadForNoTem::CalDates()
//{

//}
CMyDate CLoadForNoTem::CalPreciseComDay(CMyDate Date)
{
    CMyDate Tmp,ComDate;

    ComDate = CLoadForNoTem::CalComDay(Date);


    if(0 != m_lLDsFuture[Date].GetType().substr(0,2).compare("00"))
    {
        if(0 == m_lLDsFuture[Date].GetType().substr(0,2).compare("ZZ"))
        {

            ComDate = ComDate + 1;
            while(0 != m_lLDsComFuture[ComDate].GetType().compare("000"))
               ComDate = ComDate + 1;
        }
        else
        {
            string Type = m_lLDsFuture[Date].GetType();
            map<CMyDate,CLoadDay>::iterator Inner = m_lLDsComFuture.begin();
            map<CMyDate,CLoadDay>::iterator InnerEnd = m_lLDsComFuture.end();
            while(Inner != InnerEnd)
            {
                if(Inner->second.GetType().substr(0,2) == Type.substr(0,2))
                    break;
                Inner++;
            }
            ComDate = Inner->first;

        }
    }
    else
    {

        while(0 != m_lLDsComFuture[ComDate].GetType().substr(0,2).compare("00"))
        {
           ComDate = ComDate + 7;
        }

    }


    cout<<"in CalPreciseComDay"<<endl<<"primary Date:"<<Date.GetStrDate().c_str()<<",Com Date:"
          <<ComDate.GetStrDate().c_str()<<endl;
    cout<<"WeekSeq:"<<CMyDate::CalWeekSeq(Date)<<",WeekDay:"<<CMyDate::CalWeekDay(Date)<<",to"<<endl;
    cout<<"WeekSeq:"<<CMyDate::CalWeekSeq(ComDate)<<",WeekDay:"<<CMyDate::CalWeekDay(ComDate)<<",to"<<endl;

    return ComDate;
}
void CLoadForNoTem::CalFutureLoad()
{




}
