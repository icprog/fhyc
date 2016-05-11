#ifndef MY_OCI_H
#define MY_OCI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include <oci.h>
using namespace std;

#define MAXITER 1


class CMyOCCI
{
private:
    char SDate[50],SType[50];
    OCIEnv *m_iEnvhp;
    OCIError *m_iErrhp;
    OCISvcCtx *m_iSvchp;

    OCIServer *m_iSrvhp;
    OCISession *m_iAuthp;
    //OCIBind *bndhp[MAXBINDS];

    bool m_bLoggedOn;

    char *m_pReturnData;

    static int MAXCOLUMNLEN;



    text *m_cUserName;
    text *m_cPassword;
    text *m_cSid;
public:

    CMyOCCI(text *UserName, text *Password, text *Sid);

    ~CMyOCCI();

    sword exec_sql(char *sqlstmt);

    long long exec_select(char *sql_string, int column_num, const char *desc);

    void report_error(OCIError *errhp);




private:

    sword InitDatabase();

    void checkerr(OCIError *errhp, sword status);

    sword log_on(OCISession *authp, OCIError *errhp, OCISvcCtx *svchp,
                 text *uid,text *pwd, ub4 credt, ub4 mode);

    sword attach_server(ub4 mode, OCIServer *srvhp,
                                OCIError *errhp, OCISvcCtx *svchp, text *Sid);


    sword init_handles(OCIEnv **envhp, OCISvcCtx **svchp, OCIError **errhp,
                       OCIServer **srvhp, OCISession **authp, ub4 init_mode);

    void logout_detach_server(OCISvcCtx *svchp, OCIServer *srvhp, OCIError *errhp,
                              OCISession *authp, text *userid);

    sword finish_demo(boolean loggedon, OCIEnv *envhp, OCISvcCtx *svchp,
                      OCIServer *srvhp, OCIError *errhp,
                      OCISession *authp,  text *userid);

    void free_handles(OCIEnv *envhp, OCISvcCtx *svchp, OCIServer *srvhp,
                      OCIError *errhp, OCISession *authp, OCIStmt *stmthp);


};












#endif // MY_OCI_H
