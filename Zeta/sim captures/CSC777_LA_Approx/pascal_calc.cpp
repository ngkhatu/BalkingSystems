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


/************************
Linear Arrival Rate Approximation
Author: Nikhil Khatu
Course: CSC777- Telecommunication Network Design
Professor: Rudra Dutta
Assignment: This program uses the GNU Multiple Precision Library to calculate the probability of a Linear Arrival Rate system
having 'k' packets at any given point in time. The probability is calculated with an approximation that is the Pascal(negative binomial) distribution.
********************
First install GMP from the website:
http://gmplib.org/

Then to compile with gcc/g++ and run:
gcc pascal_calc.cpp -lgmp -lstdc++ -o pascal_calc
./pascal_calc
****************************/

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <gmp.h> // Include GNU Multiple Precision Library
//#include <gmpxx.h>




#define mu 1
#define f_precision 2000 // Specified in bits
#define print_console 0
#define print_to_file 1

using namespace std;
mpf_t left_side_gmp;
mpf_t factorials_divided;


void calculate_right_side(double a_var, int k)
{
// Declare local GMP variables
mpf_t tmp_upper, tmp_upper2, upper_mult, k_factorial_fp;
mpz_t k_factorial;
// Initialize local GMP variables
mpf_init2(tmp_upper, f_precision);
mpf_init2(tmp_upper2, f_precision);
mpf_init2(upper_mult, f_precision);
mpf_init2(k_factorial_fp, f_precision);
mpz_init(k_factorial);

// Upper Values
    mpf_set_d(tmp_upper, 1.0);
    for(int k_iter=0; k_iter<=(k-1); k_iter++)
        {
            mpf_set_d(tmp_upper2, (a_var+k_iter));
            mpf_mul(upper_mult, tmp_upper, tmp_upper2);
            mpf_set(tmp_upper, upper_mult);
        }
// Lower values
        mpz_fac_ui(k_factorial, k); // Factorial of k with gmp int
        mpf_set_z(k_factorial_fp, k_factorial); // To gmp float

// Divide upper final product with with large factorial
        mpf_div(factorials_divided, upper_mult, k_factorial_fp);

// Deallocate local GMP variables
mpf_clear(tmp_upper);
mpf_clear(tmp_upper2);
mpf_clear(upper_mult);
mpz_clear(k_factorial);
mpf_clear(k_factorial_fp);

    return;
}

void calculate_left_side(long double a_var, double beta, int k)
{
    double  bottom_part;
    mpf_t beta_gmp_fp, tmp_gmp_var, beta_k_gmp_fp;

    mpf_init2(tmp_gmp_var, f_precision);
    mpf_init2(beta_k_gmp_fp, f_precision);
    mpf_init2(beta_gmp_fp, f_precision);

    // (1-Beta)^a_var
    bottom_part=pow((1-beta), a_var);
    mpf_set_d(tmp_gmp_var, bottom_part);

    // Beta ^k
    mpf_set_d(beta_gmp_fp, beta);
    mpf_pow_ui(beta_k_gmp_fp, beta_gmp_fp, k);

    // Left Side
    mpf_mul(left_side_gmp, tmp_gmp_var, beta_k_gmp_fp);

    mpf_clear(beta_gmp_fp);
    mpf_clear(tmp_gmp_var);
    mpf_clear(beta_k_gmp_fp);

    return;
}


void run_pascal_calc(unsigned long int alpha, double beta)
{
        long double p_k, sum, a_var;
//-------------------------------------------------------------------------
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

        filename = "Prob_alp" + alpha_str + '_' + 'b' + beta_str + ".dat";
        cout<<filename<<endl;
        ofstream myfile(filename.c_str());

//-------------------------------------------------------------------------






sum = 0;
a_var = (alpha / beta)*(1 - beta);
//cout<<"a_var= "<<a_var<<endl;

for(int k=1; k<1500;k++)
    {
        mpf_t final_prob_k_gmp;

        // Init gmp vars
        mpf_init2(factorials_divided, f_precision);
        mpf_init2(left_side_gmp, f_precision);
        mpf_init2(final_prob_k_gmp, f_precision);

        // Begin Calculating
        calculate_right_side(a_var, k);
        calculate_left_side(a_var, beta, k);
        mpf_mul(final_prob_k_gmp, left_side_gmp, factorials_divided);
        // Return GMP variable with double precision
        p_k = mpf_get_d(final_prob_k_gmp);

        //Deallocate GMP variables
        mpf_clear(final_prob_k_gmp);
        mpf_clear(left_side_gmp);
        mpf_clear(factorials_divided);
        // Final value
        sum += p_k;


// Output to Console
        if(print_console)
               {
                        cout<<"k ="<<k;
                        cout<< " alpha ="<<alpha;
                        cout<<" beta ="<<beta;
                        cout<<" p_k ="<<p_k<<endl;
                    }
                if(print_to_file)
                    {
                        if(myfile.is_open()) myfile<<p_k<<endl;
                        else cout<<"Unable to open file!"<<endl;
                    }
            }
            if(print_console) cout<<"summ: "<<sum<<endl;
// Finished
        myfile.close();

        return;
}

int main()
{

    /********run_pascal_calc(int alpha, double beta)****
    **********where alpha is >= 0 ***********************
    **********and beta is between 0 and 1 ***************
    **************SAMPLE VALUES*************************/
    run_pascal_calc(150, .5);
    run_pascal_calc(150, .75);
    run_pascal_calc(150, .85);
    run_pascal_calc(150, .99);

	return 0;
}
