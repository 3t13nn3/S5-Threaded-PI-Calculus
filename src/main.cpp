//ETIENNE (づ｡◕‿‿◕｡)づ PENAULT

#include <iostream>
#include <thread>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>

#include <gmpxx.h>

void bailey_borwein_plouffe(int, std::vector<mpf_class> *, int, int);
void pi_to_file(int, int, mpf_class const&);


int main(int argc, char ** argv){
	/*INIT ARG*/
	if(argc!= 3){
		std::cout << "(づ｡◕‿‿◕｡)づ Usage: ./pi number_of_decimals number_of_threads." << std::endl;
		return -1;
	}

	/*RECOVERING ARGS*/
	int number_of_decimals = atoi(argv[1]) + 1; //+1 to get the real numbers of decimals
	int number_of_threads = atoi(argv[2]);

	if(number_of_decimals == 0 || number_of_threads == 0){
		std::cout << "Can't select 0." << std::endl;
		return -1;
	}

	/*Init the variable which will stock our Π value*/
	mpf_class pi(0,4*number_of_decimals);

	/*
	Intialize some vectors we will need for stocking:
		-Threads
		-The result of each thread for Π
	*/
  	std::vector<std::thread> threads;
  	std::vector<mpf_class> results(number_of_threads,mpf_class(0,4*number_of_decimals));

	/*Threading results' calculus by using our previous pow part vector*/
	for(int i = 0; i< number_of_threads; ++i){
		threads.emplace_back(bailey_borwein_plouffe,number_of_decimals,&results, i, number_of_threads);
	}

	for(int i = 0; i< number_of_threads; ++i){
		threads[i].join();
	}

	/*Adding each part of result to our final Π value*/
	for(int i = 0; i< number_of_threads; ++i){
		pi += results[i];
	}

	/*Writting our result to a file*/
	pi_to_file(number_of_threads, number_of_decimals, pi);

  	return 0;
}

/*
Applying/Calculating BBP Formula (งツ)ว.

For using this function with many threads, it uses a principle of a different offset for each thread.
Each thread will be moving forward of the number of thread that we use which is our offset.

Here, we are calculating a result for each thread (the thread number is the "start" variable).
It is safe-guaranteed by using the thread number as the index we use for stocking the result into our results' vector (pi_result) beacuse each thread will never browse the same indexes.
By this method, each thread will access to the same vector (the vector where we store our thread results), but at different indexes.

In fact, we do a result for each thread for avoiding datarace.
Indeed, if we are adding something to our single result variable, many threads may be modifying the variable value at the same time and will distort the result value.

Then after filling our result with this function, we have to add every part of this vector to a value to get the final result (last instruction in the main) (ᵔᴥᵔ).
*/
void bailey_borwein_plouffe(int nb_limit, std::vector<mpf_class> * pi_result, int start, int nb_threads){
	for(int k(start); k< nb_limit; k+=nb_threads){

		/*Poow part calcul of each decimal*/
		mpf_t tmp;
		mpf_init2(tmp, k);
		mpf_set_d(tmp, 1/16.0);
		mpf_pow_ui(tmp, tmp, k);
		mpf_class p(tmp, k);

		(*pi_result)[start] += (p) * 
						((mpf_class(4)/((mpf_class(8)*mpf_class(k))+mpf_class(1))) -
						 (mpf_class(2)/((mpf_class(8)*k)           +mpf_class(4))) -
						 (mpf_class(1)/((mpf_class(8)*mpf_class(k))+mpf_class(5))) -
						 (mpf_class(1)/((mpf_class(8)*mpf_class(k))+mpf_class(6))));
	}
}

/*Writing a number in a file*/
void pi_to_file(int nb_thread, int nb_decimal, mpf_class const& pi){
	std::ofstream file;
	file = std::ofstream(std::to_string(nb_thread) + "_threads_" + std::to_string(nb_decimal-1) + "_decimals.txt");
	file << std::setprecision(nb_decimal) << pi;
}
