@echo off
set PWD=%~sdp0

echo .                                                                                                     .
echo *******************************************************************************************************
echo ///////////////////////////////////////////////////////////////////////////////////////////////////////
echo *******************************************************************************************************
echo This is testing batch, please select testing modes:

echo Special Test: run test with several typical samples designed by the author.
echo Efficiency Test: run automatic test with large samples.(WARNING: this test costs huge amount of time)
echo Console Test: enter your own sample case to test.
echo ...NOTICE: if your enter efficiency test by mistake, feel free to press Ctrl+C to interrupt.
choice /C secq /M ">>>SpecialCase, EfficiencyTest, ConsoleTest, QuitWithoutTest?"
if errorlevel 4 goto end
if errorlevel 3 goto choice_3
if errorlevel 2 goto choice_2
if errorlevel 1 goto choice_1

:choice_1
echo Now special case tesing begins, make sure all custom cases are in place(/specialSample/treeSample.txt).
echo Generating Figures and copying samples...
cd /d %PWD%\specialSample
copy /y treeSample.txt %PWD%\specialSample\treeVisualization\data
copy /y treeSample.txt %PWD%\SampleTest
cd /d %PWD%\specialSample\treeVisualization
start "" "drawTree.exe"
pause
goto test

:choice_2
echo Now auto efficiency test begins.
echo Generating samples...
cd /d %PWD%\efficiencySample
start "" "raw.exe"
pause
start "" "tree.exe"
pause
copy /y treeSample.txt %PWD%\SampleTest
goto test

:test
echo Program begin...
cd ..
cd /d %PWD%\SampleTest
start "" "test_with_file.exe"
goto end_with_test

:choice_3
echo Program begin...
cd ..
cd /d %PWD%\SampleTest
start "" "test_with_console.exe"
goto end

:end_with_test
echo All testing samples executed.
goto end

:end
echo Now process terminated.
pause