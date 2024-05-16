# define container paths
$DOCKER_TOOLS_DIR = "efs_container:/zeropilot-3.5/tools"
$SCRIPTS_DIR = "/zeropilot-3.5/tools/scripts"

# Help message
if ( $args[0] -eq "help" ) {
    echo "run_docker.ps1 help          | show this message"
    echo "run_docker.ps1 init          | setup docker only"
    echo "run_docker.ps1 compile       | compile/build ZeroPilot and return build files in firmware/build/"
    echo "run_docker.ps1 test          | compile tests, run tests, and return build files in testing/build/"
    echo "run_docker.ps1 clang-tidy    | lint code with clang-tidy and return linting log in lint_output/"
    echo "run_docker.ps1 clang-format  | reformat and return source files with clang-format"
    echo "                               in lint_output/formatted_files"
    echo "run_docker.ps1 cppcheck      | lint code with cppcheck and return linting log in lint_output/"
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

docker cp $PSScriptRoot/../. efs_container:/zeropilot-3.5/

# Create directory for linting output if it doesn't exist
if ( -not ( Test-Path $PSScriptRoot/lint_output ) ) {
    New-Item -ItemType Directory -Force -Path $PSScriptRoot/lint_output
}

switch ( $args[0] )
{
    "init" {
        $images = docker image ls
        $containers = docker ps -a
        if ( ( $images -like "*efs_image*" ) -and ( $containers -like "*efs_container*" ) ) {
            echo "Docker is initialized."
        } else {
            echo "Docker failed to initialize."
        }
    }

    "shell" {
        docker attach efs_container
    }

    "compile" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./build.bash -c"
        docker cp $DOCKER_TOOLS_DIR/firmware $PSScriptRoot/
    }

    "test" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./build.bash -t testing -c && ./test.bash"
        docker cp $DOCKER_TOOLS_DIR/testing $PSScriptRoot/
    }

    "clang-format" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./clang-format.bash"
        docker cp $DOCKER_TOOLS_DIR/lint_output/formatted_files $PSScriptRoot/lint_output/
    }

    "clang-tidy" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./clang-tidy.bash"
        docker cp $DOCKER_TOOLS_DIR/lint_output/clang-tidy.txt $PSScriptRoot/lint_output/
    }

    "cppcheck" {
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./cppcheck.bash"
        docker cp $DOCKER_TOOLS_DIR/lint_output/cppcheck.txt $PSScriptRoot/lint_output/
    }

    default {
        echo "Invalid run_docker option!"
    }
}

docker stop -t 3 efs_container
