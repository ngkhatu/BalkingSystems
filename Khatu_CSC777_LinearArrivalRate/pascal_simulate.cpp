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
Assignment: Linear Arrival Rate Simulation. This program is threaded for a quadcore processor to run 4x simulations in tandem.
Each simulation runs for 50,000 simulation seconds with simulation precision of 1ms. The simulation is sampled every one simulation second
to check how many busy circuits are in our linear arrival rate system. The sampling is then normalized to get the probability of having
'k' busy circuits in the system at any given point in time.

Directions: Specify up to four alpha/beta combinations in the 'main' routine for scheduling of a simulation thread. Each simulation will output
to a coresponding .dat file labeled using the specified alpha/beta values.

To compile on a linux machine:
gcc pascal_simulate.cpp -pthread -lstdc++ -std=c++0x -o pascal_simulate
./pascal_simulate

*/

#include <sstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> // Pthread library
#include <string>
#include <math.h>
#include <fstream>

#define initial_time 0.0
#define simulation_time 50000.0
#define hop_time 0.001
#define mu 1
#define Buffer_Size 1500
#define NUM_THREADS 4

using namespace std;

struct thread_data{
    int alpha;
    double beta;
    int thread_id;
};
struct thread_data thread_data_array[NUM_THREADS];


struct packet{
    int packet_num;
    double Arrival_time;
    double Begin_service;
    double End_service;
    packet* next;
};


