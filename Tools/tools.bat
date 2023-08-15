@echo off

@rem ==================
@rem --- get inputs ---
@rem ==================

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
        if !is_multiline! equ 0 (
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
call :clean_vars is_multiline raw_input char0 clean_input key value

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
    if !is_valid! equ 0 call :throw_err "invalid operation '%%A'" & goto :exit_err
)
call :clean_vars operations is_valid

@rem read options
for /f "tokens=1-2*" %%A in ("%*") do set opts=%%C
if "%opts%"=="" goto :start_work

set /a is_key=1
for %%A in (%opts%) do (
    if !is_key! equ 1 (
        set key=
        if "%%A"=="-p" call :set_options PLATFORM firmware "redundant option '-p' - platform select is only supported in firmware mode"
        if "%%A"=="-e" call :set_options LINT_EXCLUSION firmware "redundant option '-e' - lint exclusion is only supported in firmware mode"
        if "%%A"=="-t" call :set_options TEST_SELECT testing "redundant option '-t' - test select is only supported in testing mode"
        if "!key!"=="" call :throw_err "invalid option '%%A'" & goto :exit_err
        set /a is_key=0
    ) else (
        set !key!=%%A
        set /a is_key=1
    )
)
call :clean_vars opts is_key key

@rem ==================
@rem --- start work ---
@rem ==================

:start_work
@rem find cmake generator
where ninja >nul 2>nul && set generator="Ninja" && goto :generator_found
where make >nul 2>nul && set generator="Unix Makefiles" && goto :generator_found
where mingw32-make >nul 2>nul && set generator="MinGW Makefiles" && goto :generator_found
call :throw_err "no cmake generator found" & goto :exit_err
:generator_found
set "generator=%generator:~0,-1%"

echo Script is running in %mode% mode
set bin_dir=%~dp0%mode%/build

@rem clean build artifacts
if %do_clean% equ 1 (
    echo Cleaning old %mode% build environment
    cmake -E rm -rf "%bin_dir%" 
) 

@rem core operations
cmake -E make_directory %bin_dir%
if "%mode%"=="firmware" (
    @rem create build system if it does not exist
    if not exist "%bin_dir%/cmakecache.txt" (
        echo\
        echo Configuring for %PLATFORM%
        echo Creating %generator:"=% build system...
        cmake -E chdir %bin_dir%^
         cmake -G %generator% -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE="../../../Boardfiles/%PLATFORM%/%PLATFORM%.cmake" ..
        if not !ERRORLEVEL! equ 0 call :throw_err "Failed to create %generator:"=% build system" & goto :exit_err
    )

    @rem compile
    if %do_compile% equ 1 (
        echo\
        echo Compiling project...
        cmake -E chdir %bin_dir% cmake --build .
        if !ERRORLEVEL! equ 0 (
            echo Successfully compiled Zeropilot for firmware
        ) else (
            call :throw_err "Failed to compile ZeroPilot firmware project" & goto :exit_err
        )
    )
    
    @rem lint
    if %do_lint% equ 1 (
        echo\
        echo Linting project with clang-tidy...

        echo\
        echo Checking project format with clang-format...
    )
) else (
    @rem compile
    if %do_compile% equ 1 (
        echo\
        echo Creating %generator:"=% build system...
        cmake -E chdir %bin_dir% cmake -G %generator% ..
        if not !ERRORLEVEL! equ 0 call :throw_err "Failed to create %generator:"=% build system" & goto :exit_err

        echo\
        echo Compiling project...
        cmake -E chdir %bin_dir% cmake --build .
        if !ERRORLEVEL! equ 0 (
            echo Successfully compiled Zeropilot for testing
        ) else (
            call :throw_err "Failed to compile ZeroPilot testing project" & goto :exit_err
        )
    )

    @rem run
    if %do_run% equ 1 (
        echo\
    )
)

endlocal
exit /b 0

@rem ========================
@rem --- helper functions ---
@rem ========================

:set_operations
set /a is_valid=1
for %%A in (%*) do set /a %%A=1
exit /b 0

:set_options
set key=%~1
if not "%mode%"=="%~2" call :throw_warn "%~3"
exit /b 0

:clean_vars
for %%A in (%*) do set %%A=
exit /b 0

:throw_warn
echo Warning: %~1
exit /b 0

:throw_err
echo Error: %~1
exit /b 0

:exit_err
exit /b 1
