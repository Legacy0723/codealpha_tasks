# CodeAlpha Tasks (C++ Programming)

Task submissions for the CodeAlpha C++ Programming internship, September 2026
batch. All programs are console applications written in C++.

| Task | Project | Description | Platform |
|------|---------|-------------|----------|
| 1 | [cgpa-calculator](cgpa-calculator/) | Calculates semester GPA and overall CGPA from letter grades and credit hours | Any |
| 2 | [login-registration-system](login-registration-system/) | User registration and login with masked, hashed passwords | Windows |
| 4 | [bank-system](bank-system/) | Banking app with customer and staff roles, accounts, transfers, PIN management and file storage | Windows |

Each folder has its own README with build instructions.

Note: the bank system is assigned Task 4 in the task list. It is submitted in
the third slot of the submission form, which only has Task 1 to 3 fields.

## Build

Every project is a single file:

```
g++ main.cpp -o program.exe
```

## Notes

- `bank-system` and `login-registration-system` use `<conio.h>`, so they
  build on Windows only.
- Password and PIN hashing in these projects is for learning and is not
  secure enough for real systems.
- Data files created at runtime are excluded from the repo by `.gitignore`.

## Author

Emmanuel
