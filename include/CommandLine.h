//
//  CommandLine.h
//  SharedFolding
//
//  Created by Paul M Dodd on 11/9/13.
//  Copyright (c) 2013 Paul Dodd. All rights reserved.
//

#ifndef SharedFolding_CommandLine_h
#define SharedFolding_CommandLine_h

#include "utils.h"
namespace utils
{
using namespace std;
//template <class TVal>
class CCommandLineArgBase
{
    public:
    // Constructors and Destructors.
        CCommandLineArgBase() :  m_bInitialized(false), m_bIsRequired(false), m_bNextRequired(false) {}
        virtual ~CCommandLineArgBase() {}
    
    // Abstract Methods
        virtual bool ParseCommandLine(const char * av = NULL) {cout << "Must overide this function." << endl; return false; }
    
    // Accessor Methods.
        const bool& IsRequired() const { return m_bIsRequired; }
        const bool& IsInitialized() const { return m_bInitialized; }
        const bool& NextRequired() const { return m_bNextRequired; }
        const string& GetArgString() const { return m_Arg; }
        void SetUsage(string use) {m_Usage = use; }
        void PrintUsage() const { printf("%-20s %s\n", m_Arg.c_str(), m_Usage.c_str()); }
        // const TVal& GetValue() const { return m_Value; }
        void PushDepend(CCommandLineArgBase*  p) { m_Dependencies.push_back(p); }
        void PushExclude(CCommandLineArgBase*  p) { m_Exclusions.push_back(p); }
    
    protected:
        string      m_Arg;
        string      m_Usage;
        bool        m_bInitialized;
        bool        m_bIsRequired;
        bool        m_bNextRequired;
    
        // for more complicated commands
        vector<CCommandLineArgBase*> m_Dependencies;    // other arguments that must be required with this argument.
        vector<CCommandLineArgBase*> m_Exclusions;      // other arguments that cannont be specified with this argument.
};

template<class TVal>
class CCommandLineArgNum : public CCommandLineArgBase
{
    public:
        CCommandLineArgNum( std::string arg,
                            const TVal& min = TVal(INT32_MIN),
                            const TVal& max = TVal(INT32_MAX),
                            bool bRequired = false,
                            TVal init = TVal(0)
                            ) :  m_MinVal(min), m_MaxVal(max)
        {
            m_Arg = arg;
            m_Value = init;
            m_bIsRequired = bRequired;
            m_bNextRequired = true;
            
            assert(m_MinVal < m_MaxVal);
        }
        ~CCommandLineArgNum() {}
    
        bool ParseCommandLine(const char * av = NULL)
        {
            TVal temp = TVal(atof(av));
            if(BOUNDED(temp, m_MinVal, m_MaxVal))
            {
                m_Value = temp;
                m_bInitialized = true;
            }
            else{
                cout << "Error: "<< GetArgString() << " must exist on the interval ["<<m_MinVal<<", "<<m_MaxVal<<"]. "<< temp << " is outside that interval" << endl;
            }
            
            return m_bInitialized;
        }
    
    // operators
    friend ostream& operator << (ostream& out, CCommandLineArgNum& arg)
    {
        out << arg.GetArgString() << " " << arg.Value() << endl;
        return out;
        
    }
    
    // Acessor Methods.
        const TVal& Value() { return m_Value; }
    
    private:
        TVal m_MinVal;
        TVal m_MaxVal;
        TVal m_Value;
};


class CCommandLineArgBool : public CCommandLineArgBase
{
    public:
        CCommandLineArgBool(string arg, bool bRequired = false) : m_Value(false)
        {
            m_Arg = arg;
            m_bIsRequired = bRequired;
            m_bNextRequired = false;
        }
        ~CCommandLineArgBool() {}
    
        bool ParseCommandLine(const char * av = NULL)
        {
            m_Value = true; // always turn on? should toggle it?
            m_bInitialized = true;
            return m_bInitialized;
        }
    
    // operators
        friend ostream& operator << (ostream& out, CCommandLineArgBool& arg)
        {
            out << arg.GetArgString() << " " << (arg.Value() ? "true" : "false") << endl;
            return out;
        }
    
    // Acessor Methods.
        const bool& Value() { return m_Value; }
    
    private:
        bool            m_Value;
};


class CCommandLineArgString : public CCommandLineArgBase
{
    public:
        CCommandLineArgString(string arg, string AllowedValues = "", bool bRequired = false)
        {
            m_Arg = arg;
            m_AllowedValues=utils::SplitString(AllowedValues, ",");
            m_bIsRequired = bRequired;
            m_bNextRequired = true;
        }
        ~CCommandLineArgString() {}
    
