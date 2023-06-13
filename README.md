# ZeroPilot 3.5 Compile Commands

- A powershell script is provided for Windows

- A bash script is provided for Linux and Mac

- Once compiled, binaries for flashing can be found under *./Tools/Production/build*

- Once compiled, a testing executable can be found under *./Tools/Testing/build*


## Syntax

### Windows
./Tools/tools.ps1 \[function\]  \[options\]
<br>

### Linux and Mac

./Tools/tools.bash \[function\]  \[options\]

## Function

The function is a mandatory parameter that specifies script action: *compile* or *run*
- Set to *compile* to build the project for either flashing or testing
- Set to *run* to execute an existing test executable

## Options

Options are nonmandatory parameters which are used to override the default configurations specified in *./Tools/default_config.txt*

### Compile Only Options

- -t <br> &nbsp;&nbsp;&nbsp;&nbsp;Specifies the build type. Set to either *Production* or *Testing* 

- -p <br> &nbsp;&nbsp;&nbsp;&nbsp;Specifies the platform to build for (i.e. nucleol552zeq)

- -c <br> &nbsp;&nbsp;&nbsp;&nbsp;A clean build will be conducted if and only if this flag is present

### Run Only Options

- -f <br> &nbsp;&nbsp;&nbsp;&nbsp;Specifies a filter to selectively run tests in the GoogleTest executable

## Examples

- Compile with default settings, no clean <br> &nbsp;&nbsp;&nbsp;&nbsp;./Tools/tools.ps1 compile

- Compile with default settings, clean <br> &nbsp;&nbsp;&nbsp;&nbsp;./Tools/tools.ps1 compile -c

- Compile with default settings except for compile type, clean <br> &nbsp;&nbsp;&nbsp;&nbsp;./Tools/tools.ps1 compile -t Testing -c

- Run with default filter <br> &nbsp;&nbsp;&nbsp;&nbsp;./Tools/tools.ps1 run

- Run with custom filter <br> &nbsp;&nbsp;&nbsp;&nbsp;./Tools/tools.ps1 run -f \<filter\>
