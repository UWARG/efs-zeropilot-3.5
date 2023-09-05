# ====================
# --- var declare ---
# ====================

# read options from command line
param(
    [Parameter(Mandatory = $true, Position = 0)] [string] $FUNCTION,
    [string] $t,
    [string] $p,
    [switch] $c,
    [string] $f,
    [string] $m
)

# read defaults from file, and set variable intial value
foreach($line in Get-Content "$PSScriptRoot/default_config.txt") {
    # ignore comments and empty lines
    if($line -match "(^\s*#.*$|^\s*$)") {
        continue
    }

    # split line into a key-value pair
    $key, $value = $line -split "="

    # remove leading and trailing spaces, then write variables
    New-Variable -name $key.trim() -value $value.trim().replace('"', '')
}

# override initial value if command line options are present
$COMPILE_TYPE = if ($t) {$t} else {$COMPILE_TYPE}
$PLATFORM = if ($p) {$p} else {$PLATFORM}
$TEST_FILTER = if ($f) {$f} else {$TEST_FILTER}
$MODEL_NAME = if ($m) {$m} else {$MODEL_NAME}


# ====================
# --- compile/run ---
# ====================

if($FUNCTION -eq "compile") {    
    # validate build type
    if(! ( $COMPILE_TYPE -eq "Firmware" -or $COMPILE_TYPE -eq "Testing" ) ) {
        Write-Error "Error: Build type $COMPILE_TYPE is undefined."
        exit 1
    }
    
    # select generator
    if(Get-Command "ninja" -ErrorAction SilentlyContinue) {
        $GENERATOR = "Ninja"
    }
    elseif(Get-Command "make" -ErrorAction SilentlyContinue) {
        $GENERATOR = "Unix Makefiles"
    }
    elseif(Get-Command "mingw32-make" -ErrorAction SilentlyContinue) {
        $GENERATOR = "MinGW Makefiles"
    }
    else {
        Write-Error "Error: No cmake generator found."
        exit 1
    }

    # compile setup
    Write-Host "Building ZeroPilot for $($COMPILE_TYPE.ToLower())."
    $COMPILE_DIR = "$PSScriptRoot/$COMPILE_TYPE/build"
    if($COMPILE_TYPE -eq "Firmware") {
        Write-Host "Building for platform $PLATFORM and model $MODEL_NAME."
    }
    if($c) {
        Write-Host "Cleaning old $($COMPILE_TYPE.ToLower()) build environment."
        cmake -E rm -rf $COMPILE_DIR   
    }

    # create build system
    Write-Host "`nCreating $GENERATOR build system..."
    cmake -E make_directory $COMPILE_DIR
    if($COMPILE_TYPE -eq "Firmware") {
        cmake -E chdir $COMPILE_DIR `
            cmake `
                -G "${GENERATOR}" `
                -DCMAKE_BUILD_TYPE="Debug" `
                -DCMAKE_TOOLCHAIN_FILE="../../../Boardfiles/$PLATFORM/$PLATFORM.cmake" `
                -DMODEL_NAME="$MODEL_NAME" `
                -Wdev `
                -Wdeprecated `
                ..
    }
    else {
        cmake -E chdir $COMPILE_DIR `
            cmake `
                -G "${GENERATOR}" `
                ..
    }
    if(! $LASTEXITCODE -eq 0) {
        Write-Error "Error: Failed to create $GENERATOR build system."
        exit 1
    }

    # compile project
    Write-Host "`nCompiling project..."
    cmake --build $COMPILE_DIR
    if(! $LASTEXITCODE -eq 0) {
        Write-Error "Error: Failed to compile project."
        exit 1
    }

    # compile done
    Write-Host "`nZeroPilot $($COMPILE_TYPE.toLower()) build SUCCESS!"
    exit 0
}
elseif ($FUNCTION -eq "run") {
    $TEST_PROGRAM = "$PSScriptRoot/Testing/build/unit_testing"
    
    # check for test executable existance
    if($null -eq (Get-Command "$TEST_PROGRAM" -ErrorAction SilentlyContinue))
    {
        Write-Error "Error: No test executable found. Build testing project first."
        exit 1
    }
    
    & $TEST_PROGRAM --gtest_filter=$TEST_FILTER
    exit $LASTEXITCODE
}
else {
    Write-Error "Error: Function $FUNCTION is undefined."
    exit 1
}
