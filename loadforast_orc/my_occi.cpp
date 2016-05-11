#include"my_occi.h"
int CMyOCCI::MAXCOLUMNLEN = 1024;

CMyOCCI::CMyOCCI(text *UserName, text *Password, text *Sid):
    m_cUserName(UserName),m_cPassword(Password),m_cSid(Sid)
{
    m_iEnvhp = NULL;
    m_iErrhp = NULL;
    m_iSvchp = NULL;

    m_iSrvhp = NULL;
    m_iAuthp = NULL;

    m_pReturnData = NULL;
    m_bLoggedOn = false;

    m_nColumnNum =0;
    m_nRowNum =0;
    cout<<UserName<<", "<<Password<<", "<<Sid<<endl;


    InitDatabase();
}


double CMyOCCI::GetDouble(long column, long row)
{
    return atof(m_pReturnData+row*MAXCOLUMNLEN*m_nColumnNum+column*MAXCOLUMNLEN);
}

float CMyOCCI::GetFloat(long column, long row)
{
    return GetDouble(column,row);
}

string CMyOCCI::GetStr(long column, long row)
{
    return string(m_pReturnData+row*MAXCOLUMNLEN*m_nColumnNum+column*MAXCOLUMNLEN);
}

long CMyOCCI::GetInt(long column, long row)
{
    return atoi(m_pReturnData+row*MAXCOLUMNLEN*m_nColumnNum+column*MAXCOLUMNLEN);
}

