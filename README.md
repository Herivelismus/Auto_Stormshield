
# Auto Stormshield

A little C program to avoid logging into the Stormshield portal everytime.

## Setup

Download the appropriate version of the program
- <pre>[Windows x64](link)
- [MacOS x64](link)
- [MacOS ARM](https://github.com/Herivelismus/Auto_Stormshield/releases/download/v0.1/login_macos_arm)
- [Linux x64](link)

Then edit the `credentials.txt` file in the same directory as your executable as so:

```bash
  yourid
  yourpassword
  https://fw-cgcp.emse.fr
```
Leave https://fw-cgcp.emse.fr for Mines St Etienne students.

## Usage
Just run the executable

Or you can also pass `id`, `password` and `host` in the terminal:

```bash
  ./login myid my_passwd myhost
```

## Compile it yourself
```bash
  gcc login.c -o login -lcurl -lxml2 -lpcre
```
## Contact
you can always contact me at [jules.massart@etu.emse.fr](mailto:jules.massart@etu.emse.fr)

