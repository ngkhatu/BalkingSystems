 

#include <iostream>

#include <fstream>

#include <stdlib.h>

using namespace std;

 

void q_and_l( double arr_rate , // Arrival rate

double dep_rate , // Departure rate

int m , // Number of servers

int q , // Buffer size

double& t_wait , // Waiting time "

double& p_balk ); // Prob. of balking "



int main(void)

{

int	 m, b;

double	 a, d, twait , ploss;

ofstream	os( "trace" );

bool	 flag = true;

os	<< "======================================================================"

<< endl << endl;

 

while ( flag ) {

cout << "Enter arrival rate (a) and departure rate (d): " << endl;

cin >> a >> d;

if ( a == 0 && d == 0 ) break;

cout << "Enter # of servers (k) and # of buffers (b): " << endl;

cin >> m >> b;

q_and_l( a , d , m , b , twait , ploss );

 

cout	<< " Arrival rate : " << a << endl

<< " Departure rate : " << d << endl

<< " # of Servers : " << m << endl

<< " # of Buffers : " << b << endl

<< " Average waiting time : " << twait << endl

<< " Average prob. of loss : " << ploss << endl;

os	 << " Arrival rate : " << a << endl

<< " Departure rate : " << d << endl

<< " # of Servers : " << m << endl

<< " # of Buffers : " << b << endl

<< " Average waiting time : " << twait << endl

<< " Average prob. of loss : " << ploss << endl

<< endl

<< "======================================================================"

<< endl << endl;


 return(0);

}

}

 

/* ============ Analysis of an M/M/m/b Queue ============= */

 

void q_and_l( double arr_rate , // Arrival rate

double dep_rate , // Departure rate

int m , // Number of servers

int b , // Buffer size

double& t_wait , // Waiting time "

double& p_balk ) // Prob. of Balking "

{	double sum, u, q_size;

double	*p = (double*) malloc( (m+b+1) * sizeof(double) );

int k;

 

u = arr_rate / dep_rate; // u is the traffic intensity

 

// ... Find the probabilities of k messages in the system

p[0] = 1.0; // First, relative probabilities

for ( k = 1 ; k <= m ; k++ )

p[k] = p[k-1] * u / k;

for ( k = 1 ; k <= b ; k++ )

p[m+k] = p[m+k-1] * u / m;

sum = 0.0; // Then, absolute probabilities

for ( k = 0 ; k <= m+b ; k++ )

sum += p[k];

for ( k = 0 ; k <= m+b ; k++ )

p[k] /= sum;

 

// ... Find the average queue size

q_size = 0.0;

for ( k = 1 ; k <= b ; k++ ) {

q_size += k * p[m+k];

}

 

// ... Find the average waiting time

t_wait = q_size / arr_rate;

 

// ... Find the probability of balking

p_balk = p[m+b];

 

}

 

 
