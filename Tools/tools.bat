@rem main script start
@echo off
setlocal enabledelayedexpansion


@rem read config.txt
set /a is_multiline=0
for /f "tokens=*" %%A in (%~dp0config.txt) do (   
    @rem get first char of input line
    set raw_input=%%A
    set "char0=!raw_input:~0,1!"
    
    @rem ignore lines that begin with comment delim, '#'
    if not "!char0!"=="#" (
        @rem ignore everything after the first comment delim
        for /f "tokens=1 delims=#" %%B in ("!raw_input!") do set clean_input=%%B
        
        @rem process cleaned input line
        if [!is_multiline!]==[0] (
            @rem single line mode
            @rem read key-value pair
            for /f "tokens=1,2 delims==" %%C in ("!clean_input!") do (
                @rem switch to multiline mode if 'value' is start delim, '>'
                @rem otherwise init var with key-value pair
                if "%%D"==">" (
                    set /a is_multiline=1
                    set key=%%C
                ) else (
                    set %%C=%%D
                ) 
            )
        ) else (
            @rem multiline mode
            @rem read value
            for /f "tokens=*" %%E in ("!clean_input!") do (
                @rem switch to single line mode and init var with key-value pair if 'value' is end delim, '<'
                @rem otherwise build multiline 'value' arg
                if "%%E"=="<" (
                    set /a is_multiline=0
                    set "value=!value:~1!"
                    set !key!=!value!
                ) else (
                    set "value=!value!;%%E"
                )
            )
        )
    )
)


@rem read mode
if "%~1"=="" call :throw_err "missing mode" & goto :exit_err
if "%~1"=="frm" set mode=firmware
if "%~1"=="firmware" set mode=firmware
if "%~1"=="tst" set mode=testing
if "%~1"=="testing" set mode=testing
if "%mode%"=="" call :throw_err "unknown mode '%~1'" & goto :exit_err


@rem read operations
if "%~2"=="" call :throw_err "missing operations" & goto :exit_err
set operations=%~2
set operations=%operations:-=;%

set /a do_clean=0
set /a do_compile=0
set /a do_lint=0
set /a do_run=0
for %%A in (%operations%) do (
    set /a is_valid=0
    if "%%A"=="x" call :set_operations do_clean do_compile
    if "%%A"=="c" call :set_operations do_compile
    if "%%A"=="l" (
        call :set_operations do_lint 
        if not "%mode%"=="firmware" call :throw_warn "redundant operation 'l' - linting is only supported in firmware mode"
    )
    if "%%A"=="r" (
       call :set_operations do_run
       if not "%mode%"=="testing" call :throw_warn "redundant operation 'r' - running is only supported in testing mode"
    )
    if [!is_valid!]==[0] call :throw_err "invalid operation '%%A'" & goto :exit_err
)


@rem read options



@rem main script end
endlocal
exit /b 0


@rem helper functions
:set_operations
set /a is_valid=1
for %%A in (%*) do set /a %%A=1
exit /b 0

:throw_warn
echo Warning: %~1
exit /b 0

:throw_err
echo Error: %~1
exit /b 0

:exit_err
exit /b 1
