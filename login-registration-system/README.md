# Login and Registration System (C++)

A console program that lets users register and log in, with passwords hashed
and stored in a text file. Built as a CodeAlpha C++ task.

## Features

- Register with a username and password
- Login with existing credentials
- Masked password entry (shows `*` while typing, backspace supported)
- Input validation: username at least 3 characters with no spaces,
  password at least 8 characters
- Usernames are case-insensitive and must be unique
- Passwords are stored as hashes, never as plain text

## Build and run (Windows)

```
g++ main.cpp -o login.exe
login.exe
```

Registered users are saved to `database.txt` in the folder where you run the
program. The file is created automatically and is excluded from the repo.

## Notes

- Uses `<conio.h>` (`_getch()`), so it builds on **Windows only**.
- Passwords are hashed with the djb2 algorithm without a salt. This is fine
  for learning but is **not secure** for real systems, which should use a
  salted, slow hash such as bcrypt or Argon2.

## Possible improvements

- Salted, stronger password hashing
- Cross-platform masked input
- Account lockout after repeated failed logins
- Password change and account deletion

## Author

Emmanuel
