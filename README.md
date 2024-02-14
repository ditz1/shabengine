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

### Engine Info
By default, the engine will run in debug mode.
In debug mode, by default the engine will draw bones.
Press V to toggle bone drawing,
Press B to toggle debug mode,

#### Made using Raylib
[https://www.raylib.com/]
