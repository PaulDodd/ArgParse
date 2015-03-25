/*
 *  SharedInclude.h
 *  
 *
 *  Created by Paul on 11/27/11.
 * 
 *
 */
#pragma once

#include <assert.h>
#include <math.h>
#include <ctime>
#include <complex>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>

#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <climits>
#include <memory>
#include <ctype.h>
#include <utility>

// Datatypes
typedef unsigned long long  uint64;

// Global #defines
#define SMALL           (1.0e-12)  // Should set this to the computer epsilon
#define SMALL_FLOAT     (1.0e-6)
#define LARGE           (1.0e10)



inline const std::string  RESET() { return  "\033[0m"; }
inline const std::string  BLACK() { return    "\033[30m"; }      /* Black */
inline const std::string  RED() { return      "\033[31m"; }      /* Red */
inline const std::string  GREEN() { return    "\033[32m"; }      /* Green */
inline const std::string  YELLOW() { return   "\033[33m"; }      /* Yellow */
inline const std::string  BLUE() { return     "\033[34m"; }      /* Blue */
inline const std::string  MAGENTA() { return  "\033[35m"; }      /* Magenta */
inline const std::string  CYAN() { return     "\033[36m"; }      /* Cyan */
inline const std::string  WHITE() { return    "\033[37m"; }     /* White */
inline const std::string  BOLDBLACK() { return    "\033[1m\033[30m"; }      /* Bold Black */
inline const std::string  BOLDRED() { return      "\033[1m\033[31m"; }      /* Bold Red */
inline const std::string  BOLDGREEN() { return    "\033[1m\033[32m"; }      /* Bold Green */
inline const std::string  BOLDYELLOW() { return   "\033[1m\033[33m"; }      /* Bold Yellow */
inline const std::string  BOLDBLUE() { return     "\033[1m\033[34m"; }      /* Bold Blue */
inline const std::string  BOLDMAGENTA() { return  "\033[1m\033[35m"; }      /* Bold Magenta */
inline const std::string  BOLDCYAN() { return     "\033[1m\033[36m"; }      /* Bold Cyan */
inline const std::string  BOLDWHITE() { return    "\033[1m\033[37m"; }      /* Bold White */



// Environment defines
#if __cplusplus >= 201103L
    #ifndef c_plus_plus_11
        #define c_plus_plus_11
    #endif
#endif

























