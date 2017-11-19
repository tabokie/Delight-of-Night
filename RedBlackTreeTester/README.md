# Tester for red-black tree

=============

**Overall Intro**

Execute the testing BAT file to run efficiency/special/customized case testing.

=NOTICE=: the program for testing MUST receive input as assigned format(N0\n Ni{ni}\n), or unexpected consequences may come.

=WARNING=: It's not recommended to check through sub-folders in this directory. The program scripts lack maintenence and instructions are too complex.
	If any executable file goes missing, please rebuild it carefully with instructions.
	//originally built with gcc version 4.9.2


**Pseudocode**

-------------------------------------------------------------------------------
//"Sample Test" is the main engine to drive tests, 
//receives data stream
//and outputs test result and time log.

>>>Function SampleTest(Samples):
	read Samples
	for i<-1 to size(Samples):
		for j<-0 to iterationTime:
			call CheckingFunction(sample)
		log time duration
-------------------------------------------------------------------------------

//"Special Test" is A special container for special designed case.
//reads designed cases in file, 
//and outputs figure and hand cases to SampleTest Module for further test.

>>>Function SpecialTest:
	Read Special Samples
	Draw Sample Figure
	call SampleTest(Samples)
-------------------------------------------------------------------------------

//"Efficiency Test" is Designed for efficiency test.
//Can generate large size of red-black tree samples 
//and hand to SampleTest Module.

>>>Function EfficiencyTest(number,size):
	for i<-1 to number:
		call randomNumberGenerator(size)
		call buildRed_blackTree(randomNumbers)
		call SampleTest(red_blackTree)
-------------------------------------------------------------------------------

//"Console Test" is For user's test.
//NOTICE: Input must follow formula {N0\n Ni{ni}\n}, but not {N\n Ni\n Nij\n}.

>>>Function ConsoleTest(input):
	call SampleTest(input)

-------------------------------------------------------------------------------
