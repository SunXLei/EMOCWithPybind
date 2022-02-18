#include "metric/igd_plus.h"

#include <fstream>
#include <iostream>
#include <cmath>

#include "core/macro.h"
#include "core/global.h"
#include "core/utility.h"


namespace emoc {

	double CalculateIGDPlus(Individual** pop, int pop_num, int obj_num, std::string& problem_name)
	{
		// load pf data
		int pf_size = 0;
		double** pfdata = nullptr;
		pfdata = LoadPFData(pf_size, obj_num, problem_name);

		// calculate igd value
		double igd_value = 0;
		Individual* temp_ind = nullptr;
		double min_distance = 0, temp_distance = 0;
		double* new_distance = new double[obj_num];

		for (int i = 0; i < pf_size; i++)
		{
			min_distance = EMOC_INF;
			for (int j = 0; j < pop_num; j++)
			{
				temp_distance = 0;
				temp_ind = pop[j];

				for (int k = 0; k < obj_num; k++)
				{
					new_distance[k] = pop[j]->obj_[k] > pfdata[i][k] ? pop[j]->obj_[k] - pfdata[i][k] : 0;
					temp_distance += new_distance[k] * new_distance[k];
				}

				temp_distance = sqrt(temp_distance);
				if (min_distance > temp_distance)
				{
					min_distance = temp_distance;
				}
			}
			igd_value += min_distance;
		}
		igd_value /= pf_size;

		// free pf data memory
		for (int i = 0; i < pf_size; ++i)
			delete[] pfdata[i];
		delete[] pfdata;
		delete[] new_distance;


		return 	igd_value;
	}

}