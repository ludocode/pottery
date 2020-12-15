@echo off

REM This script is run by the continuous integration server to build and run
REM the Pottery unit test suite with MSVC on Windows.

REM Run the "more" variant of unit tests
call test\unit.bat more
