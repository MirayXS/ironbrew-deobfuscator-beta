# Ironbrew Deobfuscator

a really big project built on C++17 which is around 5k lines or more, i've been working on it since late june 2021, left still unfinished, and i've released this one version for now (with removed some stuff such as source generator.), since im not planning on selling the deobfuscator and i have another stuff to deal with currently.

the entire deobfuscator is barebones AST level based, doesn't use anything related to lua source.

tested with big scripts (without control flow yet) and works pretty good.

everything in this project is made by me (expect one/two things i've credited people for.), and if you use any part of it you should atleast give me a credit.

if you need to talk with me about anything related to this project, i'd appreciate that.

Add me on discord :
Cyclops#0003 (ID : 306127854658256896)

# TODO

Deobfuscator analysis:

Intermediate Level (AST & etc) : fully done.
Low Level : still in work.

Deobfuscator TODO list : low-level control flow passes only: topological sorting for nodes with kosaraju's algorithm for SCC detection (better control flow output and ordered nodes.), string decryption, undo test spam mutate garbage,
also do we need to do inlining ? not being really used as much in scripts, on inlining instead of MOVE instructions and CALL replaced as JMP to relative offsets


aswell as preccedor/succesor architecture for basic blocks, might help us with DCE so we can eliminate blocks that aren't being referenced?
