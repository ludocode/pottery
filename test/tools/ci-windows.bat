@echo off

REM This script is run by the continuous integration server to build and run
REM the Pottery unit test suite with MSVC on Windows.

call test\unit.bat all
