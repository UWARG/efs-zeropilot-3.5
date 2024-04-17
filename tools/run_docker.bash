#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")
# define container paths
DOCKER_TOOLS_DIR=efs_container:/zeropilot-3.5/tools
SCRIPTS_DIR=/zeropilot-3.5/tools/scripts

# Help message
if [[ $1 == "help" ]]; then
    echo "run_docker.bash help          | show this message"
    echo "run_docker.bash compile       | compile/build ZeroPilot and return build files in firmware/build/"
    echo "run_docker.bash test          | compile tests, run tests, and return build files in testing/build/"
    echo "run_docker.bash clang-tidy    | lint code with clang-tidy and return linting log in lint_output/"
    echo "run_docker.bash clang-format  | reformat and return source files with clang-format"
    echo "                                in lint_output/formatted_files"
    echo "run_docker.bash cppcheck      | lint code with cppcheck and return linting log in lint_output/"
    echo "run_docker.bash shell         | launch a bash shell in the Docker container"
    exit
fi

# Build image if it doesn't exist
IMAGES=$(docker image ls)
if [[ $IMAGES != *efs_image* ]]; then
    docker build -t efs_image -f $TOOLS_DIR/Dockerfile .
fi

# Create container if it doesn't exist
CONTAINERS=$(docker ps -a)
if [[ $CONTAINERS != *efs_container* ]]; then
    docker run -tid --name=efs_container efs_image bash
else
    # Start container
    docker start efs_container
fi

docker cp $TOOLS_DIR/../. efs_container:/zeropilot-3.5/

# Create directory for linting output if it doesn't exist
mkdir -p $TOOLS_DIR/lint_output

case $1 in
    "shell")
        docker attach efs_container
    ;;

    "compile")
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./build.bash -c"
        docker cp $DOCKER_TOOLS_DIR/firmware $TOOLS_DIR/
    ;;

    "test")
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./build.bash -t testing -c && ./test.bash"
        docker cp $DOCKER_TOOLS_DIR/testing $TOOLS_DIR/
    ;;

    "clang-format")
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./clang-format.bash"
        docker cp $DOCKER_TOOLS_DIR/lint_output/formatted_files $TOOLS_DIR/lint_output/
    ;;

    "clang-tidy")
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./clang-tidy.bash"
        docker cp $DOCKER_TOOLS_DIR/lint_output/clang-tidy.txt $TOOLS_DIR/lint_output/
    ;;

    "cppcheck")
        docker exec efs_container /bin/bash -c "cd $SCRIPTS_DIR && ./cppcheck.bash"
        docker cp $DOCKER_TOOLS_DIR/lint_output/cppcheck.txt $TOOLS_DIR/lint_output/
    ;;
esac

docker stop -t 3 efs_container
