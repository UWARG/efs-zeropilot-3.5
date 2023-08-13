@echo off

setlocal enabledelayedexpansion

@rem read config.txt
set /a is_multiline=0
for /f "tokens=*" %%A in (%~dp0config.txt) do (   
    @rem get first char of input line
    set raw_input=%%A
    set "char0=!raw_input:~0,1!"
    
    @rem ignore lines that begin with comment delim, '#'
    if not !char0!==# (
        @rem ignore everything after the first comment delim
        for /f "tokens=1 delims=#" %%B in ("!raw_input!") do set clean_input=%%B
        
        @rem process cleaned input line
        if !is_multiline!==0 (
            @rem single line mode
            @rem read key-value pair
            for /f "tokens=1,2 delims==" %%C in ("!clean_input!") do (
                @rem switch to multiline mode if 'value' is start delim, '>'
                @rem otherwise init var with key-value pair
                if %%D==^> (
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
                if %%E==^< (
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

endlocal
