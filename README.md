# OOPD-Project

## Steps to build and compile the code

To compile the c++ file you need to run the `make` command.
* Clone this repository, and move to the `OOPD-Project`. 
* run `make` command. This command will generate four files.
* `main` file is optimized file for execution.
* `mainDebug` file is also created, which is used for debugging purpose.
* `multi_threaded` file is multithreaded implementation of shell.
* `multi_threadedDebug` file is also created, which is used for debugging purpose.
* Additionally you can use `make clean` command to delete these four generated binaries.
* After that is there is also a bash script named `Question_2.sh`. To make it executale you need to do the following:
    -   Type the command `icacls .\Question_2.sh /grant "$($env:USERDOMAIN)\$($env:USERNAME):(RX)"` into kernel and press enter
    - After that run `./Question_2.sh` to create all directories mentioned in the quesiton.

## Input format

* This is a Shell Implemention
* It works exactly like linux shell. It supports few commands like `cd, mv, ls, rm, mkdir, clear, exit` with options like `-r, -l, -h, -a`
* Like you run these commands in linux based system. In same manner you run in this kernel also.

## Output format

* like every linux command this will also give outputs.

## List of References

- I have also used `StackOverflow` for identifying my silly mistakes.
- I have also used `ChatGPT` for my project.
- Class Notes.