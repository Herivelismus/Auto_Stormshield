
# Auto Stormshield

A little C program to avoid logging into the Stormshield portal everytime.

## Setup

Download the appropriate version of the program
- [Windows x64](link)
- [MacOS x64](link)
- [MacOS ARM](https://github.com/Herivelismus/Auto_Stormshield/releases/download/v0.1/login_macos_arm)
- [Linux x64](link)

Then download [credentials.txt](https://github.com/Herivelismus/Auto_Stormshield/releases/download/v0.1/credentials.txt) file in the same directory as your executable then edit:

```bash
  id
  password
  https://fw-cgcp.emse.fr
```
Leave https://fw-cgcp.emse.fr for Mines St Etienne students.

## Usage
Just run the executable

Or you can also pass `id`, `password` and `host` in the terminal:

```bash
  ./login id password host
```

## Compile it yourself
```bash
  gcc login.c -o login -lcurl -lxml2 -lpcre
```
## Contact
You can always contact me at [jules.massart@etu.emse.fr](mailto:jules.massart@etu.emse.fr)