        bool ParseCommandLine(const char * av = NULL)
        {
            string temp = string(av);
            if(m_AllowedValues.size() > 0)
            {
                if(utils::IsInList(temp, m_AllowedValues))
                {
                    m_Value = temp;
                    m_bInitialized = true;
                }
            }
            else
            {
                m_Value = temp;
                m_bInitialized = true;
            }
            return m_bInitialized;
        }
    
    // operators
        friend ostream& operator << (ostream& out, CCommandLineArgString& arg)
        {
            out << arg.GetArgString() << " " << arg.Value() << endl;
            return out;
        }
    
    // Acessor Methods.
        const string& Value() { return m_Value; }
    
    private:
        vector<string> m_AllowedValues;
        string m_Value;
};

template <class TVal>
class CCommandLineArgVector : public CCommandLineArgBase
{
    public:
        CCommandLineArgVector(std::string arg, string delim = ",", bool bRequired = false) : m_Delim(delim)
        {
            m_Arg = arg;
            m_bIsRequired = bRequired;
            m_bNextRequired = true;
        }
        ~CCommandLineArgVector() {}
    
        bool ParseCommandLine(const char * av = NULL)
        {
            string strArg(av);
            vector<string> strVec = utils::SplitString(strArg, m_Delim, true);
            for(size_t i = 0; i < strVec.size(); i++)
            {
                TVal temp; stringstream ss;
                ss << strVec[i];
                ss >> temp;
                m_Value.push_back(temp);
            }
            m_bInitialized = (m_Value.size() > 0);
            return m_bInitialized;
        }
    
    // operators
    friend ostream& operator << (ostream& out, CCommandLineArgVector& arg)
    {
        out << arg.GetArgString() << " vector of size " << arg.Value().size() << endl;
        return out;
        
    }
    
    // Acessor Methods.
        const vector<TVal>& Value() { return m_Value; }
    
    private:
        string          m_Delim;
        vector<TVal>    m_Value;

};


class CCommandLineParser
{
    public:
        CCommandLineParser(string name)
        {
            m_ProgramName = name;
        }
        ~CCommandLineParser() {}
        void AddArg(CCommandLineArgBase& arg)
        {
            m_Args.insert(pair<string, CCommandLineArgBase*>(arg.GetArgString(), &arg));
        }
        CCommandLineArgBase* GetArg(const string& argstr)
        {
            map<string, CCommandLineArgBase*>::iterator iter = m_Args.find(argstr);
            if(iter == m_Args.end())
            {
                cout << "Argument (" << argstr << ") is unknown." << endl;
                return NULL;
            }
            return iter->second;
        }
    
        void PrintArgs(){}
    
        void PrintUsage() const
        {
            map<string, CCommandLineArgBase*>::const_iterator iter;
            
            printf("%s ", m_ProgramName.c_str());
            for(iter = m_Args.cbegin(); iter != m_Args.cend(); iter++)
                printf("%s", ((iter->second->IsRequired() ? "<" : "[") + iter->second->GetArgString() + (iter->second->IsRequired() ? ">" : "]")).c_str());
            printf("\n");
            for(iter = m_Args.begin(); iter != m_Args.end(); iter++)
                iter->second->PrintUsage();
        }
    
        bool Parse(int argc, const char * argv[])
        {
            bool bParseComplete = true;
            for(int i = 1; i < argc; i++)
            {
                CCommandLineArgBase* pArg = GetArg(string(argv[i]));
                if(!pArg)
                {
                    cout << " Error! unknown argument: "<< argv[i] << endl;
                    bParseComplete = false;
                    break;
                }
                else
                {
                    if(pArg->NextRequired())
                    {
                        bParseComplete = pArg->ParseCommandLine(argv[++i]) && bParseComplete;
                    }
                    else
                    {
                        bParseComplete = pArg->ParseCommandLine() && bParseComplete;
                    }
                }
            }
            
            map<string, CCommandLineArgBase*>::iterator iter;
            for(iter = m_Args.begin(); iter != m_Args.end(); iter++)
            {
                if(!iter->second->IsInitialized() && iter->second->IsRequired())
                {
                    cout << "Error! Argument ("<< iter->second->GetArgString() << ") is required."<< endl;
                    bParseComplete = false;
                }
            }
            
            return bParseComplete;
        }
    
        size_t NumArgs() const { return m_Args.size(); }
    
