//
// utils.h
//
#ifndef SHARED_FILE_UTILS_H
#define SHARED_FILE_UTILS_H

#include "SharedInclude.h"



namespace utils
{

using namespace std;
typedef long long int64;

#define MIN4(a, b, c, d)                 min(min(a,b), min(c,d))
#define BOUNDED(x, xmin, xmax)          (x >= xmin && x <= xmax)



// TODO: pull in the tuple functions from the json_parser project;



/************************************************************************************************************************************************************************/
// std::vector class helper functions.
/************************************************************************************************************************************************************************/
template<class TVal>
inline size_t argmin(const vector<TVal>& v)
{
    TVal m = v[0];
    size_t mndx = 0;
    for(size_t i = 1; i < v.size(); i++){
        if(v[i] < m){
            m = v[i];
            mndx = i; } }
    return mndx;
}

template<class TVal>
inline TVal mininum(const vector<TVal>& v)
{
    return v[argmin(v)];
}

template<class TVal>
inline size_t argmax(const vector<TVal>& v)
{
    TVal m = v[0];
    size_t mndx = 0;
    for(size_t i = 1; i < v.size(); i++){
        if(v[i] > m){
            m = v[i];
            mndx = i; } }
    return mndx;
}

template<class TVal>
inline TVal maximum(const vector<TVal>& v)
{
    return v[argmax(v)];
}

template<class TVal>
inline void FromArrayToVector(vector<TVal>& vec, const TVal * pArray, const size_t& szArray)
{
    assert(pArray);
    for( int i = 0; i < szArray && pArray; i ++ )
    {
        vec.push_back(pArray[i]);
    }
}

template<class TVal>
inline void FromVectorToArray(const vector<TVal>& vec, TVal * pArray, const size_t& szArray)
{
    assert(pArray);
    for( int i = 0; i < vec.size() && pArray; i ++ )
    {
        pArray[i] = vec[i];
    }
}

template<class TVal>
inline vector<TVal> VectorCat(const vector<TVal>& v1, const vector<TVal>& v2)
{
    vector<TVal> ret = v1;
    for(size_t i = 0; i < v2.size(); i++)
        ret.push_back(v2[i]);
    return ret;
}

#ifdef c_plus_plus_11
template<typename TVal, typename Compare = equal_to<TVal> >
inline size_t FindInVec(const vector<TVal>& v, const TVal& val)
{
    bool bFound = false;
    size_t ndx = v.size();
    Compare IsEqual;
    for(size_t i = 0; i < v.size() && !bFound; i++)
    {
        if(IsEqual(val, v[i]))
        {
            bFound = true;
            ndx = i;
        }
    }
    return ndx;
}

template<typename TVal, typename Compare = equal_to<TVal> >
inline bool IsInVec(const vector<TVal>& v, const TVal& val)
{
    return (FindInVec<TVal, Compare>(v, val) < v.size());
}

template<typename TVal, typename Compare = equal_to<TVal> >
inline bool PushUnique(vector<TVal>& v, const TVal& elem)
{
    bool bPushedElem = false;
    if(!IsInVec<TVal, Compare>(v, elem))
    {
        v.push_back(elem);
        bPushedElem = true;
    }
    
    return bPushedElem;
}

template<typename TVal, typename Compare = equal_to<TVal> >
inline vector<TVal> Intersection(const vector<TVal>& v1, const vector<TVal>& v2)
{
    vector<TVal> intersection;
    
    for(size_t i = 0; i < v1.size(); i++)
    {
        if(IsInVec<TVal, Compare>(v2, v1[i]))
            PushUnique(intersection, v1[i]); // no repeats allowed.
    }
    
    return intersection;
}
#else

template< typename TVal >
inline size_t FindInVec(const vector<TVal>& v, const TVal& val)
{
    bool bFound = false;
    size_t ndx = v.size();
    equal_to<TVal> IsEqual;
    for(size_t i = 0; i < v.size() && !bFound; i++)
    {
        if(IsEqual(val, v[i]))
        {
            bFound = true;
            ndx = i;
        }
    }
    return ndx;
}

template<typename TVal >
inline bool IsInVec(const vector<TVal>& v, const TVal& val)
{
    return (FindInVec<TVal>(v, val) < v.size());
}

template<typename TVal >
inline bool PushUnique(vector<TVal>& v, const TVal& elem)
{
    bool bPushedElem = false;
    if(!IsInVec<TVal>(v, elem))
    {
        v.push_back(elem);
        bPushedElem = true;
    }
    
    return bPushedElem;
}

template<typename TVal >
inline vector<TVal> Intersection(const vector<TVal>& v1, const vector<TVal>& v2)
{
    vector<TVal> intersection;
    
    for(size_t i = 0; i < v1.size(); i++)
    {
        if(IsInVec<TVal>(v2, v1[i]))
            PushUnique(intersection, v1[i]); // no repeats allowed.
    }
    
    return intersection;
}

#endif

/************************************************************************************************************************************************************************/
// Some helpful math functions
/************************************************************************************************************************************************************************/

// sign(x): returns +1 or -1 if val is greater or less that 0 respectively.
// modified this to also return 0 if fabs(val) < tol.
template<class TVal>
inline int sign(const TVal& val, const double tol = 0.0)
{
    assert(tol >= 0.0);
    return ( fabs(val) <= tol ? 0 : ((val < 0) ? -1 : 1));
}

inline int mod_dist(const int& a, const int& b, const int& n)
{
    int an = abs(a % n), bn = abs(b % n);
    int d = abs(an-bn);
    if(abs(d) > (n/2))
    {
        d = n - d;
    }
    return d;
}

template< class TVal>
inline TVal arccos(const TVal& x){
    if(x < -1)
        return acos(-1);
    else if(x > 1)
        return acos(1);
    else
        return acos(x);
}


// this is strange but we have to do it this way because floating point template parameters are not supported.
// tol = constant * 10^power;
template<class TVal, int power = -6, size_t constant = 1>
struct float_is_equal : binary_function<TVal, TVal, bool>
{
    TVal tol;
    float_is_equal() : tol(TVal(constant)*pow(TVal(10.0), TVal(power))) {}
    bool operator()(const TVal& x, const TVal& y) { return fabs(x-y) < tol;}
};

template<typename TVal, int power = -6, size_t constant = 1>
struct float_vec_is_equal : binary_function<vector<TVal>, vector<TVal>, bool>
{
    float_is_equal<TVal, power, constant> f;
    float_vec_is_equal() {}
    bool operator()(const vector<TVal>& x, const vector<TVal>& y)
    {
        bool bEqual = true;
        //cout << "x.size:"<< x.size() << " y.size():"<< y.size()<<endl;
        if(x.size() != y.size()) bEqual = false;
        for(size_t i = 0; i < x.size() && bEqual; i++) bEqual = f(x[i], y[i]);
        return bEqual;
    }
};



/************************************************************************************************************************************************************************/
// std::string class helper functions.
/************************************************************************************************************************************************************************/

template<class TVal>
inline string NumberToString(TVal num, const size_t& szFill = 0, const char& chFill = '0') // TODO: add formatting options.
{
    // could alternatively use sprintf.
    string str;
    stringstream stream;
    if(szFill > 0)
        stream << setfill(chFill) << setw(szFill);
    stream<<num;
    stream>>str;
    return str;
}

inline string StripWhiteSpace(const string& str)
{
    // strips the white space only at the begining or end of a string.
    string ret = str;
    size_t back = ret.length() > 0 ? ret.length()-1 : std::string::npos;
    while(std::isspace(ret[0]) || std::isspace(ret[back]))
    {
        if(std::isspace(ret[0]))
            ret.erase(0,1);
        if(std::isspace(ret[back]))
            ret.erase(back,1);
        back = ret.length() > 0 ? ret.length()-1 : std::string::npos;
    }
    return ret;
}

inline vector<string> SplitString(const string& str, const string& splitStr, bool bStripWhiteSpace = true, bool bAllowSuccessiveDelim = false)
{
    vector<string> strvec;
    size_t pos = 0; size_t prevPos = 0;
    string temp;
    bool bMore = (str.length() != 0);
    
    if(str.length() == 1)
    {
        strvec.push_back((str == splitStr) ? "" : str);
        return strvec;
    }
    else if(splitStr.length() == 0)
    {
        cout << "ERROR! Must use a valid delimiter. " << endl;
        throw(0xfffff);
    }
    
    while (bMore) {
        pos = str.find(splitStr, prevPos);
        
        if ( pos == string::npos)
        {
            pos = str.length();
            //cout << "string: "<< str << " @  ("<< prevPos << ", " << pos <<")" << endl;
            bMore = false;
        }
        if((!bAllowSuccessiveDelim) && ((pos-prevPos) == 0))
        {
            prevPos = pos + 1; // skip the extra delim.
            continue;
        }
        
        temp = str.substr((prevPos), (pos-prevPos));
        // cout << "sub string: "<< temp << " @  ("<< prevPos << ", " << pos <<")" << endl;
        
        prevPos = pos + 1;
        strvec.push_back(bStripWhiteSpace ? StripWhiteSpace(temp) : temp);
    }
    return strvec;
}

// rotate str by rot postions ( positive is forward negative is backward)
inline string InvertString(const string& str)
{
    string invert;
    size_t n = str.length();
    for(size_t i = 0; i < n; i++)
        invert += str[n-1-i];
    return invert;
}

inline bool COMP(const string& A, const string& B, size_t& i, size_t& j)
{
    bool bComp = true;
    size_t n = A.length();
    for(size_t k = 0; k < n && bComp; k++)
    {
        if(A[(i+k)%n] < B[(j+k)%n])
        {
            j += k+1;
            bComp = false;
        }
        else if (A[(i+k)%n] > B[(j+k)%n])
        {
            i += k+1;
            bComp = false;
        }
    }
    return bComp;
}

inline bool CircularCompareString(const string& A, const string& B)
{
    bool bEqual = false, bContinue = A.length() == B.length();
    size_t i = 0, j = 0, n = A.length();
    vector<size_t> DA, DB;
    while( bContinue )
    {
        bEqual = COMP(A, B, i, j);
        bContinue = i < n && j < n && !bEqual;
    }
    
    return bEqual;
}




/************************************************************************************************************************************************************************/
// General helper functions.
/************************************************************************************************************************************************************************/

template <class TVal, class UVal>
inline bool IsInList(TVal& query, UVal& list, size_t n = 0)
{
    if(n == 0)
        n = list.size();
    for(size_t i = 0; i < n; i++)
    {
        if(query == list[i])
            return true;
    }
    
    return false;
}

/************************************************************************************************************************************************************************/
//  File and Directory helper functions.
/************************************************************************************************************************************************************************/
#define DOT_CHAR                        '.'
#define DIR_DELIM_CHAR                  '/'
#define WILDCARD_CHAR                   '*'

#define DOT_STR                         "."
#define DIR_DELIM_STR                   "/"
#define WILDCARD_STR                    "*"

inline string   file_part(const string& path) {
    return (path.substr(path.find_last_of(DIR_DELIM_CHAR)+1, path.length()));
}

inline string   file_ext(const string& path) {
    string file = file_part(path);
    return (file.substr(file.find_last_of(DOT_CHAR)+1, file.length()));
}

inline string   file_name(const string& path) {
    string file = file_part(path);
    return (file.substr(0, file.find_last_of(DOT_CHAR)));
}

inline string   path_part(const string& path) {
    return (path.substr(0,path.find_last_of(DIR_DELIM_CHAR)+1));
}

inline string   path_join(const string& path, const string& join) {
    return (path.length() == 0 ? join : (path[path.length()-1] != DIR_DELIM_CHAR && join[0] != DIR_DELIM_CHAR) ? (path+DIR_DELIM_CHAR+join) : ((path[path.length()-1] == DIR_DELIM_CHAR && join[0] != DIR_DELIM_CHAR) || (path[path.length()-1] != DIR_DELIM_CHAR && join[0] == DIR_DELIM_CHAR) ? path+join : path.substr(0, path.length()-1)+join));
}

inline string   search_part(const string& path) {
    return ((path.find(WILDCARD_CHAR) == string::npos) ? path : path.substr(0,path.find(WILDCARD_CHAR)));
}

inline bool     is_directory(const dirent* dir) {
    return (dir->d_type == DT_DIR);
}

#ifdef c_plus_plus_11

template <class TVal>
bool load_txt(vector< vector<TVal> >& data, const std::string& path, string delim = ",", const size_t& reserve = 0 )
{
    ifstream txtfile;
    txtfile.open(path, ios_base::in);
    unique_ptr< char[] > buffer;
    size_t length = 0;
    
    if(txtfile)
    {
        txtfile.seekg (0, txtfile.end);
        length = txtfile.tellg();
        txtfile.seekg (0, txtfile.beg);
        // allocate memory:
        buffer.reset( new char [length] );
        // read data as a block:
        txtfile.read (buffer.get(),length);
    }
    else
    {
        return false;
    }
    
    size_t pos = 0;
    while(pos < length)
    {
        
        char * end = (char *) memchr(&buffer[pos], '\n' ,length - pos);
        size_t n = 0;
        
        if (end)
            n = size_t(ptrdiff_t(end - &buffer[pos])/sizeof(char));
        else
            n = length - pos;
        
        // cout << "Reading position " << pos << " of " << length << " for "<< n << " chars" << "\r" << std::flush;
        string line(&buffer[pos], n);
        //cout << "checkpoint 1" << endl;
        vector<string> split = SplitString(line, delim);

        if(data.size() == 0)
        {
            for(size_t i = 0; i < split.size(); i++)
            {
                vector<TVal> temp;
                data.push_back(temp);
                if( reserve > 0 )
                    data[i].reserve(reserve);
            }
        }
        //cout << "checkpoint 2" << endl;
        for(size_t i = 0; i < split.size(); i++)
        {
            TVal temp;
            stringstream ss;
            //cout << "split[i] = " << split[i] << endl;
            ss << split[i];
            ss >> temp;
            data[i].push_back( temp );
        }
        
        pos += n+1; // skip the '\n' char
        
    }
    // cout  << endl;
    
    return true;
}
template <class TVal>
inline vector< vector<TVal> > load_txt(const std::string& path, string delim = ",", const size_t& reserve = 0 )
{
    vector< vector<TVal> > data;
    load_txt<TVal>(data, path, delim, reserve);
    return data;
}

#endif

// TODO:
//  Clean up this function to make this work more expectedly.
inline vector<string> FindAllPaths(string SearchPath, __uint8_t FileTypeRestriction = 0)
{
    dirent* dirNav = NULL;
    DIR* dirc = NULL;
    bool bAdd = false;
    vector<string> paths;
    string parent = path_part(SearchPath);
    string wildCard = file_part(SearchPath);
    vector<string> searchPattern = SplitString(wildCard, WILDCARD_STR);
        
    dirc = opendir(parent.c_str());
    
    if(!dirc)
    {
        cerr << " Could not open : " << parent <<endl;
        return paths;
    }
    
    while((dirNav = readdir(dirc)) && dirc)
    {
        string name(dirNav->d_name);
        cout <<name<<(is_directory(dirNav) ? " Dir " : " File ")<<endl; // TODO: make this scan recursively.  
        bAdd = true;
        for (int i = 0; (i < searchPattern.size() && bAdd); i++)
        {
            if(name.find(searchPattern[i]) == string::npos)
                bAdd = false;
        }
        if (bAdd) paths.push_back((parent + name));
    }
    
    return paths;
}


inline long long GetSizeOfFile(FILE* file)
{
    size_t szFile = 0;
    if(file)
    {
        fseek (file , 0 , SEEK_END);
        szFile = ftell (file);
        rewind (file);
    }
    return szFile;
}

inline bool CompareFilesLineByLine(string path1, string path2, bool bPrintOut = false)
{
    bool bcomp = true; char yes;
    ifstream file1; ifstream file2;
    file1.open(path1.c_str()); file2.open(path2.c_str());
    int64 ctLinesMatch = 0;
    int64 ctLinesDiff = 0;
    
    while(!file1.eof() || (!file2.eof() && bPrintOut))
    {
        char line1[3200];  // 3200 characters, Too small?
        char line2[3200];
        file1.getline(&line1[0], 3200, '\n');
        file2.getline(&line2[0], 3200, '\n');
        string testline1 = line1;
        string testline2 = line2;
        
        
        
        if(strcmp(testline1.c_str(), testline2.c_str()))
        {
            if(bPrintOut)
            {
                cout<<"Lines are different: "<<endl;
                cout<<"file 1 : "<< testline1 << endl <<"file 2 : "<<  testline2 <<endl;
                bcomp = false;
                
                cout << endl<< " Print next line mismatch (y/n)? " ;
                cin>>yes;
                
                if(strcmp(&yes, "y"))
                    bPrintOut = true;
                else 
                    bPrintOut = false;
            }
            ctLinesDiff++;
            
            
        }
        else {
            ctLinesMatch ++;
        }

    }
    
    cout    << " lines Match : " << ctLinesMatch<<endl
            << " lines Different: " << ctLinesDiff<<endl;
    
    
    return bcomp;
    

}

inline bool CompareFiles(string Path1, string Path2, bool bPrintOut = false )
{
    FILE* pFile1 = NULL; FILE* pFile2 = NULL;
    unsigned char* buf1,*buf2;
    buf1 = NULL; buf2 = NULL;
    int64 szFile1, szFile2, szBuffer, szSmall;
    szFile1 = 0; szFile2 = 0; szBuffer = 1024*1024; // 1MB buffer
    bool ret_val = true;
    int64 ctRead1 = 0;  int64 ctRead2 = 0; int64 ctSmall = 0; int64 ctBytesMatch = 0; int64 ctBytesMisMatch = 0;
    int64 ctTotalRead = 0;
    
    pFile1 = fopen(Path1.c_str(), "r");
    pFile2 = fopen(Path2.c_str(), "r");
    
    szFile1 = GetSizeOfFile(pFile1);
    szFile2 = GetSizeOfFile(pFile2);
    szSmall = (szFile1 <= szFile2) ? szFile1 : szFile2;
    
    if(szFile1 && szFile2)
    {
        buf1 = (unsigned char*) malloc(szBuffer);
        buf2 = (unsigned char*) malloc(szBuffer);
        if(!buf1 || !buf2) return false;
        
        memset(buf1, 0x00, szBuffer);
        memset(buf2, 0x00, szBuffer);
        
        do 
        {
            ctRead1 = 0; ctRead2 = 0;
            ctRead1 = fread(buf1, 1, szBuffer, pFile1);
            ctRead2 = fread(buf2, 1, szBuffer, pFile2);
            ctSmall = (ctRead1 < ctRead2) ? ctRead1 : ctRead2;
            
            if(memcmp(buf1, buf2, ctSmall) != 0) // there is a difference in the two buffers.
            {
                int i = 0; char yes;
                while (buf1[i] == buf2[i]) i++; // find the position where the files are defferent.  
                
                ctBytesMisMatch += ctSmall - i; // Could over estimate the value of mis matched bytes.  
                
                while(bPrintOut && buf1[i] != buf2[i])
                {
                    cout << " Mismatch occured at byte: " << ctTotalRead + i<<endl;
                    cout << " File 1: ";
                    while (buf1[i] != '\n' && i < szBuffer) // print out buffer 1.  
                        cout << buf1[i++];
                    cout << buf1[i++];
                    cout << " File 2: ";
                    while (buf2[i] != '\n' && i < szBuffer) // print out buffer 1.  
                        cout << buf2[i++];
                    cout << buf2[i++];
                    
                    cout << endl<< " Print next line mismatch (y/n)? " ;
                    cin>>yes;
                    
                    if(strcmp(&yes, "y"))
                        bPrintOut = true;
                    else 
                        bPrintOut = false;
                }
                    
            }
            else {
                ctBytesMatch += ctSmall;
            }
            
            ctTotalRead += ctSmall;
            
        }while(ctTotalRead < szSmall && ctRead1 && ctRead2);
        
                
        free(buf1);
        free(buf2);
    }
    else 
    {
        cerr<<Path1 <<" size : " <<szFile1 <<endl<<Path2<< " size : " << szFile2<<endl;
    }
    
    cout    << "results" << endl<< " File 1 size : " << szFile1<<endl
            << " File 2 size: " << szFile2 << endl 
            << " Bytes Match : " << ctBytesMatch<<endl
            << " Bytes Different: " << ctBytesMisMatch<<endl;

    fclose(pFile1);
    fclose(pFile2);
    
    return ret_val;
}


/************************************************************************************************************************************************************************/
// Data analysis helper classes and functions.
/************************************************************************************************************************************************************************/

// Below are classes and functions that rely on c++11 support.
#ifdef c_plus_plus_11


class CHistogram
{
    public:
        template<class TVal>
        CHistogram(size_t nbins, vector<TVal>& data, const string& path = "", const float* const min = NULL, const float* const max = NULL) :  m_Path(path), m_nBins(nbins), m_pHist(NULL), m_pBinsEdges(NULL)
        {
            m_pHist = std::shared_ptr<size_t>(new size_t[m_nBins]);
            m_pBinsEdges = std::shared_ptr<float>(new float[m_nBins+1]);
            InitializeArrays();
            MakeHistogram(data, min, max);
            
        }
        ~CHistogram()
        {
            m_pHist.reset();
            m_pBinsEdges.reset();
            
            // cout << "hist:" << hex << m_pHist.get() << " bins: "<< m_pBinsEdges.get() << dec << endl;
        }
        void InitializeArrays()
        {
            for(size_t b = 0; b < m_nBins+1; b++)
                m_pBinsEdges.get()[b] = 0;
            
            for(size_t d = 0; d < m_nBins; d++)
                m_pHist.get()[d] = 0;
            
        }
    
