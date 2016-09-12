# DPLL_SAT_solver
The program reads a BLIF file that contains a CNF function, and determine its satisfiability by using the DPLL algorithm. The program is written in C++. <br />

# Instructions
Execution Instruction (Windows): <br />
1.	Go to the sub-folder /DPLL/Release which contains the executable file called DPLL.exe. <br />
2.	Open the windows command shell cmd.exe, and cd to the folder contains the DPLL.exe executable file described in step 1. <br />
3.	Type: DPLL.exe <path to the BLIF test file>, you can find some test files in the /test_nodes folder. <br />
4.	The program should start, and shows the major steps it used to find the satisfiability of the CNF function. <br />

Compile Instruction (Visual Studio 2015): <br />
1.	In the Solution Explorer, right click on the DPLL project and select property. <br />
2.	Change the Configuration to Release, Platform to win32. <br />
3.	Under Configuration Properties, go to C/C++ and click Precompiled Headers. Change the option on Precompiled Header to Not Using Precompiled Headers. <br />
4.	Under Configuration Properties, go to C/C++ and click Preprocessor. Add the following to the  Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS <br />
5.	Build the solution under the Release and x86 configuration. <br />

# Author:
Shaoyu Chen

