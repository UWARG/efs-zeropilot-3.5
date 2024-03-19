#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

# Help message
if [[ $1 == "help" ]]; then
    echo "run_docker.bash help          | show this message"
    echo "run_docker.bash compile       | compile/build ZeroPilot and return build files in Firmware/build/"
    echo "run_docker.bash clang-tidy    | lint code with clang-tidy and return linting log in LintOutput/"
    echo "run_docker.bash clang-format  | reformat and return source files with clang-format"
    echo "                                in LintOutput/formatted_files"
    echo "run_docker.bash cppcheck      | lint code with cppcheck and return linting log in LintOutput/"
    echo "run_docker.bash shell         | launch a bash shell in the Docker container"
    exit
fi

# Build image if it doesn't exist
IMAGES=$(docker image ls)
if [[ $IMAGES != *efs_image* ]]; then
    docker build -t efs_image -f $SCRIPT_PATH/Dockerfile .
fi

# Create container if it doesn't exist
CONTAINERS=$(docker ps -a)
if [[ $CONTAINERS != *efs_container* ]]; then
    docker run -tid --name=efs_container efs_image bash
else
    # Start container
    docker start efs_container
fi

docker cp $SCRIPT_PATH/../. efs_container:/src/

# Create directory for linting output if it doesn't exist
mkdir -p $SCRIPT_PATH/LintOutput

case $1 in
    "shell")
        docker attach efs_container
    ;;

    "compile")
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./tools.bash compile -c"
        docker cp efs_container:/src/Tools/Firmware $SCRIPT_PATH/
    ;;

    "clang-format")
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./clang-format.bash"
        docker cp efs_container:/src/Tools/LintOutput/formatted-files $SCRIPT_PATH/LintOutput/
    ;;

    "clang-tidy")
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./clang-tidy.bash"
        docker cp efs_container:/src/Tools/LintOutput/clang-tidy.txt $SCRIPT_PATH/LintOutput/
    ;;

    "cppcheck")
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./cppcheck.bash"
        docker cp efs_container:/src/Tools/LintOutput/cppcheck.txt $SCRIPT_PATH/LintOutput/
    ;;
esac

docker stop -t 3 efs_container
