#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlfront.h>          /* sqlfront.h always comes first */
#include <sybdb.h>             /* sybdb.h is the only other file you need */
#define SQLDBIP "172.23.1.124"  //SQL数据库服务器IP
#define SQLDBPORT "1433"       //SQL数据库服务器端口
#define SQLDBNAME "master"     //SQL数据库服务器数据库名
#define SQLDBUSER "sa"         //SQL数据库服务器数据库用户名
#define SQLDBPASSWD "aki@test" //SQL数据库服务器用户密码
#define SQLDBSERVER SQLDBIP ":" SQLDBPORT
#define DBSQLCMD "select conn.session_ID as SPID,conn.client_net_address as IPAddress,sess.host_name as MachineName,sess.program_name as ApplicationName,login_name as LoginName,sess.nt_user_name as SystemName FROM sys.dm_exec_connections conn inner join sys.dm_exec_sessions sess ON conn.session_ID=sess.session_ID ORDER BY conn.connect_time"
int main(int argc, char *argv[])
{
    int i, ch;
    LOGINREC *login;   //描述客户端的结构体，在连接时被传递到服务器.
    DBPROCESS *dbproc; //描述连接的结构体，被dbopen()函数返回
    RETCODE erc;       //库函数中最普遍的返回类型.
                       /*************************************************************/
                       //在开始调用本库函数前常常要先调用dbinit()函数
    if (dbinit() == FAIL)
    {
        fprintf(stderr, "%s:%d: dbinit() failed\n", argv[0], __LINE__);
        exit(1);
    }
    //dblogin()函数申请 LOGINREC 结构体，此结构体被传递给dbopen()函数，用来创建一个连接。
    //虽然基本上不会调用失败，但是检查它！.
    if ((login = dblogin()) == NULL)
    {
        fprintf(stderr, "%s:%d: unable to allocate login structure\n", argv[0], __LINE__);
        exit(1);
    }
    //LOGINREC结构体不能被直接访问，要通过以下宏设置，下面设置两个必不可少的域
    DBSETLUSER(login, SQLDBUSER);
    DBSETLPWD(login, SQLDBPASSWD);
    /*************************************************************/
    //dbopen()与服务器建立一个连接. 传递 LOGINREC 指针和服务器名字
    if ((dbproc = dbopen(login, SQLDBSERVER)) == NULL)
    {
        fprintf(stderr, "%s:%d: unable to connect to %s as %s\n",
                argv[0], __LINE__,
                SQLDBSERVER, SQLDBUSER);
        exit(1);
    }
    // 可以调用dbuser()函数选择我们使用的数据库名，可以省略，省略后使用用户默认数据库.
    if (SQLDBNAME && (erc = dbuse(dbproc, SQLDBNAME)) == FAIL)
    {
        fprintf(stderr, "%s:%d: unable to use to database %s\n",
                argv[0], __LINE__, SQLDBNAME);
        exit(1);
    }
    /*************************************************************/
    dbcmd(dbproc, DBSQLCMD); //将SQL语句填充到命令缓冲区
    printf("\n");
    if ((erc = dbsqlexec(dbproc)) == FAIL)
    {
        fprintf(stderr, "%s:%d: dbsqlexec() failed\n", argv[0], __LINE__);
        exit(1); //等待服务器执行SQL语句，等待时间取决于查询的复杂度。
    }
    /*************************************************************/
    //在调用dbsqlexec()、dbsqlok()、dbrpcsend()返回成功之后调用dbresults()函数
    printf("then fetch results:\n");
    int count = 0;
    while ((erc = dbresults(dbproc)) != NO_MORE_RESULTS)
    {
        struct col
        {                 //保存列的所有信息
            char *name;   //列名字
            char *buffer; //存放列数据指针
            int type, size, status;
        } * columns, *pcol;
        int ncols;
        int row_code;
        if (erc == FAIL)
        {
            fprintf(stderr, "%s:%d: dbresults failed\n",
                    argv[0], __LINE__);
            exit(1);
        }
        ncols = dbnumcols(dbproc); //返回执行结果的列数目
        if ((columns = calloc(ncols, sizeof(struct col))) == NULL)
        {
            perror(NULL);
            exit(1);
        }
        /* read metadata and bind. */
        for (pcol = columns; pcol - columns < ncols; pcol++)
        {
            int c = pcol - columns + 1;
            pcol->name = dbcolname(dbproc, c); //返回指定列的列名
            pcol->type = dbcoltype(dbproc, c);
            pcol->size = dbcollen(dbproc, c);
            printf("%*s", 30, pcol->name);
            if ((pcol->buffer = calloc(1, 30)) == NULL)
            {
                perror(NULL);
                exit(1);
            }
            erc = dbbind(dbproc, c, NTBSTRINGBIND, 30, (BYTE *)pcol->buffer);
            if (erc == FAIL)
            {
                fprintf(stderr, "%s:%d: dbbind(%d) failed\n",
                        argv[0], __LINE__, c);
                exit(1);
            }
            erc = dbnullbind(dbproc, c, &pcol->status); //(5)
            if (erc == FAIL)
            {
                fprintf(stderr, "%s:%d: dbnullbind(%d) failed\n",
                        argv[0], __LINE__, c);
                exit(1);
            }
        }
        printf("\n");
        /* 打印数据 */
        while ((row_code = dbnextrow(dbproc)) != NO_MORE_ROWS)
        { //读取行数据
            switch (row_code)
            {
            case REG_ROW:
                for (pcol = columns; pcol - columns < ncols; pcol++)
                {
                    char *buffer = pcol->status == -1 ? "null" : pcol->buffer;
                    printf("%*s", 30, buffer);
                }
                printf("\n");
                break;
            case BUF_FULL:
                break;
            case FAIL:
                fprintf(stderr, "%s:%d: dbresults failed\n",
                        argv[0], __LINE__);
                exit(1);
                break;
            default: // (7)
                printf("data for computeid %d ignored\n", row_code);
            }
        }
        /* free metadata and data buffers */
        for (pcol = columns; pcol - columns < ncols; pcol++)
        {
            free(pcol->buffer);
        }
        free(columns);
        if (DBCOUNT(dbproc) > -1) /* 得到SQL语句影响的行数 */
            fprintf(stderr, "%d rows affected\n", DBCOUNT(dbproc));
    }
    dbclose(dbproc);
    dbexit();
}

