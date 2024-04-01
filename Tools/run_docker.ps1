# define path to the helper scripts dir in the container
$SCRIPTS_DIR = "/src/Tools/HelperScripts"

# Help message
if ( $args[0] -eq "help" ) {
    echo "run_docker.ps1 help          | show this message"
    echo "run_docker.ps1 compile       | compile/build ZeroPilot and return build files in Firmware/build/"
    echo "run_docker.ps1 test          | compile tests, run tests, and return build files in Testing/build/"
    echo "run_docker.ps1 clang-tidy    | lint code with clang-tidy and return linting log in LintOutput/"
    echo "run_docker.ps1 clang-format  | reformat and return source files with clang-format"
    echo "                               in LintOutput/formatted_files"
    echo "run_docker.ps1 cppcheck      | lint code with cppcheck and return linting log in LintOutput/"
    echo "run_docker.ps1 shell         | launch a bash shell in the Docker container"
    exit
}

# Build image if it doesn't exist
$images = docker image ls
if ( -not ( $images -like "*efs_image*" ) ) {
    docker build -t efs_image -f $PSScriptRoot/Dockerfile .
}

# Create container if it doesn't exist
$containers = docker ps -a
if ( -not ( $containers -like "*efs_container*" ) ) {
    docker run -tid --name=efs_container efs_image bash
} else {
    # Start container
    docker start efs_container
}

docker cp $PSScriptRoot/../. efs_container:/src/

# Create directory for linting output if it doesn't exist
if ( -not ( Test-Path $PSScriptRoot/LintOutput ) ) {
    New-Item -ItemType Directory -Force -Path $PSScriptRoot/LintOutput
}

switch ( $args[0] )
{
    "shell" {
        docker attach efs_container
    }

    "compile" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./build.bash -c"
        docker cp efs_container:/src/Tools/Firmware $PSScriptRoot/
    }

    "test" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./build.bash -t Testing -c && ./test.bash"
        docker cp efs_container:/src/Tools/Testing $PSScriptRoot/
    }

    "clang-format" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./clang-format.bash"
        docker cp efs_container:/src/Tools/LintOutput/formatted-files $PSScriptRoot/LintOutput/
    }

    "clang-tidy" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./clang-tidy.bash"
        docker cp efs_container:/src/Tools/LintOutput/clang-tidy.txt $PSScriptRoot/LintOutput/
    }

    "cppcheck" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./cppcheck.bash"
        docker cp efs_container:/src/Tools/LintOutput/cppcheck.txt $PSScriptRoot/LintOutput/
    }
}

docker stop -t 3 efs_container
