1.db.iniΪ���ݿ����������ļ���ֻ���޸�usr��pwd��dbname�ֶ�(����֮ǰ���ú����ݿ��������֣����ֱ�Ϊ�û�������������ݿ���������
2.log.txtΪԤ��������־�ļ���
3.���Ŀ¼��/home/weblogic/exebin
���г���/home/weblogic/exebin/main.sh
4.�õ������ݱ�
(1)forcast
GUID,VARCHAR2(36)
SID,VARCHAR2(50)
SDATE,VARCHAR2(50)
MWH,NUMBER
MAX_MW,NUMBER


(2)datetype
SDATE,VARCHAR2(50)
STYPE,VARCHAR2(50)

(3)hisdata
create or replace view hisdata as
select MARKETCODE as SID,to_char(TAB_DATE,'yyyy-MM-dd') as SDATE,UnifyQty as MWH,MAXDEMAND as MAX_MW
 from BA_DPOWER_ELEC;