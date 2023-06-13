script_path=$(dirname -- "$0")

if [ $# = 0 ]; then 
    echo "no args passed, exiting"
    exit
fi

if [ $1 = "clean" ]; then  
    if [ -d "$script_path/build" ]; then
        echo "removing testing build folder ..."
        rm -rf $script_path/build
    else
        echo "no build folder, exiting ..."
        exit
    fi
elif [ $1 = "build" ]; then
    echo "building tests"
    if [ ! -d "$script_path/build" ]; then
        mkdir $script_path/build
    fi
    cd $script_path/build
    cmake ..
    make 
    cd ..
elif [ $1 = "run" ]; then 
    if [ ! -f "$script_path/build/test_proj" ]; then
        echo "please build the test_proj executable using the build option. exiting"
        exit
    fi
    if [ $# -lt 2 ]; then
        echo "no filter passed, assuming all tests"
        $script_path/build/test_proj
        exit
    fi
    FILTER=$2
    echo "using filter: $FILTER"
    $script_path/build/test_proj --gtest_filter=$FILTER
    exit
else 
    echo "arg not recognized. exiting"
    exit
fi