# Ironbrew Deobfuscator

a really big project built on C++17 which is around 5k lines or more, i've been working on it since late june 2021, left still unfinished, and i've released this one version for now (with removed some stuff such as source generator.), since im not planning on selling the deobfuscator and i have another stuff to deal with currently.

the entire deobfuscator is barebones AST level based, doesn't use anything related to lua source.

tested with big scripts (without control flow yet) and works pretty good.

everything in this project is made by me, and if you use any part of it you should atleast give me a credit.

if you need to talk with me about anything related to this project, i'd appreciate that, the discord can be found in my profile.


# NOTES
this project can't be compiled by users yet since unfinished, only if you want you can use ANTLR runtime as 3rd party and generate lua parser files with the ANTLR Lua.g4 grammar file.

# Deobfuscator Analysis

Intermediate Level (AST & etc) : fully done.

Low Level : WIP

# TODO

Deobfuscator TODO list : 

AST Level : fix some bugs at super op handler (very rare)

low-level control flow passes only (but kinda useless since nobody uses the controlflow features except string enc): topological sorting for bblocks (SCC detection etc methods) (better control flow output when converting to insns'), string decryption, undo test spam mutate garbage,
also do we need to do inlining ? not being really used as much in scripts, on inlining instead of MOVE instructions and CALL replaced as JMP to relative offsets and branches and dominators are being modified.

aswell as preccedor/succesor architecture for basic blocks and DCE structure.
