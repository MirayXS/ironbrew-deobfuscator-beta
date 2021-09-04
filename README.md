# Ironbrew Deobfuscator

a really big project (around 5k lines), i've worked on since late june 2021, still unfinished
released this one version for now (with removed some stuff such as source generator.), since im not planning on selling the deobfuscator and i have another stuff to deal with currently.

the entire deobfuscator is barebones AST level based, doesn't use anything related to lua source.

tested with big scripts (without control flow yet) and works pretty good.

everything in this project is made by me (expect some stuff i've credited people for.), and if you use any part of it you should atleast give me a credit.

if you need to talk with me about anything related to this project, i'd appreciate that.
make sure you join this discord server :
https://discord.io/glacier_

Add me on discord :
Cyclops#0003 (ID : 306127854658256896)

# TODO

Deobfuscator analysis:

Intermediate Level (AST & etc) : fully done.
Low Level : still in work.

Deobfuscator TODO list : low-level control flow passes only: topological sorting for nodes with kosaraju's algorithm for SCC detection (better control flow output and ordered nodes.), string decryption, undo test spam mutate garbage,
also do we need to do inlining ? not being really used as much in scripts, on inlining instead of MOVE instructions and CALL replaced as JMP to relative offsets
