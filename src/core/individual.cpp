#include "core/individual.h"

namespace emoc {

	Individual::Individual(int dec_num, int obj_num) :
		rank_(0), fitness_(0.0)
	{
		//dec_test.resize(dec_num);
		//obj_test.resize(obj_num);
		dec = pybind11::array_t<double>(dec_num);
		obj = pybind11::array_t<double>(obj_num);
		dec_ = (double*)dec.request().ptr;
		obj_ = (double*)obj.request().ptr;

		
	}

	Individual::~Individual()
	{
		//delete[] dec_;
		//delete[] obj_;
		dec_ = nullptr;
		obj_ = nullptr;
	}
}


