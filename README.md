# Mad Dictator
A conquest strategy CNET BBS door game for the Commodore Amiga computer.

# Developer Notes

## Prerequisites
- An Amiga, real hardware or emulated, running AmigaOS 3.2.x
- SAS/C 6.58 
- A good C IDE or text editor for the Amiga.
- ANSI-Draw Amiga application.
- Familarity C programming, using the AmigaOS CLI, and CNET BBS administration and general use.

## Developer Setup
- Add an alias named mad_dict that points to the installation directory. Example:
  ```assign mad_dict: dh0:applications/mad_dictator```
- SAS/C installed and configured. You should be able to run the sc command without errors.

## How to Build
From within the project directory: 
```
1.MadDictator:> build
```

## How to Play Locally (No-BBS)
From within the project directory:
```
1.MadDictator:> bin/client <cnet_player_id> <cnet_player_nickname>

Example:

1.MadDictator:> bin/client 12 kevinum

```

## How to Install as CNET-BBS Door Game
1. Add an alias named mad_dict that points to the installation directory. Example:

```assign mad_dict: dh0:applications/mad_dictator```
2. Configure a new menu item to launch Mad Dictator with the users id and nickname:
```
Title: Mad Dictator
Data dir path: 1.MadDictator:> mad_dict:client %40 %1
```

CNET BBS will pass in the users cnet_user_id (%40) and nickname (%1) as arguments into
the Mad Dictator client application when the CNET menu item is selected. See CNET user
manual for GETUSER values in Chapter 18 of the CNET manual for more information.


