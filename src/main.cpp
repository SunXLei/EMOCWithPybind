#include <ctime>
#include <cstdio>
#include <pthread.h>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "core/file.h"
#include "core/global.h"
#include "core/individual.h"
#include "problem/zdt.h"
#include "problem/dtlz.h"
#include "algorithms/moead/moead.h"
#include "algorithms/moead_de/moead_de.h"
#include "algorithms/nsga2/nsga2.h" 
#include "algorithms/ibea/ibea.h"
#include "algorithms/spea2/spea2.h"
#include "algorithms/smsemoa/smsemoa.h"
#include "algorithms/moead_dra/moead_dra.h"
#include "algorithms/moead_frrmab/moead_frrmab.h"
#include "algorithms/hype/hype.h"
#include "metric/hv.h"
#include "metric/igd.h"
#include "metric/gd.h"
#include "metric/spacing.h"
#include "random/random.h"
 
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#if defined(__linux) || defined(linux)
#include <sys/time.h>
#endif

#define THREAD_NUM 8
using emoc::g_GlobalSettingsArray;
using emoc::EMOCParameters;
using emoc::EMOCResult;
using emoc::Global;
using emoc::Individual;
namespace py = pybind11;

struct ThreadParamters
{
	EMOCParameters *para;

	int run_start;
	int run_end;
	int thread_id;
};


void *Work(void *args);
void EMOCMultiThreadTest(EMOCParameters *parameter);
void EMOCSingleThreadTest(EMOCParameters *parameter);



//int main(int argc, char* argv[])
//{	
//	// initilize some bases for random number
//	randomize();
//
//	// initialize parameters
//	emoc::EMOCParameters *parameter = new emoc::EMOCParameters(); // 这里只能用new，因为结构体里面有string，如果malloc的话不会做构造，所以malloc出来string是未初始化的，不能正确使用
//	ParseParamerters(argc, argv, parameter);
//	//ReadParametersFromFile("src/config/config.txt", parameter);
//	parameter->igd_value = (double *)malloc(sizeof(double) * parameter->runs_num);
//	double *runtime = (double *)malloc(sizeof(double) * parameter->runs_num);
//
//	std::cout << "current task:" << std::endl;
//	std::cout << "-------------------------------------------" << std::endl;
//	std::cout << "problem:              " << parameter->problem_name << std::endl;
//	std::cout << "algorithm:            " << parameter->algorithm_name << std::endl;
//	std::cout << "population number:    " << parameter->population_num << std::endl;
//	std::cout << "decision number:      " << parameter->decision_num << std::endl;
//	std::cout << "objective number:     " << parameter->objective_num << std::endl;
//	std::cout << "evaluation:           " << parameter->max_evaluation << std::endl;
//	std::cout << "runs:                 " << parameter->runs_num << std::endl;
//	std::cout << "is open multithread:  " << parameter->is_open_multithread << std::endl;
//	std::cout << "multithread number:   " << parameter->thread_num << std::endl;
//	std::cout << "-------------------------------------------\n" << std::endl;
//
//	clock_t start, end;
//	start = clock();
//
//	// EMOC test run
//	if (parameter->is_open_multithread)
//		EMOCMultiThreadTest(parameter);
//	else
//		EMOCSingleThreadTest(parameter,runtime);
//
//	end = clock();
//	double time = (double)(end - start) / CLOCKS_PER_SEC;
//
//	//printf("------total run time: %fs--------\n\n", time);
//
//	//for (int i = 0; i < parameter->runs_num; ++i)
//	//{
//	//	printf("run %d igd value: %f \n", i, parameter->igd_value[i]);
//	//}
//
//	free(runtime);
//	free(parameter->igd_value);
//	delete parameter;
//
//	std::cout<<"\nTask has finished, please enter to exit."<<std::endl;
//
//	
//	return 0;
//}
//
//void *Work(void *args)
//{
//	ThreadParamters *parameter = (ThreadParamters *)args;
//	const char *algorithm_name = parameter->para->algorithm_name.c_str();
//	const char *problem_name = parameter->para->problem_name.c_str();
//
//	int population_num = parameter->para->population_num;
//	int dec_num = parameter->para->decision_num;
//	int obj_num = parameter->para->objective_num;
//	int max_eval = parameter->para->max_evaluation;
//	int output_interval = parameter->para->output_interval;
//
//	for (int run = parameter->run_start; run <= parameter->run_end; ++run)
//	{
//		int thread_id = parameter->thread_id;
//
//		// algorithm main entity
//		g_GlobalSettingsArray[thread_id] = new emoc::Global(algorithm_name, problem_name, population_num, dec_num, obj_num, max_eval, thread_id, output_interval, run);
//		g_GlobalSettingsArray[thread_id]->Start();
//
//
//		std::string problem_name = g_GlobalSettingsArray[thread_id]->problem_name_;
//		int obj_num = g_GlobalSettingsArray[thread_id]->obj_num_;
//		double igd = emoc::CalculateIGD(g_GlobalSettingsArray[thread_id]->parent_population_.data(), g_GlobalSettingsArray[thread_id]->population_num_, obj_num, problem_name);
//
//		printf("current thread id : %d, runs: %d, igd:%f\n", thread_id, run, igd);
//		parameter->para->igd_value[run] = igd;
//
//
//		//RecordPop(run, 0, g_GlobalSettingsArray[thread_id]);
//
//
//
//
//		delete g_GlobalSettingsArray[thread_id];
//	}
//
//	return nullptr;
//}
//
//void EMOCMultiThreadTest(EMOCParameters *parameter)
//{
//	int thread_num = parameter->thread_num;
//	std::vector<ThreadParamters*> thread_para_array(thread_num, nullptr);
//	for (int i = 0; i < thread_num; ++i)
//	{
//		thread_para_array[i] = (ThreadParamters *)malloc(sizeof(ThreadParamters));
//		thread_para_array[i]->para = parameter;
//	}
//
//	std::vector<int> job_overload(thread_num, 0);
//	int interval = (double)parameter->runs_num / thread_num;
//	int remainder = parameter->runs_num % thread_num;
//	for (int i = 0; i < thread_num; ++i)
//	{
//		job_overload[i] = interval;
//		if (remainder-- > 0)
//			job_overload[i]++;
//		//printf("thread %d: %d runs\n",i, job_overload[i]);
//	}
//
//	// multithread running
//	std::vector<pthread_t> tid(thread_num);
//	int total_overload = 0;
//	for (int i = 0; i < thread_num; ++i)
//	{
//		if (job_overload[i] > 0)
//		{
//			thread_para_array[i]->run_start = total_overload;
//			thread_para_array[i]->run_end = total_overload + job_overload[i] - 1;
//			thread_para_array[i]->thread_id = i;
//			total_overload += job_overload[i];
//		}
//		else
//			continue;
//
//		pthread_create(&tid[i], nullptr, Work, (void *)thread_para_array[i]);
//	}
//
//	for (int i = 0; i < thread_num; ++i)
//	{
//		if (job_overload[i] > 0)
//			pthread_join(tid[i], nullptr);
//	}
//
//
//	for (int i = 0; i < thread_num; ++i)
//		free(thread_para_array[i]);
//}

