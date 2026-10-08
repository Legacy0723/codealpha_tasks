# Banking System (C++)

A console-based banking application written in C++ with separate
customer and staff roles and file-based storage.

## Features

**Customer**
- View account details and recent transactions
- Deposit, withdraw, and transfer between accounts
- Change PIN

**Staff**
- Create and update customer profiles
- Open and close accounts
- Reset customer PINs
- View all customers and accounts
- Change the staff key

**General**
- Masked PIN / staff key entry
- Input validation (4-digit PINs, 10-digit phone numbers, numeric amounts)
- Data saved to text files and reloaded on startup

## Build and run (Windows)

```
g++ main.cpp -o bank.exe
bank.exe
```

On first run you will be asked to create a staff key.

## Notes

- Uses `<conio.h>` (`_getch()`), so it builds on **Windows only**.
- PINs and the staff key are stored as FNV-1a hashes. This is fine for a
  learning project but is **not secure** enough for real banking use.
- Data files (`customers.txt`, `accounts.txt`, `transactions.txt`,
  `staff.key`) are created at runtime and are excluded from the repo.

## Possible improvements

- Cross-platform masked input
- Salted, stronger password hashing
- Unit tests
- Splitting the code into separate header and source files

## Author

Emmanuel
