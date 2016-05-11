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



CLoadForNoTem::CLoadForNoTem(string Start,string End):PastDaySpan(60),ComDayNum(60)
{
    CMyDate s(Start),e(End);

    int DayNum = CMyDate::CalDifference(s,e);

    m_iFutureStart = s;
    m_iFutureEnd = e;

    m_iToyearEnd = CMyDate::AddDayNum(m_iFutureStart, -1);
    m_iToyearStart = CMyDate::AddDayNum(m_iToyearEnd, -ComDayNum);

    m_iPastStart = CalComDay(m_iToyearStart) ;
    m_iPastEnd = CMyDate::AddDayNum(m_iPastStart, ComDayNum);

    m_iComFutureStart = CalComDay(m_iFutureStart)-PastDaySpan;
    m_iComFutureEnd = CMyDate::AddDayNum(m_iComFutureStart, DayNum + 2*PastDaySpan);
    InitZeroLoad(0,0,string("0000"));

}



//initialize the four map structs of map<CMyDate,CLoadDay>
void CLoadForNoTem::InitZeroLoad(double Energy,double MaxPower,string Type)
{
    CMyDate temp;
    for(temp = m_iFutureStart;temp<=m_iFutureEnd;temp=temp+1)
        AddLDToFuture(temp.GetStrDate(),Energy,MaxPower,Type);

    for(temp = m_iComFutureStart;temp<=m_iComFutureEnd;temp=temp+1)
        AddLDToComFuture(temp.GetStrDate(),Energy,MaxPower,Type);

    for(temp = m_iToyearStart;temp <= m_iToyearEnd;temp=temp+1)
        AddLDToYear(temp.GetStrDate(),Energy,MaxPower,Type);

    for(temp = m_iPastStart;temp<=m_iPastEnd;temp=temp+1)
        AddLDPast(temp.GetStrDate(),Energy,MaxPower,Type);
}

//calculate the corresponding day in past year, the WeekSeqs are the same.
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
    int MaxPowerNum = 0, EnergyNum = 0;
    double EnergyRateSum = 0,PowerRateSum = 0;
    map<CMyDate,CLoadDay>::iterator PastIter = m_lLDsPast.begin();
    map<CMyDate,CLoadDay>::iterator PastIterEnd = m_lLDsPast.end();
    map<CMyDate,CLoadDay>::iterator ToyearIter = m_lLDsToyear.begin();
    map<CMyDate,CLoadDay>::iterator ToyearIterEnd = m_lLDsToyear.end();

    if(m_lLDsPast.size()!=m_lLDsToyear.size())
    {
        cout<<"past:"<<m_lLDsPast.size()<<",Toyear:"<<m_lLDsToyear.size()<<endl;
        cout<<"CalPowerRate Error because of size"<<endl;
        exit(1);
    }
    for(; PastIter!=PastIterEnd; PastIter++,ToyearIter++)
    {

        if(PastIter->second.GetType().substr(0,2) == "00" && ToyearIter->second.GetType().substr(0,2) == "00")
        {            
            if(0 != ToyearIter->second.GetMaxPower()&& 0 != PastIter->second.GetMaxPower())
            {
                MaxPowerNum++;
                PowerRateSum += ToyearIter->second.GetMaxPower()/(PastIter->second.GetMaxPower());
            }


            if(0 != ToyearIter->second.GetEnergy() && 0 != PastIter->second.GetEnergy())
            {
                EnergyNum++;
                EnergyRateSum += ToyearIter->second.GetEnergy()/(PastIter->second.GetEnergy());
            }
        }
    }
    if(0 != MaxPowerNum)
        CLoadForNoTem::m_fPowerRate = PowerRateSum/MaxPowerNum;
    else//all MaxPower is zero.
        CLoadForNoTem::m_fPowerRate = 1;

    if(0 != EnergyNum)
        CLoadForNoTem::m_fEnergyRate = EnergyRateSum/EnergyNum;
    else//The Energy of every day is zero.
        CLoadForNoTem::m_fEnergyRate = 1;

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
            while(0 != m_lLDsComFuture[ComDate].GetType().substr(0,5).compare("00000"))
               ComDate = ComDate + 1;
        }
        else
        {
            string Type = m_lLDsFuture[Date].GetType();
            map<CMyDate,CLoadDay>::iterator Inner = m_lLDsComFuture.begin();
            map<CMyDate,CLoadDay>::iterator InnerEnd = m_lLDsComFuture.end();
            cout<<"LDsComFuture start:"<<Inner->first.GetStrDate()
               <<"LDsComFuture end:"<<m_iComFutureEnd.GetStrDate()<<endl;

            while(Inner != InnerEnd)
            {
                //cout<<"searching date:"<<Inner->first.GetStrDate()<<",type:"<<Inner->second.GetType()<<endl;
                if((Inner->second.GetType().substr(0,2) == Type.substr(0,2))||
                        (Inner->second.GetType().substr(2,2) == Type.substr(0,2)))
                    break;                
                Inner++;
            }
            if(Inner == InnerEnd)
            {
                cout<<"encounter error while find holiday,the date is:"<<Date.GetStrDate()<<",type is:"
                   <<Type.c_str()<<endl;
                exit(1);
            }
            ComDate = Inner->first;

        }
    }
    else
    {
//        int num = 0;
        map<CMyDate,CLoadDay>::iterator Iter_temp = m_lLDsComFuture.find(ComDate);
        while( (Iter_temp != m_lLDsComFuture.end())
               && (0 != Iter_temp->second.GetType().substr(0,2).compare("00")))
        {
            ComDate = ComDate + 7;
            Iter_temp = m_lLDsComFuture.find(ComDate);
        }
//        while(0 != Iter_temp->second.GetType().substr(0,2).compare("00"))
//        {
//            num++;
//            if(num > 7)exit(1);
//            cout<<"Date:"<<Date.GetStrDate()<<",comDate:"<<ComDate.GetStrDate()<<endl;
//            cout<<"Date type:"<<m_lLDsFuture[Date].GetType().c_str()<<
//                  ",Comday type:"<<m_lLDsComFuture[ComDate].GetType().c_str()<<endl;
//            PrintLDs(m_lLDsComFuture);
//           ComDate = ComDate + 7;
//        }

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