EMOCResult EMOCTestRun(EMOCParameters *parameter)
{
	std::cout << "EMOC Run Start\n";
	const char *algorithm_name = parameter->algorithm_name.c_str();
	const char *problem_name = parameter->problem_name.c_str();

	int population_num = parameter->population_num;
	int dec_num = parameter->decision_num;
	int obj_num = parameter->objective_num;
	int max_eval = parameter->max_evaluation;
	int output_interval = parameter->output_interval;
	pybind11::array_t<double>& initial_pop = parameter->initial_pop;

	// temp
	randomize();
	parameter->igd_value = (double*)malloc(sizeof(double) * parameter->runs_num);
	

	EMOCResult result;

	for (int run = 0; run < parameter->runs_num; ++run)
	{
		int thread_id = 0;
		//run time recording
		clock_t start, end;
		start = clock();

		// algorithm main entity
		g_GlobalSettingsArray[thread_id] = new emoc::Global(algorithm_name, problem_name, population_num, dec_num, obj_num, max_eval, thread_id, output_interval, run);
		
		// check setting problem and Init, Init must behind the SetProblem
		if (parameter->problem)	g_GlobalSettingsArray[thread_id]->SetProblem(parameter->problem);
		g_GlobalSettingsArray[thread_id]->Init();
		
		// check setting initial population
		if (initial_pop.size() > 0)	g_GlobalSettingsArray[thread_id]->SetInitialPop(initial_pop);

		g_GlobalSettingsArray[thread_id]->Start();

		end = clock();
		double time = (double)(end - start) / CLOCKS_PER_SEC;

		//std::string problem_name = g_GlobalSettingsArray[thread_id]->problem_name_;
		//int obj_num = g_GlobalSettingsArray[thread_id]->obj_num_;
		//double igd = emoc::CalculateIGD(g_GlobalSettingsArray[thread_id]->parent_population_.data(), g_GlobalSettingsArray[thread_id]->population_num_, obj_num, problem_name);
		//double gd = emoc::CalculateGD(g_GlobalSettingsArray[thread_id]->parent_population_.data(), g_GlobalSettingsArray[thread_id]->population_num_, obj_num, problem_name);
		//double hv = emoc::CalculateHV(g_GlobalSettingsArray[thread_id]->parent_population_.data(), g_GlobalSettingsArray[thread_id]->population_num_, obj_num);
		//double spacing = emoc::CalculateSpacing(g_GlobalSettingsArray[thread_id]->parent_population_.data(), g_GlobalSettingsArray[thread_id]->population_num_, obj_num);
		//std::cout << "IGD Value:" << igd << "\n";
		printf("[%dth run] runtime: %fs\n", run, time);

		// collect results
		result.pop_num = g_GlobalSettingsArray[thread_id]->algorithm_->real_popnum_;
		for (int j = 0; j < result.pop_num; j++)
		{
			result.dec.push_back(std::vector<double>(dec_num));
			result.obj.push_back(std::vector<double>(obj_num));
			for (int k = 0; k < dec_num; k++)
			{
				result.dec[j][k] = g_GlobalSettingsArray[thread_id]->parent_population_[j]->dec_[k];
			}

			for (int k = 0; k < obj_num; k++)
			{
				result.obj[j][k] = g_GlobalSettingsArray[thread_id]->parent_population_[j]->obj_[k];
			}
		}

		delete g_GlobalSettingsArray[thread_id];
	}



	printf("EMOC Run End\n");

	return result;
}

