#pragma once
#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>


namespace emoc {

	// Class for each solution in optimization process, and
	// it holds some basic datas of each solution.
	class Individual
	{
	public:
		Individual(int dec_num, int obj_num);
		//Individual(const Individual& ind);
		~Individual();

	public:
		int rank_;
		double fitness_;

		pybind11::array_t<double> dec;
		pybind11::array_t<double> obj;
		double* dec_;
		double* obj_;

		// 用vector不行，只是单纯的内存拷贝，python里面改不了它的值，必须得用array_t
		//std::vector<double> dec_test;
		//std::vector<double> obj_test;

	};

}