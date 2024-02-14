# SHABFIGHT - WIP
### shabibi studios

### Installation

clone SHABFIGHT repository to your pc:
---
```bash
git clone [Repository URL]
cd [download-directory]
cd build
```

**By default, the Makefile is configured for Windows.**

### Building the Game

#### For Windows

Open PowerShell and run:

```powershell
make
./shabgame.exe
```

#### For Mac

**the library used for the mac build was compiled on arm, if you have an x86_64 mac you will get linker errors**
--will fix soon 

Open Terminal and run:

```bash
make mac
./shabgame
```

---
### Game Info
##### this is subject to change
Testing Controls <br />
A - move left <br />
D - move right <br />
W - punch <br />
S - sweep <br />
Q - kick <br />
E - uppercut <br />
F - block <br />
R - low block <br />

left_arrow - move left <br />
right_arrow - move right <br />


### Engine Info
By default, the engine will run in debug mode <br />
In debug mode, by default the engine will draw bones <br />
Press V to toggle bone drawing <br />
Press B to toggle debug mode <br />

#### Made using Raylib
[https://www.raylib.com/]
