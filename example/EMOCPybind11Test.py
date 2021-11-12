import sys
import numpy as np
import functools
import operator
sys.path.append("./")
import EMOC

parameter = EMOC.EMOCParameters()
parameter.algorithm_name = "MOEAD"
parameter.problem_name = "zdt1"
parameter.population_num = 100
parameter.decision_num = 10
parameter.objective_num = 2
parameter.max_evaluation = 25000
parameter.output_interval = 100

# help(EMOC.EMOCTestRun)
class MyProblem(EMOC.Problem):
    def __init__(self, dec_num, obj_num):
        super(MyProblem, self).__init__(dec_num,obj_num)
        lower_bound = [0] * dec_num
        lower_bound[3] = -1
        self.lower_bound = lower_bound
        self.upper_bound = [1] * dec_num
        self.count = 0

    def CalObj(self, ind):
        self.count = self.count + 1
        x = ind.dec
        obj = ind.obj
        # print("here\n")
        k = self.dec_num-self.obj_num+1
        sum1 = 0
        for i in range(k):
            sum1+= (x[self.obj_num-1+i]-0.5)**2 - np.cos(20*np.pi*(x[self.obj_num-1+i]-0.5))
        g = 100*(k+sum1)

        for i in range(self.obj_num):
            ind.obj[i] = 0.5*(1.0+g)

        for i in range(self.obj_num):
            ind.obj[i] *= functools.reduce(operator.mul,
                                       [x for x in x[:self.obj_num-i-1]],1
                                       )
            if i>0:
                ind.obj[i]*= 1- x[self.obj_num-i-1]       # dec = ind.dec_test


class MyUF1(EMOC.Problem):
    def __init__(self, dec_num, obj_num):
        super(MyUF1, self).__init__(dec_num,obj_num)
        lower_bound = [-1] * dec_num
        lower_bound[0] = 0
        self.lower_bound = lower_bound
        self.upper_bound = [1] * dec_num

    def CalObj(self, ind):
        x = ind.dec
        obj = ind.obj
        # print("here\n")
        k = self.dec_num-self.obj_num+1
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

        obj[0] = x[0] + 2.0 * sum1 / count1
        obj[1] = 1.0 - np.sqrt(x[0]) + 2.0 * sum2 / count2


# 设置问题
myProblem = MyUF1(parameter.decision_num,parameter.objective_num)
# parameter.problem = myProblem

result = EMOC.EMOCTestRun(parameter)
print(result.dec)
print(result.obj)
print(result.pop_num)

