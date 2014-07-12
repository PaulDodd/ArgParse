//
// Stopwatch.h
// Paul Dodd
// 04/25/2013
// 
 
namespace utils
{

using namespace std;
#if __cplusplus >= 201103L

class CStopwatch
{
    // Uses the time.h for cross platform support as a result only to the second precision is obtained.  
    public:
        CStopwatch(string message = "") : m_Start(0), m_Now(0), m_bPrintOnDestroy(true)
        {
            time(&m_Start);
            m_LastLap = m_Start;
            m_Message = message;
        }
        ~CStopwatch() 
        {
            if( m_bPrintOnDestroy ){
                cout << endl << endl ;
                PrintElapsedTime();
            }
        } 
        double Lap()
        {
            // returns the number of seconds that have elapsed between the start and now.
            time(&m_Now);
            double diff = difftime(m_Now, m_LastLap);
            m_LastLap = m_Now;
            return diff;
        }
        double ElapsedTime()
        {
            // returns the number of seconds that have elapsed between the start and now. 
            time(&m_Now);
            double diff = difftime(m_Now, m_Start);
            return diff;
        }

        void PrintElapsedTime()
        {
            cout << m_Message << " Elapsed time: " <<setprecision(4) << ElapsedTime() << " seconds " << endl;
        }

    private:
        time_t m_Start;
        time_t m_LastLap;
        time_t m_Now;
        bool   m_bPrintOnDestroy;
        string m_Message;
};

#else

#include <chrono>
// update this class to use the updated clock classes.
class CStopwatch
{
    // Uses the time.h for cross platform support as a result only to the second precision is obtained.  
    public:
        CStopwatch(string message = "") : m_Start(0), m_Now(0), m_bPrintOnDestroy(true)
        {
            time(&m_Start);
            m_LastLap = m_Start;
            m_Message = message;
        }
        ~CStopwatch() 
        {
            if(m_bPrintOnDestroy)
                PrintElapsedTime();
        } 
        double Lap()
        {
            // returns the number of seconds that have elapsed between the start and now.
            time(&m_Now);
            double diff = difftime(m_Now, m_LastLap);
            m_LastLap = m_Now;
            return diff;
        }
        double ElapsedTime()
        {
            // returns the number of seconds that have elapsed between the start and now. 
            time(&m_Now);
            double diff = difftime(m_Now, m_Start);
            return diff;
        }

        void PrintElapsedTime()
        {
            cout << endl << endl << m_Message << " Elapsed time: " <<setprecision(4) << ElapsedTime() << " seconds " << endl;
        }

    private:
        time_t m_Start;
        time_t m_LastLap;
        time_t m_Now;
        bool   m_bPrintOnDestroy;
        string m_Message;
};


#endif


}




