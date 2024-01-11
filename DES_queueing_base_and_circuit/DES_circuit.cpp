/*
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Author: Nikhil Khatu
Course: CSC777- Telecommunication Network Design
Professor: Rudra Dutta
Assignment: Homework #2- Problem#3
To compile:
gcc 3_main.cpp -lstdc++ -o numthree
./numthree
*/

#include <iostream>
//#include <random>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string>
#include <math.h>
#include <fstream>

using namespace std;

#define initial_time 0.0
#define simulation_time 86400.0
#define hop_time 0.1

//#define lambda (1/200)
#define mu 0.0055555555
#define Buffer_Size 1000


struct circuit{
    bool vacant[50];
    double End_hold_time[50];
};




int main()
{
    double simulation_clock;
    long double Arrival_time, Exponential_RV;
    double lambda;
    long double blocking_rate;
    int drop_count, connection_count, loop, total_arrivals;

    bool drop_flag;
    struct circuit sim_circuit;


    lambda = 2.61;

    srand (time(NULL));

    drop_count = 0;
    connection_count = 0;
    total_arrivals = 0;

    for(int i; i<50;i++) sim_circuit.vacant[i] = 1;


    Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
    Exponential_RV = -(log(Exponential_RV)/lambda);
    Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
    Arrival_time = simulation_clock + Exponential_RV;

// Begin Simulation
    simulation_clock = initial_time;
    while(simulation_clock <= simulation_time)
        {

            if(Arrival_time - simulation_clock < hop_time)
            {
                drop_flag = 1;
                for(loop=0;loop<50;loop++)
                {


                    if(sim_circuit.vacant[loop])
                        {
                            Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
                            Exponential_RV = -(log(Exponential_RV)/mu);
                            Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
                            sim_circuit.vacant[loop] = 0;
                            sim_circuit.End_hold_time[loop] = simulation_clock + Exponential_RV;

                            drop_flag = 0;
                            connection_count++;
                            break;

                        }
                }
                if(drop_flag) drop_count++;
                total_arrivals++;

                Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
                Exponential_RV = -(log(Exponential_RV)/lambda);
                Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
                Arrival_time = simulation_clock + Exponential_RV;
                //cout<<"Arrival time: "<<Arrival_time<<endl;
            }
// Check each call for ending times
            for(loop=0;loop<50;loop++) if(!sim_circuit.vacant[loop] && (sim_circuit.End_hold_time[loop] - simulation_clock) < hop_time) sim_circuit.vacant[loop] = 1;

            // Increment Simulation Clock
            simulation_clock += hop_time;
    }



for(loop=0;loop<50;loop++)
{
    if(sim_circuit.vacant[loop]) cout<<loop<<"is vacant"<<endl;
    else cout<<loop<<"is occupied"<<endl;
}
cout<<"drop: "<<drop_count<<endl;
cout<<"connect:"<<connection_count<<endl;
cout<<"Total arrivals: "<<total_arrivals<<endl;
blocking_rate =100*(drop_count/(double)total_arrivals);
cout<<"Blocking rate: "<<blocking_rate<<endl;


    return 0;
}
