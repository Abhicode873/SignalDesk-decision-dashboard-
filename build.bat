@echo off
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o signaldesk.exe
if errorlevel 1 (
  echo Build failed. Ensure MinGW/G++ is installed and on PATH.
  pause
  exit /b 1
)
echo Build successful: signaldesk.exe
