#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <iomanip>
#include <algorithm>

#define SQR(a) ((a)*(a))

constexpr double R0 = 8.3143;               //universal gas constant
constexpr double ACCURACY_LEVEL = 1.0E-005; //calculation accuracy
constexpr double MAX_MACH = 50;             //maximum Mach number

class property                              //auxiliary fluid class methods
{
public:
    virtual ~property(){};
    class  Invalid {};                                              //error handling
    virtual size_t BinarySearch(double val, size_t DATA,
	                            const double *arr) const;           //binary search of nearest to val element in array arr
    virtual double GetValue (double t, const double *a,
	                         size_t DATA, const double *arr) const; //search propertie value at some temperture
};

class fluid : public property //fluid properties
{
public:
    explicit fluid(std::string file_name);
    ~fluid()
    {
        _CleanUp(); //free memory
    }
    double get_mu (double t) const   //get fluid dynamic viscosity at temperture t
    {
        return GetValue(t, mu, TEMP_DATA, this->t);
    }
    double get_wsum (double t) const //get fluid molar mass at temperture t
    {
        return GetValue(t, wsum, TEMP_DATA, this->t);
    }
    double get_cp (double t) const   //get specific heat at temperture t
    {
        return GetValue(t, cp, TEMP_DATA, this->t);
    }
    double get_pr (double t) const   //get Prandtle number at temperture t
    {
        return GetValue(t, pr, TEMP_DATA, this->t);
    }
    double get_k () const            //get average adiabatic coefficient
    {
        return k;
    }
    double get_RG (double t) const   //get gas constant at temperture t
    {
        return R0/GetValue(t, wsum, TEMP_DATA, this->t);
    }
    double get_t0 (double p) const   //get total temperature at pressure p
    {
        return GetValue(p, t0, PRES_DATA, this->p);
    }

private:
    void _CleanUp();   //free memory
    std::string name;  //name of fluid properties file
    size_t PRES_DATA;  //number of pressure values
    size_t TEMP_DATA;  //number of temperature values
    double *cp;        //specific heat
    double *mu;        //dynamic viscosity
    double *wsum;      //molar mass
    double *pr;        //Prandtle number
    double *p;         //pressure array
    double *t;         //temperature array
    double *t0;        //total temperature array
    double k;          //adiabatic coefficient
};

class Nozzle
{
public:
    explicit Nozzle(std::string file_name);
    class Invalid{};
    void output(std::string file_name);

private:
    double TFACTOR {0.8};             //wall and fluid temperature ratio
    double FA_func (double m) const;  //Fa(M) - amount of extension of the nozzle, m - Mach number
    double FA_solve (double a, double b, double fa) const;
    double Get_Mach (size_t i) const;
    double d_min;     //throat diametr
    double p_chamber; //pressure in the combustion chamber
    double t_chamber; //temperture in the combustion chamber
    double k;         //average adiabatic coefficient
    double Rk;        //gas constant
    std::vector <std::pair <double, int>> di; //section diametr and flow type
    std::vector <double> s_ef;                //effective length of boundary layer
    std::vector <double> fa;                  //amount of extension of the nozzle
    std::vector <double> p;                   //pressure
    std::vector <double> t;                   //temperature
    std::vector <double> a;                   //local sound speed
    std::vector <double> w;                   //flow velocity
    std::vector <double> mach;                //Mach number
    std::vector <double> alfa;                //heat transfer coefficient
    std::vector <double> te;                  //characteristic temperature
    std::string fuel_name;                    //flud properties file name
    double alfa_avd (const fluid &flow, size_t i) const; //heat transfer coefficient
    double Te_avd   (const fluid &flow, size_t i) const; //characteristic temperature
};

std::string GetCurrentDir ();
void keep_window_open ();
void err(std::string s);
