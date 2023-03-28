# NPShell
Network Programming Project1: NP SHELL<br>
In this project, you are asked to design a shell named npshell. The npshell should support the
following features:
1. Execution of commands
2. Ordinary Pipe
3. Numbered Pipe
4. File Redirection

## Scenario
The following is a scenario of using the npshell.
```
bash$ ./npshell # execute your npshell

% printenv PATH # initial PATH is bin/ and ./
bin:.

% setenv PATH bin # set PATH to bin/ only

% printenv PATH
bin

% ls
bin npshell test.html

% ls bin
cat ls noop number removetag removetag0

% cat test.html > test1.txt

% cat test1.txt
<!test.html>
<TITLE>Test</TITLE>
<BODY>This is a <b>test</b> program
for ras.
</BODY>

% removetag test.html

Test
This is a test program
for ras.

% removetag test.html > test2.txt

% cat test2.txt
Test
This is a test program
for ras.

% removetag0 test.html
Error: illegal tag "!test.html"
Test
This is a test program
for ras.

% removetag0 test.html > test2.txt
Error: illegal tag "!test.html"

% cat test2.txt
Test
This is a test program
for ras.

% removetag test.html | number
1
2 Test
3 This is a test program
4 for ras.
5

% removetag test.html |1 # pipe STDOUT to the first command of next line

% number # STDIN is from the previous pipe (removetag)
1
2 Test
3 This is a test program
4 for ras.
5

% removetag test.html |2 # pipe STDOUT to the first command of next next line

% ls
bin npshell test1.txt test2.txt test.html

% number # STDIN is from the previous pipe (removetag)
1
2 Test
3 This is a test program
4 for ras.
5

% removetag test.html |2 # pipe STDOUT to the first command of next next line

% removetag test.html |1 # pipe STDOUT to the first command of next line
# (merge with the previous one)

% number # STDIN is from the previous pipe (both two removetag)
1
2 Test
3 This is a test program
4 for ras.
5
6
7 Test
8 This is a test program
9 for ras.
10

% removetag test.html |2

% removetag test.html |1

% number |1

% number
1 1
2 2 Test
3 3 This is a test program
4 4 for ras.
5 5
6 6
7 7 Test
8 8 This is a test program
9 9 for ras.
10 10

% removetag test.html | number |1

% number
1 1
2 2 Test
3 3 This is a test program
4 4 for ras.
5 5

% ls |2

% ls
bin npshell test1.txt test2.txt test.html

% number > test3.txt

% cat test3.txt
1 bin
2 npshell
3 test1.txt
4 test2.txt
5 test.html

% removetag0 test.html |1
Error: illegal tag "!test.html" # output error message to STDERR

% number
1
2 Test
3 This is a test program
4 for ras.
5

% removetag0 test.html !1 # pipe both STDOUT and STDERR
# to the first command of the next line

% number
1 Error: illegal tag "!test.html"
2
3 Test
4 This is a test program
5 for ras.
6

% date
Unknown command: [date].
# TA manually moves the executable "date" into $working_dir/bin/

% date
Mon Oct 4 15:12:35 CST 2021

% exit
bash$
```
## Specification
### NP Shell Behavior
1. Use ”% ” as the command line prompt. Notice that there is one space character after %.
2. The npshell parses the inputs and executes commands.
3. The npshell terminates after receiving the exit command or EOF.
4. There will NOT exist the test case that commands need to read from STDIN.

### Input
1. The length of a single-line input will not exceed 15000 characters.
2. The length of each command will not exceed 256 characters.
3. There must be one or more spaces between commands, arguments, pipe symbol (|), and redi-
rection symbol (>), but no spaces between pipe and numbers for numbered-pipe.<br>
Examples:
```
% ls -l | cat
% ls > hello.txt
% cat hello.txt |4 # no space between "|" and "4"
% cat hello.txt !4 # no space between "!" and "4"
```
4. Only English alphabets (uppercase and lowercase), digits, space, newline, ”.”, ”-”,
”:”, ”>”, ”|”, and ”!” may appear in test cases.

### Built-in Commands
1. Format
- setenv [var] [value]<br>
Change or add an environment variable.<br>
If var does not exist in the environment, add var to the environment with the value value.<br>
If var already exists in the environment, change the value of var to value.<br>
Examples:
```
% setenv PATH bin # set PATH to bin
% setenv PATH bin:npbin # set PATH to bin:npbin
```

