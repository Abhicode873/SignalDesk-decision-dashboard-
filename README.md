# SignalDesk — C++ Decision Dashboard

SignalDesk is a lightweight, offline decision-support command-line application built with **C++17**. It helps turn tasks, risks, and opportunities into a ranked list of next actions using a transparent scoring rule.

> The score is a simple heuristic, not an AI prediction. It supports decision-making rather than replacing human judgement.

## Features

- **Dashboard:** counts signals by priority and highlights the highest-scoring next action.
- **Priority scoring:** combines impact, urgency, and confidence.
- **Create signals:** record a title, category, impact, urgency, confidence, and next step.
- **Search:** find signals by title, category, or next step.
- **Local persistence:** save and reload signals in `signals.csv`.
- **Text report:** export a ranked summary to `signal_report.txt`.
- **Demo mode:** display sample data without needing to enter anything.
- **Small and portable:** one main C++ source file, no external libraries, database, or network connection.

## Requirements

- A C++17 compiler such as GCC/MinGW-w64 or Clang
- Optional: CMake 3.16+ and the VS Code CMake Tools extension

## Build and run

### Windows (PowerShell / VS Code)

Open a terminal in the directory containing `main.cpp`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o signaldesk.exe
.\\signaldesk.exe --demo
```

For interactive mode:

```powershell
.\\signaldesk.exe
```

### Linux / macOS

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o signaldesk
./signaldesk --demo
```

### CMake

```bash
cmake -S . -B build
cmake --build build
```

## How the score works

```text
score = impact × 9 + urgency × 8 + confidence × 3
```

Each input is rated from 1 to 5. The score ranges from 20 to 100.

| Score | Priority |
|---:|---|
| 75–100 | Critical |
| 58–74 | High |
| 40–57 | Medium |
| 20–39 | Low |

## Project structure

```text
SignalDesk/
├── .github/workflows/cpp-build.yml
├── .vscode/tasks.json
├── CMakeLists.txt
├── README.md
├── .gitignore
├── build.bat
├── main.cpp
└── run_demo.bat
```

## Runtime files

- `signals.csv` — saved signal data.
- `signal_report.txt` — exported priority report.

These are local runtime files and should not be committed.

## Continuous integration

GitHub Actions can build the project with CMake on Windows, Ubuntu, and macOS when code is pushed or a pull request is opened.

## Current scope

SignalDesk is a console application. It does not currently provide a graphical web dashboard, AI model integration, cloud synchronization, or multi-user storage.

## License

No license has been selected yet. Unless a license is added, standard copyright restrictions apply to reuse and redistribution.
