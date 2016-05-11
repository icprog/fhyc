1.db.ini为数据库连接配置文件，只需修改usr、pwd和dbname字段(运行之前配置好数据库连接名字），分别为用户名、密码和数据库连接名。
2.log.txt为预测程序的日志文件。
3.存放目录：/home/weblogic/exebin
运行程序：/home/weblogic/exebin/main.sh
4.用到的数据表：
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