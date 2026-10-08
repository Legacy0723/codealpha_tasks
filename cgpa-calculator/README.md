# CGPA Calculator (C++)

A console program that calculates a student's semester GPA and overall CGPA
from letter grades and credit hours. Built as a CodeAlpha C++ task.

## Features

- Input validation for the number of courses, semester number, credit hours,
  and previous CGPA / credit hours
- Letter grades are case-insensitive (`a`, `A`, `b+` all work)
- First semester: CGPA equals GPA
- Later semesters: CGPA combines the new courses with your previous CGPA and
  previous credit hours
- Prints a results table of every course, then the GPA and CGPA

## Grading scale

| Grade | Points |
|-------|--------|
| A  | 4.0 |
| B+ | 3.5 |
| B  | 3.0 |
| C+ | 2.5 |
| C  | 2.0 |
| D+ | 1.5 |
| D  | 1.0 |
| E  | 0.5 |
| F  | 0.0 |

## Formula

```
GPA  = sum(grade points x credit hours) / sum(credit hours)
CGPA = (previous CGPA x previous credit hours + this semester's weighted points)
       / (previous credit hours + this semester's credit hours)
```

## Build and run

```
g++ main.cpp -o cgpa.exe
cgpa.exe
```

Works on any platform with a C++11 compiler (on Linux/macOS, use `-o cgpa`
and run `./cgpa`).

## Author

Owusu Emmanuel