- printenv [var]<br>
Print the value of an environment variable.<br>
If var does not exist in the environment, show nothing.<br>
Examples:
```
% printenv LANG
en_US.UTF-8
% printenv VAR1 # show nothing if the variable does not exist
% setenv VAR1 test
% printenv VAR1
test
```
- exit<br>
Terminate npshell.

2. Built-in commands will appear solely in a line.
3. Built-in commands will not pipe to other commands, and no commands will pipe to built-in
commands.

### Unknown Command
1. If there is an unknown command, print error message to STDERR with the following format:<br>
Unknown command: [command].<br>
Examples:
```
% ctt
Unknown command: [ctt].
```
2. You do not need to print the arguments of unknown commands.<br>
Examples:
```
% ctt -n
Unknown command: [ctt].
```
3. The commands after unknown commands will still be executed.<br>
Examples:
```
% ctt | ls
Unknown command: [ctt].
bin npshell test.html
```
4. Messages piped to unknown commands will disappear.<br>
Examples:
```
% ls | ctt
Unknown command: [ctt].
```

### Ordinary Pipe && Numbered Pipe
1. You need to implement pipe (cmd1 | cmd2), which means the STDOUT of the left hand side<br>
command will be piped to the right hand side command.<br>
Examples:
```
% ls | cat # The output of command "ls" acts as the input of command "cat"
bin
npshell
test.html
```
2. You need implement a special piping mechanism, called numbered pipe. There are two types
of numbered pipe (cmd |N and cmd !N).

3. |N means the STDOUT of the left hand side command will be piped to the first command
of the next N-th line, where 1 ≤ N ≤ 1000.

4. !N means both STDOUT and STDERR of the left hand side command will be piped to the
first command of the next N-th line, where 1 ≤ N ≤ 1000.

5. |N and !N will only appear at the end of the line.

6. The line with build-in command or unknown command also counts as one line for numbered
pipe, but the empty line does not.<br>

Examples:
```
% ls |2

% ctt
Unknown command: [ctt].

% cat # The output of command "ls" acts as the input of command "cat"
bin
npshell
test.html

% ls |2

% setenv PATH bin

% # Press Enter

% cat # The output of command "ls" acts as the input of command "cat"
bin
npshell
test.html
```

7. When the output of one command is piped (no matter ordinary pipe or numbered pipe), it is<br>
likely that the output of this command exceeds the capacity of the pipe. In this case, you still<br>
need to guarantee that all the output of this command is piped correctly.<br>
Examples:
```
% cat large_file.txt | number # All output of "cat" should be piped correctly
<many outputs...>

% cat large_file.txt |1 # All output of "cat" should be piped correctly

% number
<many outputs...>
(Hints: You can think about when the npshell should wait the child process.)
```

8. The number of piped commands may exceed the maximum number of processes that one user<br>
can run. In this case, you still need to guarantee that all commands are executed properly.<br>
Examples:
```
# Suppose the process limit is 512 and there are 1000 "cat" in one line.
# All commands in this line should be executed properly.
% ls | cat | cat ...... | cat
bin
npshell
test.html
```
9. There will NOT exist the test case that one pipe is full and needs to be read by the following<br>
process (not forked yet), but the process limit is reached.<br>

### File Redirection
1. You need to implement standard output redirection (cmd > file), which means the output<br>
of the command will be written to files.<br>
Examples:
```
# The output of command "ls" is redirected to file "hello.txt"
% ls > hello.txt

% cat hello.txt
bin
npshell
test.html
```
2. If the file already exists, the file should be overwritten (not append).
3. You do not need to handle appending for standard output redirection (>>).
4. You do not need to implement standard input redirection (<).
5. You do not need to implement standard error redirection (2>).
6. You do not need to handle outputting to multiple files for the same command.
7. You do not need to handle outputting to both the file and the pipe for the same command.<br>
Examples:
```
% ls > f1.txt > f2.txt # This will not happen

% ls > hello.txt | cat # This will not happen

% ls > hello.txt |2 # This will not happen

% cat f1.txt | number > f2.txt # This may happen
```
## Spec
Read NP_Project1_Spec.pdf to see more information