long CMyOCCI::exec_sql(char *sqlstmt)
{
    sword status;
    OCIStmt *stmthp = (OCIStmt *) NULL;
    static OCIDefine *p_dfn    = NULL;
    text* sql_in = (text*)sqlstmt;






    if (OCIHandleAlloc((dvoid *)m_iEnvhp, (dvoid **) &stmthp,
                       (ub4)OCI_HTYPE_STMT, (CONST size_t) 0, (dvoid **) 0))
    {
      (void) printf("FAILED: alloc statement handle\n");
      return finish_demo(m_bLoggedOn, m_iEnvhp, m_iSvchp, m_iSrvhp, m_iErrhp, m_iAuthp,
                         m_cUserName);
    }

    if (OCIStmtPrepare(stmthp, m_iErrhp, sql_in, (ub4)strlen((char *)sql_in),
                      (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT))
    {
      (void) printf("FAILED: OCIStmtPrepare() insert\n");
      report_error(m_iErrhp);
      return -1;
    }

    if(status = OCIStmtExecute(m_iSvchp, stmthp, m_iErrhp, (ub4) MAXITER, (ub4) 0,
                               (CONST OCISnapshot*)0,(OCISnapshot*)0,
                               (ub4) OCI_DEFAULT ))
    {
        (void) printf("FAILED: OCIStmtExecute exec_sql(),%d\n",OCI_ERROR);
        report_error(m_iErrhp);
        printf("status=%d\n",status);
        return -1;
    }

    ub4 row_count;

    if(status = OCIAttrGet ( stmthp, OCI_HTYPE_STMT, &row_count, 0, OCI_ATTR_ROW_COUNT,
            m_iErrhp ))
    {
        (void) printf("FAILED:OCIAttrGet exec_sql(),%d\n",OCI_ERROR);
        report_error(m_iErrhp);
        printf("status=%d\n",status);
        return -1;
    }

    printf("%d rows have been affected!\n",row_count);




    if(stmthp)
    (void) OCIHandleFree((dvoid *) stmthp, (ub4) OCI_HTYPE_STMT);
    stmthp = NULL;

  return row_count;
}



long long CMyOCCI::exec_select(char *sql_string, int column_num, const char *desc)
{

    int i;
    sword status;
    char *p;
    int step = 50;
    int RowCount = 0;
    int StepCount = 1;
    OCIStmt *stmthp = (OCIStmt *) NULL;
    static OCIDefine *p_dfn    = NULL;
    m_nRowNum =0;
    m_nColumnNum = column_num;



    char *temp = (char*)malloc(MAXCOLUMNLEN*column_num);
    m_pReturnData = (char*)malloc(CMyOCCI::MAXCOLUMNLEN*column_num*StepCount*step);
    text *sqlstmt = (text *)sql_string;
    sb2 *IndiArray = (sb2*)malloc(sizeof(ub2)*column_num*step*StepCount);



    if (OCIHandleAlloc((dvoid *)m_iEnvhp, (dvoid **) &stmthp,
                       (ub4)OCI_HTYPE_STMT, (CONST size_t) 0, (dvoid **) 0))
    {
      (void) printf("FAILED: alloc statement handle\n");
      return finish_demo(m_bLoggedOn, m_iEnvhp, m_iSvchp, m_iSrvhp, m_iErrhp, m_iAuthp,
                         m_cUserName);
    }


    if (OCIStmtPrepare(stmthp, m_iErrhp, sqlstmt, (ub4)(strlen((char *)sqlstmt)+1),
                      (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT))
    {
      (void) printf("FAILED: OCIStmtPrepare() insert\n");
      report_error(m_iErrhp);
      return -1;
    }


    for(i=1; i <= column_num; i++)
    {
        if(status = OCIDefineByPos(stmthp, &p_dfn, m_iErrhp,
                          i, (void *) (temp+MAXCOLUMNLEN*(i-1)), (sb4)CMyOCCI::MAXCOLUMNLEN,
                                   SQLT_STR, (void *) &(IndiArray[i-1]), (ub2 *) 0,
                          (ub2 *) 0, OCI_DEFAULT))
        {
            (void) printf("FAILED: OCIDefineByPos() select1\n");
            checkerr(m_iErrhp, status);
            return -1;
        }
    }


    if(status = OCIStmtExecute(m_iSvchp, stmthp, m_iErrhp, (ub4) MAXITER, (ub4) 0,
                               (CONST OCISnapshot*)0,(OCISnapshot*)0,
                               (ub4) OCI_DEFAULT ))
    {
      (void) printf("FAILED: OCIStmtExecute() insert\n");
      report_error(m_iErrhp);
      return -1;
    }



//    strncpy(m_pReturnData,temp,column_num*MAXCOLUMNLEN);
//    printf("first :%s,%s,%d\n",m_pReturnData,m_pReturnData+MAXCOLUMNLEN,*((sb2*)(IndiArray+sizeof(sb2))));
    while (status != OCI_NO_DATA)
    {             /* Fetch the remaining data */

        for(i=0; i<column_num; i++)
        {

            if(-1 == IndiArray[i])
            {
                m_pReturnData[RowCount*column_num*MAXCOLUMNLEN+i*MAXCOLUMNLEN] = '\0';
            }
            else if(-2 == IndiArray[i])
            {
                cout<<"the buffer to fill output data is too small,please to enlarge it"<<endl;
                exit(1);
            }
            else if(0 < IndiArray[i])
            {
                cout<<"for some reason the output data is truncated"<<endl;
                cout<<IndiArray[i]<<", "<<i<<endl;
                exit(1);
            }
        }
        memcpy(m_pReturnData+RowCount*column_num*MAXCOLUMNLEN,temp,column_num*MAXCOLUMNLEN);
//         printf("%s,%s,%d\n",m_pReturnData+RowCount*MAXCOLUMNLEN*column_num,
//                m_pReturnData+RowCount*MAXCOLUMNLEN*column_num+MAXCOLUMNLEN,RowCount);
//        for(i=0; i<column_num; i++)
//            printf("%s,",m_pReturnData+RowCount*MAXCOLUMNLEN*column_num+i*MAXCOLUMNLEN);
//        cout<<endl;
         status = OCIStmtFetch(stmthp, m_iErrhp, 1, 0, 0);
//*((sb2*)(IndiArray+sizeof(sb2)))
         if(RowCount+4 > StepCount*step)
         {
            StepCount++;
            p = (char*)realloc(m_pReturnData,StepCount*step*MAXCOLUMNLEN*column_num);

            if(NULL != p)
            m_pReturnData = p;
         }

         RowCount++;
     }

    free(temp);
    if(NULL != stmthp)
    {
        (void) OCIHandleFree((dvoid *) stmthp, (ub4) OCI_HTYPE_STMT);
        stmthp = NULL;
    }

    if(NULL != p_dfn)
    {
        (void) OCIHandleFree((dvoid *) p_dfn, (ub4) OCI_HTYPE_DEFINE);
        p_dfn = NULL;
    }
    m_nRowNum = RowCount -1;
    return m_nRowNum;
}



void CMyOCCI::checkerr(OCIError *errhp, sword status)
{
  text errbuf[512];
  sb4 errcode = 0;

  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    (void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
    break;
  case OCI_NEED_DATA:
    (void) printf("Error - OCI_NEED_DATA\n");
    break;
  case OCI_NO_DATA:
    (void) printf("Error - OCI_NODATA\n");
    break;
  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    (void) printf("Error - %.*s\n", 512, errbuf);
    break;
  case OCI_INVALID_HANDLE:
    (void) printf("Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    (void) printf("Error - OCI_STILL_EXECUTE\n");
    break;
  case OCI_CONTINUE:
    (void) printf("Error - OCI_CONTINUE\n");
    break;
  default:
    break;
  }
}



/* ----------------------------------------------------------------- */
/* Print the error message                                           */
/* ----------------------------------------------------------------- */
void CMyOCCI::report_error(OCIError *errhp)
{
  text  msgbuf[512];
  sb4   errcode = 0;

  (void) OCIErrorGet((dvoid *) errhp, (ub4) 1, (text *) NULL, &errcode,
                       msgbuf, (ub4) sizeof(msgbuf), (ub4) OCI_HTYPE_ERROR);
  (void) printf("ERROR CODE = %d\n", errcode);
  (void) printf("%.*s\n", 512, msgbuf);
  return;
}



/* ----------------------------------------------------------------- */
/* Logon to the database using given username, password & credentials*/
/* ----------------------------------------------------------------- */
sword CMyOCCI::log_on(OCISession *authp, OCIError *errhp, OCISvcCtx *svchp,
             text *uid,text *pwd, ub4 credt, ub4 mode)
{
  /* Set attributes in the authentication handle */
  if (OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
                 (dvoid *) uid, (ub4) strlen((char *) uid),
                 (ub4) OCI_ATTR_USERNAME, errhp))
  {
    (void) printf("FAILED: OCIAttrSet() userid\n");
    return OCI_ERROR;
  }
  if (OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
                 (dvoid *) pwd, (ub4) strlen((char *) pwd),
                 (ub4) OCI_ATTR_PASSWORD, errhp))
  {
    (void) printf("FAILED: OCIAttrSet() passwd\n");
    return OCI_ERROR;
  }

  (void) printf("Logging on as %s  ....\n", uid);

  if (OCISessionBegin(svchp, errhp, authp, credt, mode))
  {
    (void) printf("FAILED: OCIAttrSet() passwd\n");
    return OCI_ERROR;
  }

  (void) printf("%s logged on.\n", uid);

  /* Set the authentication handle in the Service handle */
  if (OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
                 (dvoid *) authp, (ub4) 0, (ub4) OCI_ATTR_SESSION, errhp))
  {
    (void) printf("FAILED: OCIAttrSet() session\n");
    return OCI_ERROR;
  }

  return OCI_SUCCESS;
}


