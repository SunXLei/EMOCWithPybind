# EMOC

EMOC is a multi-objective optimization library written in c++11 which involves some basic evolutionary algorithms (now is 20+). 



## Build

The CMakeLists.txt file is provided to build project.

For **linux**, compile the project with following command:

```c++
mkdir build
cd build
cmake ..
make -j4
```

the compiled dynamic link library EMOC.blablabla.so will appear in build directory. You can use `move` command to rename the file to `EMOC.so`

  

## Usage

There is a simple usage example in example directory, Note the `EMOC.so` is in the same directory with python file.



We first import the library:

```python
import EMOC
```

Create a `EMOCParameters`  object and set the parameters:

```python
parameter = EMOC.EMOCParameters()
parameter.algorithm_name = "MOEAD"
parameter.problem_name = "zdt1"
parameter.population_num = 100
parameter.decision_num = 10
parameter.objective_num = 2
parameter.max_evaluation = 25000
parameter.output_interval = 100
```

You can also define your own problems by implement a class inherited from `EMOC.Problem`:

```python
class MyUF1(EMOC.Problem):
    def __init__(self, dec_num, obj_num):
        super(MyUF1, self).__init__(dec_num,obj_num)
        lower_bound = [-1] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [1] * dec_num

    def CalObj(self, ind):
        x = ind.dec
        temp_obj = [0] * self.obj_num
        sum1 = 0
        count1 = 0
        sum2 = 0
        count2 = 0
        for i in range(2,self.dec_num+1):
            yj = x[i-1] - np.sin(6.0 * np.pi * x[0] + i * np.pi / self.dec_num)
            yj = yj * yj
            if i % 2 == 0:
                sum2 += yj
                count2 = count2 + 1
            else:
                sum1 += yj
                count1 += 1
        temp_obj[0] = x[0] + 2.0 * sum1 / count1
        temp_obj[1] = 1.0 - np.sqrt(x[0]) + 2.0 * sum2 / count2
        ind.obj = temp_obj
```

you must set the decision bound in constructor function and implement the `CalObj` virtual function.

**Note: 1. the bound must be set with an entire list**

**Note: 2. the calculated obj result must be set as a whole too**



When you have implemented your own problem, you can set it simply by:

```python
myProblem = MyUF1(para.decision_num,para.objective_num)
para.SetProblem(myProblem)
```

At last, create EMOCManager instance, set the parameter and run:

```python
EMOCManager = EMOC.EMOCManager()
EMOCManager.SetTaskParameters(para)
EMOCManager.Run()
result = EMOCManager.GetResult()
print("IGD: ",result.igd)
print("HV: ",result.hv)
print("Runtime: ",result.runtime)
print("Population Number: ",result.pop_num)
print("Population Decisions: ",result.pop_decs)
print("Population Objectives: ",result.pop_objs)
```
