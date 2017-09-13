#include "nozzle_lib.h"

double Nozzle::Te_avd (const fluid &flow, size_t i) const   //characteristic temperature
{
    return t[i]*(1+mach[i]*mach[i]*pow(flow.get_pr(TFACTOR*t_chamber), 1.0/3)*(flow.get_k()-1)/2);
}

double Nozzle::alfa_avd (const fluid &flow, size_t i) const //the heat transfer coefficient (Avduevskii model)
{
    double tw = TFACTOR*t_chamber;                       //wall temperature
    double mu = flow.get_mu(tw);                  //fluid dynamic viscosity
    double ro_w = p[i]*flow.get_wsum(tw)/(R0*tw); //fluid density
    double Re = ro_w*w[i]*s_ef[i]/mu;             //Reynolds number
    if (Re == 0)                                  //if fluid velocity is equal zero
    {
        err("Reynolds = 0. Convection boundary condition was not generated!");
        return 0;
    }
    double Pr = flow.get_pr(tw);    //number of Prandtle
    double Cpe = flow.get_cp(t[i]); //heat capacity of the gas at the static temperature
    double Cpw = flow.get_cp(tw);   //heat capacity of the gas at the wall temperature

	//number of Stanton:
    double Stanton = 0.0296*pow(Re, -0.2)*pow(Pr, -0.6)*pow((tw*Cpw)/(t_chamber*Cpe), 0.39)*
                     pow((1+mach[i]*mach[i]*pow(flow.get_pr(tw), 1.0/3)*(flow.get_k()-1)/2), 0.11);

	double alfa = Stanton*ro_w*w[i]*Cpw; //heat transfer coefficient
    return alfa;
}

double Nozzle::FA_func (double m) const //Fa(M) - amount of extension of the nozzle
{
    double a = 2/(k+1);                 //a, b - auxiliary variables
    double b = (1 + (k-1)*SQR(m)/2);
    a *= b;
    b = pow(a, (k+1)/(2*(k-1)));
    return (1/m)*b;                     //from the equation linking the amount of extension and Mach number
}

double Nozzle::FA_solve (double a, double b, double fa) const //search the root of the equation on the interval (a, b)
                                                              //by the method of half division
{
    if ((fa - FA_func(a))*(fa - FA_func(b)) > 0) throw Invalid{}; //more then one root or no roots on the interval (a, b)
    double delta = ACCURACY_LEVEL;                                //accuracy
    double c = (a+b)/2;                                           //divide the interval in half
    while (fabs(fa - FA_func(c)) > delta)                         //if difference between values is less then accuracy this is root
    {
        if ((fa - FA_func(c))*(fa - FA_func(a)) < 0) b = c;       //check if the function changes sign on the interval (a, c),
        else a = c;                                               //if not, the new interval is (c, b)
        c = (a+b)/2;                                              //divide the interval in half
    }
    return c;                                                     //return the root
}

double Nozzle::Get_Mach (size_t i) const //determine the Mach number as function of the nozzle extension ratio
{
    if (fa[i] < 1) throw Invalid{};      //if area is less then nozzle throat - error!
    if (fa[i] == 1) return 1;            //nozzle throat
    if (fa[i] > 1)                       //more then nozzle throat
    {
        if (di[i].second == 0) return FA_solve(0, 1, fa[i]);        //subsonic flow
        if (di[i].second == 1) return FA_solve(1, MAX_MACH, fa[i]); //supersonic flow
    }
    throw Invalid{};
}

Nozzle::Nozzle(std::string file_name)
{
    std::ifstream input (GetCurrentDir()+ "\\" + file_name); //open the input data file
    if (!input) throw Invalid {};
    size_t n {0};
    input >> fuel_name; //read flow properties file name

	//pressure in the combustion chamber,
	//nozzle throat diametr,
	//wall and fluid temperature ratio,
	//number of sections:

    input >> p_chamber >> d_min >> TFACTOR >> n;
    if (!input) throw Invalid {};
    fluid fuel(fuel_name);
    k = fuel.get_k();      //adiabatic coefficient
    t_chamber = fuel.get_t0(p_chamber);  //total temperature
    for (size_t section = 0; section < n; ++section)
    {
        double buf_d;     //buffer for diametr value
        int buf_i;        //buffer for flow type flag
        double buf_s_ef;  //buffer for boundary layer length
        input >> buf_d >> buf_i >> buf_s_ef;
        if (!input) throw Invalid {};                    //if can't read, generate extension
        di.push_back(std::make_pair(buf_d, buf_i));      //section diametr and flow type
        s_ef.push_back(buf_s_ef);                        //effective length of the boundary layer
        fa.push_back(pow(di[section].first/d_min, 2));   //amount of extension of the nozzle
        mach.push_back(Get_Mach(section));               //get Mach number
        double gdf = 1 + (k-1)*pow(mach[section], 2)/2;  //gas-dynamic function
        t.push_back(t_chamber/gdf);                      //static temperature
        p.push_back(p_chamber*pow(gdf, -k/(k-1)));       //static pressure
        Rk = fuel.get_RG(t_chamber);                     //gas constant
        a.push_back(sqrt(k*Rk*t[section]));              //local speed of sound
        w.push_back(mach[section]*a[section]);           //velocity in the flow core
        te.push_back(Te_avd(fuel, section));             //characteristic temperature
        alfa.push_back(alfa_avd(fuel, section));         //heat transfer coefficient
    }
    input.close();
}

void Nozzle::output(std::string file_name)
//print results to file_name in csv format
{
    std::ofstream out(GetCurrentDir()+ "\\" + file_name);
    if (!out) throw Invalid{};
    out << "d_min, m: ;" << d_min << '\n' << "p_chamber, Pa: ;" << p_chamber << '\n' << "Fuel: ;" << fuel_name << '\n'
        << "¹;" << "d[i];" << "Flow;" << "S_eff;" << "P_st, Pa;" << "T_st, K;" << "w, m/s;"
        << "Mach;" << "alfa, W/(m^2*K);" << "Te, K;\n";
    for (size_t i = 0; i < di.size(); ++i)
    {
        out << i+1 << ';' << di[i].first << ';' << di[i].second << ';' << s_ef[i]
            << ';' << p[i] << ';' << t[i] << ';' << w[i] << ';' << mach[i] << ';'
            << alfa[i] << ';' << te[i] << ';' << '\n';
    }
    out.close();
}