/* ----------------------------------------------------------------- */
/* Attach to server with a given mode.                               */
/* ----------------------------------------------------------------- */
sword CMyOCCI::attach_server(ub4 mode, OCIServer *srvhp,
                            OCIError *errhp, OCISvcCtx *svchp,text *Sid)
{
  //text *cstring = (text *)"orcl";

  if (OCIServerAttach(srvhp, errhp, (text *) Sid,
                     (sb4) strlen((char *)Sid), (ub4) OCI_DEFAULT))
  {
    (void) printf("FAILED: OCIServerAttach()\n");
    return OCI_ERROR;
  }

  /* Set the server handle in the service handle */
  if (OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
                 (dvoid *) srvhp, (ub4) 0, (ub4) OCI_ATTR_SERVER, errhp))
  {
    (void) printf("FAILED: OCIAttrSet() server attribute\n");
    return OCI_ERROR;
  }

  return OCI_SUCCESS;
}

/*========================== UTILITY FUNCTIONS ======================*/
/*
 * These functions are generic functions that can be used in any
 * OCI program.
 */
/* ----------------------------------------------------------------- */
/* Initialize environment, allocate handles                          */
/* ----------------------------------------------------------------- */
sword CMyOCCI::init_handles(OCIEnv **envhp, OCISvcCtx **svchp, OCIError **errhp,
                   OCIServer **srvhp, OCISession **authp, ub4 init_mode)
{
  (void) printf("Environment setup ....\n");


    if(OCIEnvCreate((OCIEnv **) envhp,OCI_DEFAULT,(dvoid *)0,
           (dvoid * (*)(dvoid *, size_t)) 0,
           (dvoid * (*)(dvoid *, dvoid *, size_t))0,
           (void (*)(dvoid *, dvoid *)) 0,
           (size_t) 0, (dvoid **) 0))
    {
        (void) printf("FAILED: OCIEnvCreate()\n");
        return OCI_ERROR;
    }

//  /* Initialize the OCI Process */
//  if (OCIInitialize(init_mode, (dvoid *)0,
//                    (dvoid * (*)(dvoid *, size_t)) 0,
//                    (dvoid * (*)(dvoid *, dvoid *, size_t))0,
//                    (void (*)(dvoid *, dvoid *)) 0 ))
//  {
//    (void) printf("FAILED: OCIInitialize()\n");
//    return OCI_ERROR;
//  }

//  /* Inititialize the OCI Environment */
//  if (OCIEnvInit((OCIEnv **) envhp, (ub4) OCI_DEFAULT,
//                 (size_t) 0, (dvoid **) 0 ))
//  {
//    (void) printf("FAILED: OCIEnvInit()\n");
//    return OCI_ERROR;
//  }

  /* Allocate a service handle */
  if (OCIHandleAlloc((dvoid *) *envhp, (dvoid **) svchp,
                     (ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0))
  {
    (void) printf("FAILED: OCIHandleAlloc() on svchp\n");
    return OCI_ERROR;
  }

  /* Allocate an error handle */
  if (OCIHandleAlloc((dvoid *) *envhp, (dvoid **) errhp,
                     (ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0))
  {
    (void) printf("FAILED: OCIHandleAlloc() on errhp\n");
    return OCI_ERROR;
  }

  /* Allocate a server handle */
  if (OCIHandleAlloc((dvoid *) *envhp, (dvoid **) srvhp,
                     (ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0))
  {
    (void) printf("FAILED: OCIHandleAlloc() on srvhp\n");
    return OCI_ERROR;
  }

  /* Allocate a authentication handle */
  if (OCIHandleAlloc((dvoid *) *envhp, (dvoid **) authp,
                     (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0))
  {
    (void) printf("FAILED: OCIHandleAlloc() on authp\n");
    return OCI_ERROR;
  }

  return OCI_SUCCESS;
}



/*-------------------------------------------------------------------*/
/* Logout and detach from the server                                 */
/*-------------------------------------------------------------------*/
void CMyOCCI::logout_detach_server(OCISvcCtx *svchp, OCIServer *srvhp,
                                  OCIError *errhp, OCISession *authp, text *userid)
{
  if (OCISessionEnd(svchp, errhp, authp, (ub4) 0))
  {
    (void) printf("FAILED: OCISessionEnd()\n");
    report_error(errhp);
  }

  (void) printf("%s Logged off.\n", userid);

  if (OCIServerDetach(srvhp, errhp, (ub4) OCI_DEFAULT))
  {
    (void) printf("FAILED: OCISessionEnd()\n");
    report_error(errhp);
  }

  (void) printf("Detached from server.\n");

  return;
}


/*---------------------------------------------------------------------*/
/* Finish demo and clean up                                            */
/*---------------------------------------------------------------------*/
sword CMyOCCI::finish_demo(boolean loggedon, OCIEnv *envhp, OCISvcCtx *svchp, OCIServer *srvhp, OCIError *errhp,
                  OCISession *authp,  text *userid)
{

  if (loggedon)
    logout_detach_server(svchp, srvhp, errhp, authp, userid);

  free_handles(envhp, svchp, srvhp, errhp, authp, NULL);

  return OCI_SUCCESS;
}


/* ----------------------------------------------------------------- */
/*  Free the specified handles                                       */
/* ----------------------------------------------------------------- */
void CMyOCCI::free_handles(OCIEnv *envhp, OCISvcCtx *svchp, OCIServer *srvhp,
                  OCIError *errhp, OCISession *authp, OCIStmt *stmthp)
{
  (void) printf("Freeing handles ...\n");

  if (srvhp)
    (void) OCIHandleFree((dvoid *) srvhp, (ub4) OCI_HTYPE_SERVER);
  if (svchp)
    (void) OCIHandleFree((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX);
  if (errhp)
    (void) OCIHandleFree((dvoid *) errhp, (ub4) OCI_HTYPE_ERROR);
  if (authp)
    (void) OCIHandleFree((dvoid *) authp, (ub4) OCI_HTYPE_SESSION);
  if (stmthp)
    (void) OCIHandleFree((dvoid *) stmthp, (ub4) OCI_HTYPE_STMT);
  if (envhp)
    (void) OCIHandleFree((dvoid *) envhp, (ub4) OCI_HTYPE_ENV);

  return;
}


sword CMyOCCI::InitDatabase()
{
    if (init_handles(&m_iEnvhp, &m_iSvchp, &m_iErrhp, &m_iSrvhp, &m_iAuthp, (ub4)OCI_OBJECT))
    {
      (void) printf("FAILED: init_handles()\n");
      return finish_demo(m_bLoggedOn, m_iEnvhp, m_iSvchp, m_iSrvhp, m_iErrhp, m_iAuthp,
                         m_cUserName);
    }

    if (attach_server((ub4) OCI_DEFAULT, m_iSrvhp, m_iErrhp, m_iSvchp, m_cSid))
    {
      (void) printf("FAILED: attach_server()\n");
      return finish_demo(m_bLoggedOn, m_iEnvhp, m_iSvchp, m_iSrvhp, m_iErrhp, m_iAuthp,
                          m_cUserName);
    }

    if (log_on(m_iAuthp, m_iErrhp, m_iSvchp, m_cUserName, m_cPassword,
               (ub4) OCI_CRED_RDBMS, (ub4) OCI_DEFAULT))
    {
      (void) printf("FAILED: log_on()\n");
      return finish_demo(m_bLoggedOn, m_iEnvhp, m_iSvchp, m_iSrvhp, m_iErrhp, m_iAuthp,
                          m_cUserName);
    }

    m_bLoggedOn = true;
    return OCI_SUCCESS;
}

CMyOCCI::~CMyOCCI()
{
    if(NULL != m_pReturnData)
        free(m_pReturnData);
    finish_demo(m_bLoggedOn, m_iEnvhp, m_iSvchp, m_iSrvhp, m_iErrhp, m_iAuthp,
                m_cUserName);
}
/*
int main(int argc, char* argv[])
{

    char sql[512];
    long i;
    CMyOCCI MyOCCI((text*)"PSIDP",(text*)"PSIDP",(text*)"orcl");

    long num =MyOCCI.exec_select(argv[1],atoi(argv[2]),"hhhhh");

    for(i=0;i<num;i++)
    {
        cout<<MyOCCI.GetStr(0,i)<<","<<MyOCCI.GetDouble(2,i)<<endl;
    }

}
*/
