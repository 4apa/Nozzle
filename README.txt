---------------------------------------------------
--------------------Nozzle-------------------------
---------------------------------------------------

Program for calculation of convective heat transfer 
parametrs in the specified sections of the Laval nozzle

---------------------------------------------------
The structure of the input file (.txt)

<File name of the properties of the fluid>
<Combustion chamber pressure, Pa> <Combustion chamber temperature, K> <Wall and fluid temperature ratio> <Number of sections>
<1-st section diametr, m> <Flow type (1 - supersonic, 0 - subsonic)> <Boundary layer length, m>
... 
<n section diametr> <Flow type (1 - supersonic, 0 - subsonic)> <Boundary layer length, m>

---------------------------------------------------
The fluid properties file must be in the same folder with Nozzle.exe

The structure of the fluid properties file (.txt):

<Average adiabatic coefficient> <Number of pressure values> <Number of temerture values>
<Pressure[0], Pa> <Temperature in the combustion chamber (as function of pressure), K>
...
<Pressure[n], Pa> <Temperature in the combustion chamber (as function of pressure), K>
<Temperature[0], K> <Dynamic viscosity, Pa*s> <Specific heat, Joule> <Molar mass, kg/mol> <Prandtle number> 
...
<Temperature[m], K> <Dynamic viscosity, Pa*s> <Specific heat, Joule> <Molar mass, kg/mol> <Prandtle number>
---------------------------------------------------

Output file (.csv) contains basic flow and heat transfer parametrs
___________________________________________________________________________________________________

Author: A.Chaplygin, alchapl87@gmail.com