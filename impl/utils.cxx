

#include "utils.h"
#include "CommandLine.h"
#include "Stopwatch.h"



using namespace std;
using namespace utils;

class CTestSubCommand : public CCommandBase
{
    public:
        CTestSubCommand() : CCommandBase("sub"), m_Path("-path", "", true), m_Number("-n", 0, 10, false), m_bFlag("-flag")
        {
            SetDescription      ("This is a test sub-command");
            m_Path.SetUsage     ("Path to a file");
            m_Number.SetUsage   ("Integer between 0 and 10");
            m_bFlag.SetUsage    ("Boolean example");
            
            m_Parser.AddArg(m_Path);
            m_Parser.AddArg(m_Number);
            m_Parser.AddArg(m_bFlag);
            
        }
        ~CTestSubCommand(){}
    
        int ExecuteMain()
        {

            cout<< "Executing command " << m_Name << endl;
            if(m_Path.IsInitialized())
                cout << "Path : "<< m_Path.Value() << endl;
            if(m_Number.IsInitialized())
                cout << "Number : "<< m_Number.Value() << endl;
            if(m_bFlag.IsInitialized())
                cout << "Flag : "<< (m_bFlag.Value() ? "true" : "false") << endl;
        
            return CMD_LINE_NO_ERROR;
        }
    private:
    // Arguments
        CCommandLineArgString       m_Path;
        CCommandLineArgNum<size_t>  m_Number;
        CCommandLineArgBool         m_bFlag;
    
};

class CTestCommand : public CCommandBase
{
    public:
        CTestCommand() : CCommandBase("test")
        {
            SetDescription("Command for all tests.");
            AddCommand(m_SubCommand);
        }
        ~CTestCommand() {}
    
        int ExecuteMain()
        {
            return CMD_LINE_PRINT_HELP; // this command is a no-op
        }
    
    private:
    // sub-commands
        CTestSubCommand     m_SubCommand;
};

class TestProgram : public CProgramBase
{
    public:
        TestProgram(const string& name, int argc, const char * argv[]) : CProgramBase(name, argc, argv)
        {
            SetDescription("This program demonstrates argparse");
            AddCommand(m_TestCommand);
        }
        ~TestProgram() {}
    private:
        CTestCommand m_TestCommand;
    
};

int main(int argc, const char * argv[])
{
    return TestProgram("argparse_test", argc, argv).Main();
}









































