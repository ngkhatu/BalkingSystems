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
Assignment: Homework #2- Problem#2
To compile:
gcc 2_main.cpp -lstdc++ -o numtwo
./numtwo
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
#define simulation_time 30.0
#define hop_time 0.001

//#define lambda (1/0.02)
#define mu (1/.016)
#define Buffer_Size 1000


struct packet{
    int packet_num;
    double Arrival_time;
    double Begin_service;
    double End_service;
    packet* next;
};









int main()
{
    int TotalPackets = 0, i = 1, loop;
    int QueuePackets = 0;
    double TotalResponseTime = 0.0;
    double simulation_clock = initial_time;
    double Arrival_time, Service_time, Exponential_RV;
    double lambda;
    bool processing_flag = 0;


//----------------------------------------------------
    // Statistic Vairables
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

    //ofstream myfile1 ("Average_waiting_time.txt");
   // ofstream myfile2 ("Average_service_time.txt");
   // ofstream myfile3 ("Average_system_time.txt");
   // ofstream myfile4 ("Average_num_packets_in_queue.txt");
   // ofstream myfile5 ("num_of_packets_in_queue.txt");

   ofstream myfile1 ("sixteen.txt");
   ofstream myfile2 ("seventeen.txt");
   ofstream myfile3 ("eighteen.txt");
   ofstream myfile4 ("nineteen.txt");
   ofstream myfile5 ("twenty.txt");
   ofstream myfile6 ("twentyone.txt");
   ofstream myfile7 ("twentytwo.txt");
   ofstream myfile8 ("twentythree.txt");
   ofstream myfile9 ("twentyfour.txt");
   ofstream myfile10 ("twentyfive.txt");



    // Struct pointers
    struct packet* Buffer_head;
    struct packet* Buffer_tmp;
    struct packet* Processing_packet;

for(loop=0; loop<10; loop++)
{
    TotalPackets = 0;
    i = 1;
    QueuePackets = 0;
    TotalResponseTime = 0.0;
    simulation_clock = initial_time;
//----------------------------------------------------
    // Statistic Vairables
    Total_Waiting_time = 0.0;
    num_packets_started_processing = 0;
    Total_Service_time = 0.0;
    TotalServiced = 0;
    Total_System_time = 0.0;
    Summation_of_num_packets_in_queue = 0;
    Average_packets_in_queue = 0.0;
//--------------------------------------------------

    if(loop==0) lambda = (1/0.016);
    else if(loop==1) lambda = (1/0.017);
    else if(loop==2) lambda = (1/0.018);
    else if(loop==3) lambda = (1/0.019);
    else if(loop==4) lambda = (1/0.020);
    else if(loop==5) lambda = (1/0.021);
    else if(loop==6) lambda = (1/0.022);
    else if(loop==7) lambda = (1/0.023);
    else if(loop==8) lambda = (1/0.024);
    else if(loop==9) lambda = (1/0.025);
    else lambda= (1/0.016);


    srand (time(NULL));

    Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
    Exponential_RV = -(log(Exponential_RV)/lambda);
    Exponential_RV = static_cast<float>( static_cast<int>(Exponential_RV*1000) ) / 1000;
    Arrival_time = simulation_clock + Exponential_RV;
    Arrival_time = static_cast<float>( static_cast<int>(Arrival_time*1000) ) / 1000;

    processing_flag = 0;

// Begin Simulation
    while(simulation_clock <= simulation_time)
        {

//Begin Arrival logic
            if(Arrival_time - simulation_clock < 0.0009) //Arrival_time == simulation_clock){
                {
                    // Generate a random exponential RV for Arrival
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
                    Exponential_RV = - ((double)rand()/(RAND_MAX+1)) ;
                    Exponential_RV = -(log(Exponential_RV)/mu);
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


switch(loop){
    case 0:
        if(myfile1.is_open()) myfile1<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 1:
        if(myfile2.is_open()) myfile2<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 2:
        if(myfile3.is_open()) myfile3<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 3:
        if(myfile4.is_open()) myfile4<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 4:
        if(myfile5.is_open()) myfile5<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 5:
        if(myfile6.is_open()) myfile6<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 6:
        if(myfile7.is_open()) myfile7<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 7:
        if(myfile8.is_open()) myfile8<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 8:
        if(myfile9.is_open()) myfile9<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    case 9:
        if(myfile10.is_open()) myfile10<<Average_Waiting_time<<endl;
        else cout<<"Unable to open file!"<<endl;
        break;
    default:
        break;
}




/*
Average_Service_time = Total_Service_time / TotalServiced;
if(myfile2.is_open()) myfile2<<Average_Service_time<<endl;
else cout<<"Unable to open file2!"<<endl;

Average_System_time = Total_System_time / TotalServiced;
if(myfile3.is_open()) myfile3<<Average_System_time<<endl;
else cout<<"Unable to open file3!"<<endl;

Summation_of_num_packets_in_queue += QueuePackets;
Average_packets_in_queue = (Summation_of_num_packets_in_queue /(simulation_clock/hop_time));
if(myfile4.is_open()) myfile4<<Average_packets_in_queue<<endl;
else cout<<"Unable to open file4!"<<endl;

if(myfile5.is_open()) myfile5<<QueuePackets<<endl;
else cout<<"Unable to open file5!"<<endl;
*/
            // Increment Simulation Clock
            simulation_clock += hop_time;
    }
cout<<"Final Waiting time Average: "<<Average_Waiting_time<<endl;
//cout<<"Final Service time Average: "<<Average_Service_time<<endl;
//cout<<"Final System time Average: "<<Average_System_time<<endl;
}
/*
//Print queue for testing
Buffer_tmp=Buffer_head;
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



    return 0;
}