        bool Save()
        {
            return SaveToFile(m_Path);
        }
        bool SaveToFile(const string& path)
        {
            FILE* pFile = NULL;
            pFile = fopen(path.c_str(), "w");
            bool bSuccess = (pFile);
            if(pFile)
            {
                PrintHistogram(pFile);
                fclose(pFile);
            }
            return bSuccess;
        }
    
        void PrintHistogram(FILE* f = stdout)
        {
            for(size_t i = 0; i < m_nBins; i++)
            {
                float width = m_pBinsEdges.get()[i+1]-m_pBinsEdges.get()[i];
                float center = m_pBinsEdges.get()[i]+(width/2.0);
                fprintf(f, "%f %lu %f \n", center, m_pHist.get()[i], width);
            }
            fflush(f);
        }
    
    protected:
        template<class TVal>
        void MakeHistogram(vector<TVal>& data, const float* const min, const float* const max)
        {
            float mindata = (!min) ? int(mininum(data)) : *min;
            float maxdata = (!max) ? int(maximum(data))+1 : *max;
            assert(mindata < maxdata);
            float delta = (maxdata - mindata)/float(m_nBins);
            cout << "min = "<< mindata << " max = " << maxdata << " nbins = "<< m_nBins << endl;
            
            // Set the bin edges.
            for(size_t b = 0; b < m_nBins+1; b++)
                m_pBinsEdges.get()[b] = delta*b + mindata;
            
            // Make the histogram.
            size_t bndx = 0;
            for(size_t d = 0; d < data.size(); d++)
            {
                bndx = int((data[d]-mindata)/delta);
                m_pHist.get()[bndx]++;
                
                assert(BOUNDED(data[d], m_pBinsEdges.get()[bndx], m_pBinsEdges.get()[bndx+1]));
                if(!BOUNDED(data[d], m_pBinsEdges.get()[bndx], m_pBinsEdges.get()[bndx+1]))
                {
                    cout << "Error! data out of histogram bin ranges! "<< endl;
                }
            }
            
        }
    
    
    private:
        string m_Path;
        size_t m_nBins;
        std::shared_ptr<size_t>  m_pHist;
        std::shared_ptr<float>   m_pBinsEdges;
};





// My tuple utility functions.

// base case
template< std::size_t I = 0, class TVal, typename... Ts >
inline typename std::enable_if< I == sizeof...(Ts), void >::type put (
                                                                        std::tuple< Ts... >& t,
                                                                        TVal& val,
                                                                        const size_t& Index)
{
    return;
}
// induction
template< std::size_t I = 0, class TVal, typename... Ts >
inline typename std::enable_if< I < sizeof...(Ts), void >::type put (
                                                                        std::tuple< Ts... >& t,
                                                                        TVal& val,
                                                                        const size_t& Index)
{
    if(I == Index)
        std::get<I>(t) = decltype(std::get<I>(t))(val);
    else
        put< (I+1), TVal, Ts... > (t, val, Index);
}

// base case
template< std::size_t I = 0, class TVal, typename... Ts >
inline typename std::enable_if< I == sizeof... (Ts), bool >::type is_type (
                                                                            std::tuple< Ts... >& t,
                                                                            const size_t& Index)
{
    return false; // Index out of range so return false.
}
// induction
template< std::size_t I = 0, class TVal, typename... Ts >
inline typename std::enable_if< I < sizeof...(Ts), bool >::type is_type (
                                                                            std::tuple< Ts... >& t,
                                                                            const size_t& Index)
{
    if(I == Index)
        return typeid(std::get<I>(t)) == typeid(TVal);
    else
        return is_type<I+1, TVal, Ts...>(t, Index);
}

// base case
template< std::size_t I = 0, class TVal, typename... Ts >
inline typename std::enable_if< I == sizeof... (Ts), TVal* >::type pull (
                                                                            std::tuple< Ts... >& t,
                                                                            const size_t& Index)
{
    return NULL; // Index out of range so return NULL.
}
// induction
template< std::size_t I = 0,  class TVal, typename... Ts >
inline typename std::enable_if< I < sizeof...(Ts), TVal* >::type pull (
                                                                            std::tuple< Ts... >& t,
                                                                            const size_t& Index)
{
    if(I == Index)
        return (TVal*)&std::get<I>(t);
    else
        return pull<I+1, TVal, Ts...>(t, Index);
}

template< typename FromVal, typename ToVal>
inline typename std::enable_if< !is_same<FromVal, ToVal>::value, void >::type assign_from (
                                                                                                FromVal& from,
                                                                                                ToVal&  to)
{
    return;
}

template< typename FromVal, typename ToVal>
inline typename std::enable_if< is_same<FromVal, ToVal>::value, void >::type assign_from (
                                                                                                FromVal& from,
                                                                                                ToVal&  to)
{
    to = from;
}

// base case
template< std::size_t I = 0, class TVal, typename... Ts >
inline typename std::enable_if< I == sizeof... (Ts), void >::type pull2 (
                                                                            std::tuple< Ts... >& t,
                                                                            TVal& val,
                                                                            const size_t& Index)
{
    return; // Index out of range so return NULL.
}
// induction
template< std::size_t I = 0,  class TVal, typename... Ts >
inline typename std::enable_if< I < sizeof...(Ts), void >::type pull2 (
                                                                            std::tuple< Ts... >& t,
                                                                            TVal& val,
                                                                            const size_t& Index)
{
    if(I == Index)
        val = std::get<I>(t);
    else
        pull2<I+1, TVal, Ts...>(t, val, Index);
}


// base case
template< std::size_t I = 0>
inline typename std::enable_if< I == 5, void>::type count_to_five_or_less(const size_t& Index)
{
    return;
}
// induction
template< std::size_t I = 0>
inline typename std::enable_if< I < 5, void>::type count_to_five_or_less(const size_t& Index)
{
    if(I < Index)
        cout << I << endl;
    count_to_five_or_less<I+1>(Index);
}

/************************************************************************************************************************************************************************/

// http://stackoverflow.com/questions/14261183/how-to-make-generic-computations-over-heterogeneous-argument-packs-of-a-variadic#comment19793780_14261183
// http://stackoverflow.com/questions/5484930/split-variadic-template-arguments


// Because how the template arguments pack the tuple must be comprised of the following types: bool, int, double, or string.
// Parse will fail if a different type is recieved.
// ?? Can this be generalized more ??
//template< typename... TVals >
//class CJSONValueTuple : public CJSONValue
//{
//    public:
//        CJSONValueTuple(const string& name, tuple<TVals...>* pval, const tuple<TVals...>& defaultVal = tuple<TVals...>()) : CJSONValue(JSON_ARRAY, name), m_pValue(pval), m_DefaultValue(defaultVal) {}
//    
//    // Overloaded Methods
//        bool Parse (const json_t* pVal)
//        {
//            bool bParseSuccess = false;
//            if(json_is_array(pVal))
//            {
//                bParseSuccess = true;
//                size_t n = json_array_size(pVal);
//                json_t* data;
//                
//                for (size_t i = 0; i < n; i++)
//                {
//                    char array_number[30]; // should be enough space.
//                    sprintf(&array_number[0], "-%zu", i);
//                    string elemName(m_name + string(array_number));
//                
//                    data = json_array_get(pVal, i);
//                    if(json_is_boolean(data))
//                    {
//                        bool temp;
//                        CJSONValueBool jtemp(elemName, &temp);
//                        jtemp.Parse(data);
//                        // std::get<i>(*m_pValue) = temp;
//                        put<0, bool, TVals...>(*m_pValue, temp, i);
//                    }
//                    else if(json_is_integer(data))
//                    {
//                        int temp;
//                        CJSONValueInt jtemp(elemName, &temp);
//                        jtemp.Parse(data);
//                        put<0, int, TVals...>(*m_pValue, temp, i);
//                    }
//                    else if(json_is_real(data))
//                    {
//                        double temp;
//                        CJSONValueFloat jtemp(elemName, &temp);
//                        jtemp.Parse(data);
//                        put<0, double, TVals...>(*m_pValue, temp, i);
//                    }
//                    else if(json_is_string(data))
//                    {
//                        string temp;
//                        CJSONValueString jtemp(elemName, &temp);
//                        jtemp.Parse(data);
//                        put<0, string, TVals...>(*m_pValue, temp, i);
//                    }
//                    else{
//                        bParseSuccess = false;
//                        fprintf(stderr, "ERROR: %s Could not parse tuple element. Unknown type. \n", m_name.c_str());
//                        break;
//                    }
//                }
//
//            }
//            else{
//                fprintf(stderr, "ERROR: %s is not an array as expected. \n", m_name.c_str());
//            }
//
//            return bParseSuccess;
//        }
//    
//        bool Dump (json_t*& pRet)
//        {
//            bool bDumpSuccess = true;
//            pRet = json_array();
//            if(pRet)
//            {
//                // size_t i = 0;
//                for(size_t i = 0; i < std::tuple_size< tuple<TVals...> >::value; i++) // for( auto& val : *m_pValue)
//                {
//                    json_t* pVal = NULL;
//                    
//                    
//                    if(is_type<0, bool, TVals...>(*m_pValue, i))
//                    {
//                        auto* pTemp = pull<0, bool, TVals...>(*m_pValue, i);
//                        CJSONValueBool tjson("", pTemp);
//                        cout << "bool data @ " << pTemp << " = "<< *pTemp << endl;
//                        bDumpSuccess = tjson.Dump(pVal) && bDumpSuccess;
//                        json_incref(pVal);
//                    }
//                    else if(is_type<0, int, TVals...>(*m_pValue, i))
//                    {
//                        auto* pTemp = pull<0, int, TVals...>(*m_pValue, i);
//                        CJSONValueInt tjson("", pTemp);
//                        cout << "int data @ " << pTemp << " = "<< *pTemp << endl;
//                        bDumpSuccess = tjson.Dump(pVal) && bDumpSuccess;
//                        json_incref(pVal);
//                    }
//                    else if(is_type<0, double, TVals...>(*m_pValue, i) ||
//                            is_type<0, float, TVals...>(*m_pValue, i) )
//                    {
//                        auto* pTemp = pull<0, double, TVals...>(*m_pValue, i);
//                        CJSONValueFloat tjson("", pTemp);
//                        cout << "float data @ " << pTemp<< " = "<< *pTemp << endl;
//                        bDumpSuccess = tjson.Dump(pVal) && bDumpSuccess;
//                        json_incref(pVal);
//                    }
//                    else if(is_type<0, string, TVals...>(*m_pValue, i))
//                    {
//                        auto* pTemp = pull<0, string, TVals...>(*m_pValue, i);
//                        CJSONValueString tjson("", pTemp);
//                        cout << "string data @ " <<  pTemp << " = "<< *pTemp << endl;
//                        bDumpSuccess = tjson.Dump(pVal) && bDumpSuccess;
//                        json_incref(pVal);
//                    }
//                    else{
//                        cout << "Error could not match data type array element. " << m_name << "[" << i << "]" << endl;
//                        pVal = NULL;
//                    }
//                    
//                    if(pVal)
//                        bDumpSuccess = (json_array_append(pRet, pVal) != -1) && bDumpSuccess;
//                    else
//                        cout << "Error could not dump array element. " << m_name << "[" << i << "]" << endl;
//                    
//                    json_decref(pVal);
//                }
//            }
//            else
//            {
//                bDumpSuccess = false;
//                cout << "Error! Could not dump array. "<< m_name << endl;
//            }
//            m_pJValue = pRet;
//            return bDumpSuccess;
//        }
//    
//    
//        const tuple<TVals...>& GetDefaultValue() { return m_DefaultValue; }
//    private:
//        tuple<TVals...>*    m_pValue;
//        tuple<TVals...>     m_DefaultValue;
//    
//};

//template< typename... Types > class param_pack; // forward definition.
//
//template< >
//class param_pack< > // specialization...base case.
//{
//};
//
//template<typename Type_, typename... Others>
//class param_pack<Type_, Others... > : public param_pack< Others... >
//{
//    public:
//        using type = Type_;
//        param_pack() : m_Size(sizeof...(Others)+1) { cout << "Initializing pack with "<< m_Size << " elements."<< endl; }
//        //using type = typename enable_if<0 == I, Type_>::type;
//
//    private:
//        size_t m_Size;
//};
/*
template< typename... Types > struct param_pack; // forward definition.

template< >
struct param_pack< > // specialization...base case.
{
};


template<typename Type_, typename... Others>
struct param_pack<Type_, Others... >
{
    using type = Type_;
    size_t m_Size;

    param_pack() : m_Size(sizeof...(Others)+1) { cout << "Initializing pack with "<< m_Size << " elements."<< endl; }
    //using type = typename enable_if<0 == I, Type_>::type;
    param_pack<Others...>&& GetNext() { return param_pack<Others...>(); }
    const size_t& size() { return m_Size; }

};


template<size_t I, typename... Values> struct param_pack_iterator;


template<typename Type_, typename... Others>
struct param_pack_iterator<0, Type_, Others...>
{
    using type = Type_;
};

template<size_t I, typename Type_, typename... Others>
struct param_pack_iterator<I, Type_, Others...> : public param_pack_iterator<I-1, Others...>
{
};

template<typename... Values> struct last;

template<> struct last < > { };

template<typename Type_, typename... Others>
struct last<Type_, Others...> : last<Others...>
{
    using type = typename enable_if<sizeof...(Others) == 0, Type_>::type;
};



template < typename... Values>
inline void SomeFunction()
{
    param_pack<Values...> pack;
    
    
    cout << endl << "** Iterator **" << endl;
    cout << 0 <<" is int:" << boolalpha << is_same< typename param_pack_iterator<0, Values...>::type, int>::value << endl;
    cout << 1 <<" is int:" << boolalpha << is_same< typename param_pack_iterator<1, Values...>::type, int>::value << endl;
    cout << 2 <<" is int:" << boolalpha << is_same< typename param_pack_iterator<2, Values...>::type, int>::value << endl;
    cout << 3 <<" is int:" << boolalpha << is_same< typename param_pack_iterator<3, Values...>::type, int>::value << endl;
    cout << "is last string:" << boolalpha << is_same< typename last<Values...>::type, string>::value << endl;
    cout << "is last double:" << boolalpha << is_same< typename last<Values...>::type, double>::value << endl;
    cout << "is last int:" << boolalpha << is_same< typename last<Values...>::type, int>::value << endl;
    cout << endl << endl;
    
    
    cout << "Finally initialized this pack with " << pack.size() << " elements" << endl;
    for(size_t i = 0; i < pack.size(); i++)
    {
        auto iter = pack.GetNext();
        typename param_pack<Values...>::type x;
        cout << i <<" is int:" << boolalpha << is_same< decltype(x), int>::value << endl;
        
    }
}


// TODO: Change the naming conventions here to be a little more representative and general.
class CValueElementBase
{
    public:
    
        virtual void    IsInt() = 0; // just a simple test function.

    
};

template<class TVal>
class CValueElement : public CValueElementBase
{
    public:
        using type = TVal;
    
        void IsInt() // overload of a test funciton.
        {
            cout << "is type int: " <<boolalpha<< is_same<type, int>::value << endl;
        }
};

class CPacklet
{
    public:
        CPacklet(){}
        ~CPacklet(){}
    
        void SetMap(const size_t& argc, ...)
        {
            
        }
    
//        template<typename TVal>
//        void getTypes()
//        {
//            CValueElement<TVal> temp;
//            m_ValueVector.push_back(temp);
//            cout    << "There are "<< 0 << " types left "<< endl
//                    << "Found "<< m_ValueVector.size() << " types so far." << endl;
//        }
        template<typename TVal, typename... Types>
        typename enable_if<sizeof...(Types) == 0, void >::type
        getTypes()
        {
            m_ValueVector.push_back( unique_ptr< CValueElementBase >(new CValueElement<TVal> ));
            cout    << "There are "<< 0 << " types left "<< endl
                    << "Found "<< m_ValueVector.size() << " types so far." << endl;
            
        }
    
        template<typename TVal, typename... Types>
        typename enable_if< 0 < sizeof...(Types) , void >::type
        getTypes()
        {
            m_ValueVector.push_back( unique_ptr< CValueElementBase >(new CValueElement<TVal> ));
            cout    << "There are "<< sizeof...(Types) << " types left "<< endl
                    << "Found "<< m_ValueVector.size() << " types so far." << endl;
            if(sizeof...(Types) > 0)
                getTypes<Types...>();
        }
    
        template<typename TVal, typename... Types>
        typename enable_if<sizeof...(Types) == 0, void >::type
        getType( const size_t& i, size_t& ct)
        {
            return;
        }
    
        template<typename TVal, typename... Types>
        typename enable_if< 0 < sizeof...(Types) , void >::type
        getType( const size_t& i, size_t& ct)
        {
            return;
        }
    
    
        template<typename... Types>
        void SetMap11()
        {
            cout << "There are "<< sizeof...(Types) << " different types" << endl;
            cout << endl;
            //CValueElement<Types...> my_Types;
            //my_Types.IsInt();
            getTypes<Types...>();
            
            for(size_t i = 0; i < m_ValueVector.size(); i++)
            {
                m_ValueVector[i]->IsInt();
            }
        }
    
        CValueElementBase* get_pack( const size_t& i)
        {
            return m_ValueVector[i].get();
        }
    
        void PrintTypes()
        {
            
        }
    
    private:
        vector< unique_ptr<CValueElementBase> > m_ValueVector; // unique ptr handles the memory management.
        vector< unique_ptr<CJSONValue> >        m_ValueMap;
};
*/


#endif



}
















#endif