    private:
        string                              m_ProgramName;
        map<string, CCommandLineArgBase*>   m_Args;
};

// Assumptions
// Should be Directed-Acyclic-Graph.
// command option cannot have the same name as a sub-command


// Add a to lower and to upper case for string comparison.

#define CMD_LINE_NO_ERROR               0x00000000
#define CMD_LINE_PARSE_ERROR            0xf0000001
#define CMD_LINE_PRINT_PARENT_HELP      0xf0000002
#define CMD_LINE_PRINT_HELP             0xf0000004

class CCommandBase
{
    public:
        CCommandBase(const string& name) : m_Name(name), m_Parser(name)
        {
        }
        ~CCommandBase(){}
    
        CCommandBase* GetCommand(const string& cmdstr)
        {
            map<string, CCommandBase*>::iterator iter = m_Commands.find(cmdstr);
            if(iter == m_Commands.end())
            {
                return NULL;
            }
            return iter->second;
        }
    
        void AddCommand( CCommandBase& cmd )
        {
            m_Commands.insert(pair<string, CCommandBase*>(cmd.GetName(), &cmd));
        }
    
        int ParseCommand(int argc, const char* argv[])
        {
        
            int ret = CMD_LINE_NO_ERROR;
            
            if(argc >= 2)
            {

                // cout << "parsing command "<< argv[0] << endl; // name of this command....
                CCommandBase* pCommand = GetCommand(string(argv[1])); // name of the next sub-command.
                if(pCommand)
                {
                    ret = pCommand->ParseCommand(argc - 1, &argv[1]); // final return code from the command.
                }
                else // there are no more sub-commands.
                {
                    if (ProcessCommand(argc, argv)) {
                        ret = this->ExecuteMain(); // leaf command executes
                    }
                }
            }
            else if(argc == 1)
            {
                if (ProcessCommand(argc, argv)) {
                    ret = this->ExecuteMain(); // leaf command executes
                }
            }
            else
            {
                cout << "Error too few arguments ("<< argc << " arguments) "<< endl;
                ret = CMD_LINE_PARSE_ERROR;
            }
            // Any arror handling here...
            if(ret == CMD_LINE_PRINT_HELP)
            {
                PrintHelp();
                ret =  CMD_LINE_NO_ERROR; // do not print any parent command helps...
            }
            else if(ret == CMD_LINE_PRINT_PARENT_HELP)
            {
                ret = CMD_LINE_PRINT_HELP;
            }
            

            return ret;
        }
    
        bool ProcessCommand(int argc, const char* argv[], bool bPrintHelpOnFail = true)
        {
            if(m_Parser.Parse(argc, argv))
            {
                return true;
            }
            else
            {
                if(bPrintHelpOnFail)
                    PrintHelp();
                return false;
            }
        }
    
        void PrintHelp() const
        {
            cout << endl;
            PrintDescription();
            if(m_Commands.size())
            {
                cout  << endl << "SUB-COMMANDS : " << endl;
                map<string, CCommandBase*>::const_iterator iter;
                for(iter = m_Commands.cbegin(); iter != m_Commands.cend(); iter++)
                    iter->second->PrintDescription();
            }
            if(m_Parser.NumArgs() > 0)
            {
                cout << endl << "USAGE : "  << endl;
                PrintUsage();
            }
            cout << endl << endl;
        }
        void PrintDescription() const { printf("%-20s %s\n", m_Name.c_str(), m_Description.c_str()); }
        void PrintUsage() const { m_Parser.PrintUsage(); }
        void SetDescription(const string& desc) { m_Description = desc; }
    
        const string& GetName() const { return m_Name; }
    // Virtual methods

        virtual int ExecuteMain() = 0;
    
    protected:
        string                          m_Name;
        string                          m_Description;
        map<string, CCommandBase* >     m_Commands;  // sub-commands
        CCommandLineParser              m_Parser;
};

class CProgramBase : public CCommandBase
{
    public:
        CProgramBase(string name, int argc, const char * argv[]) : CCommandBase(name), m_Argc(argc), m_Argv(argv) {}
        ~CProgramBase() {}
    
        virtual int ExecuteMain() // default is to print the usage for anysub commands.
        {
            PrintHelp();
            return 0;
        }
    
        int Main()
        {
            // cout << "Running main "<< m_Argv[0] << endl; // name of this program....
            int ret = ParseCommand(m_Argc, m_Argv);
            
            if(ret == CMD_LINE_PRINT_HELP)
            {
                PrintHelp();
            }
            
            return ret;
        }
    
    protected:
        int                     m_Argc;
        const char**            m_Argv;
};

class CCommandHelp : public CCommandBase
{
    public:
        CCommandHelp() : CCommandBase("HELP") { m_Description = " Print the help for command "; }
        ~CCommandHelp() {}
        int ExecuteMain() { return CMD_LINE_PRINT_PARENT_HELP; }
};

CCommandHelp g_HelpCommand;

}

#endif











