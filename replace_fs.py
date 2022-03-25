Import("env")
from sys import platform

if platform == "win32":
    env.Replace (MKSPIFFSTOOL = "mklittlefs.exe")
else:
    env.Replace (MKSPIFFSTOOL = "mklittlefs")