void *simulate(void *threadarg)
{
//-------Thread Arguments---------------------
    struct thread_data *my_data;
    my_data = (struct thread_data*) threadarg;
    int alpha = my_data->alpha;
    double beta = my_data->beta;
    int thread_id = my_data->thread_id;
//------------------------------------------

//--------Output Files---------------------------------------
        string filename, alpha_str, beta_str;
        stringstream ssa, ssb;

        ssa << alpha;
        ssb << beta;
        alpha_str = ssa.str();
        beta_str = ssb.str();

        for(int i=0; i<beta_str.length(); i++)
        {
            if(beta_str[i] == '0') beta_str[i] = 'e';
            if(beta_str[i] == '.') beta_str[i] = 't';
        }

        filename = "SimData_alp" + alpha_str + '_' + 'b' + beta_str + ".dat";
        cout<<"Created:"<<filename<<endl;
        ofstream myfile(filename.c_str());

//-------------------------------------------------------------------------

    double lambda, mu_k;
    long int k_summation_normalize = 0;


    int TotalPackets = 0, i = 1;
    int QueuePackets = 0;
    double TotalResponseTime = 0.0;
    double simulation_clock = initial_time;
    double Arrival_time, Service_time, Exponential_RV;

//----------------------------------------------------
    int k_array[Buffer_Size];
    for(int j=0; j<Buffer_Size;j++) k_array[j] = 0;

    // Statistic Variables
    double Total_Waiting_time = 0.0;
    double Average_Waiting_time;
    int num_packets_started_processing = 0;

    double Total_Service_time = 0.0;
    double Average_Service_time;
    int TotalServiced = 0;

    double Total_System_time = 0.0;
    double Average_System_time;

    int Summation_of_num_packets_in_queue = 0;
    double Average_packets_in_queue = 0.0;
/*
    ofstream myfile1 ("Average_waiting_time.txt");
    ofstream myfile2 ("Average_service_time.txt");
    ofstream myfile3 ("Average_system_time.txt");
    ofstream myfile4 ("Average_num_packets_in_queue.txt");
    ofstream myfile5 ("num_of_packets_in_queue.txt");
    */
//--------------------------------------------------


    // Struct pointers
    struct packet* Buffer_head;
    struct packet* Buffer_tmp;
    struct packet* Processing_packet;

    srand (time(NULL));

    lambda = alpha - (alpha * beta);
    Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
    Exponential_RV = -(log(Exponential_RV)/lambda);
    Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
    Arrival_time = simulation_clock + Exponential_RV;
    Arrival_time = static_cast<float>( static_cast<int>(Arrival_time*1000) ) / 1000;


    bool processing_flag = 0;

// Begin Simulation
    while(simulation_clock <= simulation_time)
        {

//Begin Arrival logic
            if(Arrival_time - simulation_clock < 0.0009) //Arrival_time == simulation_clock){
                {
                    // Generate a random exponential RV for Arrival
                    /*if(processing_flag) lambda = alpha + (((QueuePackets + 2) - alpha)*beta);// 2 = procesor packet + newly arrived
                    else lambda = alpha + (((QueuePackets + 1) - alpha)*beta);*/
                    lambda = alpha + ((QueuePackets - alpha)*beta);
                    Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
                    Exponential_RV = -(log(Exponential_RV)/lambda);
                    Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
                    Arrival_time = simulation_clock + Exponential_RV;


                    TotalPackets++;

                    // Check for buffer overflow
                    if(QueuePackets < Buffer_Size)
                    {
                        //Queue packet into Linked List here
                        if(QueuePackets == 0)
                        {
                            Buffer_head = new packet;
                            Buffer_head->next = NULL;
                            Buffer_head->Arrival_time = Arrival_time;
                            Buffer_head->Begin_service = NULL;
                            Buffer_head->End_service = NULL;
                            Buffer_head->packet_num = i;
                        }
                        else
                        {
                            Buffer_tmp = Buffer_head;
                            Buffer_head = new packet;
                            Buffer_head->Arrival_time = Arrival_time;
                            Buffer_head->Begin_service = NULL;
                            Buffer_head->End_service = NULL;
                            Buffer_head->next = Buffer_tmp;
                            Buffer_head->packet_num = i;
                        }
                        i++;
                        QueuePackets++;
                    }
                }

// Begin Processing logic

            // Packet has finished processing
            if((Service_time - simulation_clock < 0.0009) && processing_flag)
            {
                processing_flag = 0;
                Processing_packet->End_service = simulation_clock;
                //Calculations here

Total_Service_time += (Processing_packet->End_service - Processing_packet->Begin_service);
Total_System_time += (Processing_packet->End_service - Processing_packet->Arrival_time);
                delete Processing_packet;
                TotalServiced++;
            }


            // Start processing one more packet from queue
            if(QueuePackets != 0 && !processing_flag)
            {
                    // Generate Exponential RV Service time
                    mu_k = QueuePackets * mu;
                    Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
                    Exponential_RV = -(log(Exponential_RV)/mu_k);
                    Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
                    Service_time = simulation_clock + Exponential_RV;

                    // Deterministic Service Time of 20msecs
                    //Service_time = 0.016 + simulation_clock;

                    // Find the Processing Packet
                    Processing_packet = Buffer_head;
                    while(NULL != Processing_packet->next)Processing_packet = Processing_packet->next;

                    //Remove packet from linked list buffer
                    Buffer_tmp = Buffer_head;
                    while(Processing_packet != Buffer_tmp->next && Processing_packet != Buffer_head) Buffer_tmp = Buffer_tmp->next;
                    Buffer_tmp->next = NULL;

                    // Set markers
                    Processing_packet->Begin_service = simulation_clock;
//Stat mark
                    Total_Waiting_time += (Processing_packet->Begin_service - Processing_packet->Arrival_time) ;

num_packets_started_processing++;
                    processing_flag = 1;
                    QueuePackets--;
            }

Average_Waiting_time = Total_Waiting_time / num_packets_started_processing;
Average_Service_time = Total_Service_time / TotalServiced;
Average_System_time = Total_System_time / TotalServiced;
Summation_of_num_packets_in_queue += QueuePackets;
Average_packets_in_queue = (Summation_of_num_packets_in_queue /(simulation_clock/hop_time));
/*
if(myfile1.is_open()) myfile1<<Average_Waiting_time<<endl;
else cout<<"Unable to open file!"<<endl;
if(myfile2.is_open()) myfile2<<Average_Service_time<<endl;
else cout<<"Unable to open file2!"<<endl;
if(myfile3.is_open()) myfile3<<Average_System_time<<endl;
else cout<<"Unable to open file3!"<<endl;
if(myfile4.is_open()) myfile4<<Average_packets_in_queue<<endl;
else cout<<"Unable to open file4!"<<endl;
if(myfile5.is_open()) myfile5<<QueuePackets<<endl;
else cout<<"Unable to open file5!"<<endl;
*/
// Sample every one second of simulation time-> Increment counters
if(fmod(simulation_clock,1.0) < hop_time)
{
    if(processing_flag)
        {
        k_array[QueuePackets + 1] = k_array[QueuePackets + 1] + 1;
        k_summation_normalize += k_array[QueuePackets + 1] + 1;
        }
    else
        {
        k_array[QueuePackets] = k_array[QueuePackets] + 1;
        k_summation_normalize += k_array[QueuePackets] + 1;
        }
}

// Increment Simulation Clock
simulation_clock += hop_time;
} // End Simulation loop




//Print queue for testing
/*Buffer_tmp=Buffer_head;
            while(NULL != Buffer_tmp->next)
            {
                i++;
                cout<<Buffer_tmp->packet_num<<": "<<Buffer_tmp->Arrival_time<<endl;
                Buffer_tmp = Buffer_tmp->next;
            }
cout<<Buffer_tmp->packet_num<<": "<<Buffer_tmp->Arrival_time<<endl;


cout<<"Processing_flag: "<<processing_flag<<endl;
cout << "Total Serviced: "<<TotalServiced<<endl;
cout<<"Total in queue at end: "<<QueuePackets<<endl;
cout << "Total Arrivals: "<<TotalPackets<<endl;
*/

// Print sampled k values to file after normalizing
for(int j=0; j<Buffer_Size;j++)
{
    if(myfile.is_open()) myfile<<k_array[j]/(double)simulation_time<<endl; //Divide by simulation time to normalize
    else cout<<"Unable to open file!"<<endl;
}
cout<<"Alpha="<<alpha<<" Beta="<<beta<<" Final Waiting time Average: "<<Average_Waiting_time<<endl;
cout<<"Alpha="<<alpha<<" Beta="<<beta<<" Final Service time Average: "<<Average_Service_time<<endl;
cout<<"Alpha="<<alpha<<" Beta="<<beta<<" Final System time Average: "<<Average_System_time<<endl;
cout<<"Simulation finished for: alpha="<<alpha<<" beta="<<beta<<endl;
pthread_exit(NULL);
    //return;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    // Simulate in zero thread
    thread_data_array[0].alpha = 150;
    thread_data_array[0].beta = 0.5;
    thread_data_array[0].thread_id=0;
    pthread_create(&threads[0], NULL, simulate, (void *)&thread_data_array[0]);

    // Simulate in first thread
    thread_data_array[1].alpha = 150;
    thread_data_array[1].beta = .75;
    thread_data_array[1].thread_id=1;
    pthread_create(&threads[1], NULL, simulate, (void *)&thread_data_array[1]);

    // Simulate in second thread
    thread_data_array[2].alpha = 150;
    thread_data_array[2].beta = .85 ;
    thread_data_array[2].thread_id=2;
    pthread_create(&threads[2], NULL, simulate, (void *)&thread_data_array[2]);

    // Simulate in third thread
    thread_data_array[3].alpha = 150;
    thread_data_array[3].beta = .99;
    thread_data_array[3].thread_id = 3;
    pthread_create(&threads[3], NULL, simulate, (void *)&thread_data_array[3]);

    pthread_exit(NULL);

    return 0;
}

