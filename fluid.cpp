//The fluid class is responsible for storage and transfer to other modules
//the properties of the fluid
#include "nozzle_lib.h"

fluid::fluid (std::string file_name)
{
    try
    {
        name = file_name;                                   //name of properties file
        std::ifstream input (GetCurrentDir()+ "\\" + name); //open properties file
        if (!input) throw Invalid {};                       //if can't open generate exception

        input >> k >> PRES_DATA >> TEMP_DATA; // read average ratio of specific heats
		                                      // and number of pressure and temperature values

        p = new double [PRES_DATA];    //pressure
        t0 = new double [PRES_DATA];   //temperature in the combustion chamber depending on the pressure
        t = new double [TEMP_DATA];    //temperature
        mu = new double [TEMP_DATA];   //dynamic viscosity depending on temperature
        cp = new double [TEMP_DATA];   //specific heat depending on temperature
        wsum = new double [TEMP_DATA]; //molar mass depending on temperature
        pr = new double [TEMP_DATA];   //number of Prandtle depending on temperature

        for (size_t i = 0; i < PRES_DATA; ++i)  //read PRES_DATA rows from standard input
        {
            input >> p[i] >> t0[i];
            if (!input) throw Invalid {};       //if can't read generate exception
        }
        for (size_t i = 0; i < TEMP_DATA; ++i)  //read TEMP_DATA rows from standard input
        {
            input >> t[i] >> mu[i] >> cp[i] >> wsum[i] >> pr[i];
            if (!input) throw Invalid {};       //if can't read generate exception
        }
    }
    catch (Invalid)
    {
        _CleanUp();  	//free memory
        throw;
    }
    catch (...)
    {
        _CleanUp();     //free memory
        throw;
    }
}

void fluid::_CleanUp()  //free memory
{
    delete [] pr;
    delete [] wsum;
    delete [] cp;
	delete [] mu;
    delete [] t;
    delete [] t0;
    delete [] p;
}