namespace emoc {

	// for polymorphism of Problem Class in python
	class PyProblem : public Problem
	{
	public:
		using Problem::Problem;

		/* Trampoline (need one for each virtual function) */
		void CalObj(Individual* ind) override {
			PYBIND11_OVERRIDE_PURE(
				void, /* Return type */
				Problem,      /* Parent class */
				CalObj,          /* Name of function in C++ (must match Python name) */
				ind      /* Argument(s) */
			);
		}

	};
}

PYBIND11_MODULE(EMOC, m) {
	// EMOC Settings Class
	py::class_<EMOCParameters>(m, "EMOCParameters")
		.def(py::init<>())
		.def_readwrite("algorithm_name", &EMOCParameters::algorithm_name)
		.def_readwrite("problem_name", &EMOCParameters::problem_name)
		.def_readwrite("population_num", &EMOCParameters::population_num)
		.def_readwrite("decision_num", &EMOCParameters::decision_num)
		.def_readwrite("objective_num", &EMOCParameters::objective_num)
		.def_readwrite("max_evaluation", &EMOCParameters::max_evaluation)
		.def_readwrite("output_interval", &EMOCParameters::output_interval)
		.def_readwrite("problem", &EMOCParameters::problem)
		.def_readwrite("initial_pop", &EMOCParameters::initial_pop);
		//.def_readwrite("is_open_multithread", &EMOCParameters::is_open_multithread)
		//.def_readwrite("thread_num", &EMOCParameters::thread_num)
		//.def_readwrite("igd_value", &EMOCParameters::igd_value)
		 
	// EMOC Result Class
	py::class_<EMOCResult>(m, "EMOCResult")
		.def(py::init<>())
		.def_readwrite("dec", &EMOCResult::dec)
		.def_readwrite("obj", &EMOCResult::obj)
		.def_readwrite("pop_num", &EMOCResult::pop_num);

	m.def("EMOCTestRun", &EMOCTestRun);

	// define Individual class for problem's CalObj function parameters
	py::class_<Individual>(m, "Individual")
		.def(py::init<int, int>())
		.def_readwrite("dec", &Individual::dec)
		.def_readwrite("obj", &Individual::obj);

	// define Problem base class, so we can write custom problem derived from this class in python
	py::class_<emoc::Problem, emoc::PyProblem /* <--- trampoline*/>(m, "Problem")
		.def(py::init<int, int>())
		.def("CalObj", &emoc::Problem::CalObj)
		.def_readwrite("dec_num", &emoc::Problem::dec_num_)
		.def_readwrite("obj_num", &emoc::Problem::obj_num_)
		.def_readwrite("lower_bound", &emoc::Problem::lower_bound_)
		.def_readwrite("upper_bound", &emoc::Problem::upper_bound_);

}