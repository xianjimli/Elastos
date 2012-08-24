
#ifndef __TSTPRINTF_H__
#define __TSTPRINTF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elatypes.h>
#include <stdarg.h>
using namespace Elastos;

// ERROR
// SUCCESS
//
enum eTpType {
    ERR,
    SUC
};

// TPDECL: Declare class
// TPINIT: Initialize, if deadlock or debug happen, write deadlock or debug
//         information to output file.
//
//         Open the output file and clear it, then write user information to it.
//         SUC means running is succesful, in this type, file is "a+" mode.
//         ERR means running is wrong, in this type, file is "w" mode, file name
//             and line will be logged.
//
// TPOK  : TestingPrintf OK
//
// TP    : Appended TestingPrintf with no error
//
// TPERR : Appended TestingPrintf with error info, command line file name and line.
//

#define _MAX_PATH       260
#define TPDECL          class CTstPrintf TstPrintf
#define TPINIT          TstPrintf.Initialize(argc, argv, __FILE__, __LINE__)
#define TPEND           TstPrintf.TestingEnd()

#define TPOK            CTrace(SUC, NULL, NULL)("OK")

#define TP              CTrace(SUC, NULL, NULL)
#define TPERR           CTrace(ERR, __FILE__, __LINE__)

extern class CTstPrintf TstPrintf;

class CTstPrintf
{
    friend class CTrace;

public:
    char * Initialize(int nArgc, char **ppArgv,
		const char *pszFileName, const int nLineNumber)
    {
        int nArgcTmp = 1, nCmdLen = 0;

        BuildOutputFile(nArgc, ppArgv);

        nCmdLen += sprintf(m_szCommandLine, *ppArgv);

        do {
            nCmdLen += sprintf(
                m_szCommandLine + nCmdLen, "\x20%s", *(ppArgv + nArgcTmp));
        } while (++nArgcTmp < nArgc);

        m_fpOutput = fopen(m_szOutputFileName, "w");
        if (!m_fpOutput) {
            printf("*ERROR* %s, %d\n", __FILE__, __LINE__);
            return NULL;
        }

        fprintf(m_fpOutput,
                "*ERROR* [ %s ]:\nDeadlock or debug while running\n%s, %d\n",
                m_szCommandLine, pszFileName, nLineNumber);

        fclose(m_fpOutput);

        m_bIsAppendMode = FALSE;
        m_bIsTPCalled = FALSE;

        return m_szOutputFileName;
    }

    void TestingEnd() {
        if (!m_bIsTPCalled) {
            m_fpOutput = fopen(m_szOutputFileName, "w");
            if (m_fpOutput) fclose(m_fpOutput);
        }
    }

    int TestingPrintf(char const *pszFormat, va_list ap)
    {
        if (m_bIsAppendMode) {
            m_fpOutput = fopen(m_szOutputFileName, "a+");
        }
        else {
            m_fpOutput = fopen(m_szOutputFileName, "w");
        }

        if (!m_fpOutput) {
            printf("*ERROR* %s, %d\n", __FILE__, __LINE__);
            return -1;
        }

        m_bIsTPCalled = TRUE;
        m_bIsAppendMode = TRUE;

        if (m_eTpType == ERR) {
            printf("*ERROR* [ %s ]:\n", m_szCommandLine);
            fprintf(m_fpOutput, "*ERROR* [ %s ]:\n", m_szCommandLine);
        }

        if (m_eTpType == SUC || m_eTpType == ERR) {
            vprintf(pszFormat, ap);
            vfprintf(m_fpOutput, pszFormat, ap);
        }
        else {
            printf("*WARNING* Unknown print type\n");
        }

        if (m_eTpType == ERR) {
            printf("%s, %d\n",
                    m_szProgramFileName, m_szProgramLineNumber);
            fprintf(m_fpOutput, "%s, %d\n",
                    m_szProgramFileName, m_szProgramLineNumber);
        }

        fclose(m_fpOutput);

        return 0;
    }

private:
    void BuildOutputFile(int nArgc, char **ppArgv)
    {
        char *pszBeg = NULL, *pszEnd = NULL;
        char szPath[_MAX_PATH];

        pszBeg = strrchr(*ppArgv, '/');

        if (pszBeg != NULL) {
            pszEnd = strchr(pszBeg, '.');
        }
        else {
            pszBeg = strrchr(*ppArgv, '\\');

            if (pszBeg != NULL) {
                pszEnd = strchr(pszBeg, '.');
            }
            else {
                pszBeg = *ppArgv;
                pszEnd = strchr(*ppArgv, '.');
            }
        }

        if (pszEnd != NULL) {
            if (*pszBeg == '/' || *pszBeg == '\\') pszBeg += 1;
            strcpy(szPath, pszBeg);
        }
        else {
            strcpy(szPath, *ppArgv);
            strcat(szPath, ".exe");
        }

        //_fullpath(m_szOutputFileName, szPath, _MAX_PATH);
        strcpy(m_szOutputFileName, "/data/");
        strcat(m_szOutputFileName, szPath);

        *(m_szOutputFileName + strlen(m_szOutputFileName) - 4) = 0;

        if (nArgc > 1) strcat(m_szOutputFileName, *(ppArgv + 1));

        strcat(m_szOutputFileName, ".out");
    }

private:
    FILE *m_fpOutput;

    char m_szOutputFileName[_MAX_PATH];
    char m_szCommandLine[_MAX_PATH];

    bool m_bIsAppendMode;
    bool m_bIsTPCalled;

    char m_szProgramFileName[_MAX_PATH];
    int m_szProgramLineNumber;
    eTpType m_eTpType;
};

class CTrace
{

public:
    CTrace(eTpType tpType, const char *pszFileName, int nLineNumber)
    {
        TstPrintf.m_eTpType = tpType;
        TstPrintf.m_szProgramLineNumber = nLineNumber;

        if (pszFileName) {
            strcpy(TstPrintf.m_szProgramFileName, pszFileName);
        }
    }

    void operator()(const char *pszFormat, ...)
    {
        va_list ap;
        va_start(ap, pszFormat);

        TstPrintf.TestingPrintf(pszFormat, ap);

        va_end(ap);
    }
};

#endif /* __TSTPRINTF_H__ */
