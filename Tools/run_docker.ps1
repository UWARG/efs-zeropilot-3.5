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
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./tools.bash compile -c"
        docker cp efs_container:/src/Tools/Firmware $PSScriptRoot/
    }

    "clang-format" {
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./clang-format.bash"
        docker cp efs_container:/src/Tools/LintOutput/formatted-files $PSScriptRoot/LintOutput/
    }

    "clang-tidy" {
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./clang-tidy.bash"
        docker cp efs_container:/src/Tools/LintOutput/clang-tidy.txt $PSScriptRoot/LintOutput/
    }

    "cppcheck" {
        docker exec efs_container /bin/bash -c "cd /src/Tools && ./cppcheck.bash"
        docker cp efs_container:/src/Tools/LintOutput/cppcheck.txt $PSScriptRoot/LintOutput/
    }
}

docker stop -t 3 efs_container
