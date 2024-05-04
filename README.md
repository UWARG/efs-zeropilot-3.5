# ZeroPilot 3.5 Docker

Docker is our recommended way of compiling, linting and testing code, since it's cross platform and easy to use.

To get started, install Docker from their [official website](https://docs.docker.com/get-docker/) and run our docker script. It will take some time to build the Docker image and container when you run it for the first time.

## Docker Scripts
- Windows: `run_docker.ps1`
- Linux and MacOS: `run_docker.bash`

To compile the code, simply run with the `compile` option:
```
./run_docker.ps1 compile  # Windows
./run_docker.bash compile # Linux and MacOS
```

Run with the `help` option to see more options such as linting and Docker container shell
```
./run_docker.ps1 help  # Windows
./run_docker.bash help # Linux and MacOS
```

# ZeroPilot 3.5 Compile Commands

`./tools/scripts/build.bash` is used to build the project in the Docker container (although you can run this on your local Linux system as well). You may manually run this script in the container using the Docker script `shell` option to do custom builds.

- Once compiled, binaries for flashing can be found under `./tools/firmware/build`

- Once compiled, a testing executable can be found under `./tools/testing/build`

## Syntax

```
./tools/scripts/build.bash [options]
```

## Options

`options` are nonmandatory parameters which are used to override the default configurations specified in `./tools/configs/compile_config.txt`

- -t <br> &nbsp;&nbsp;&nbsp;&nbsp;Specifies the build type. Set to either `firmware` or `testing`

- -p <br> &nbsp;&nbsp;&nbsp;&nbsp;Specifies the platform to build for (i.e. nucleol552zeq)

- -c <br> &nbsp;&nbsp;&nbsp;&nbsp;A clean build will be conducted if and only if this flag is present

## Examples

- Compile with default settings, no clean <br> &nbsp;&nbsp;&nbsp;&nbsp;`./tools/scripts/build.bash`

- Compile with default settings, clean <br> &nbsp;&nbsp;&nbsp;&nbsp;`./tools/scripts/build.bash -c`

- Compile with default settings except for compile type, clean <br> &nbsp;&nbsp;&nbsp;&nbsp;`./tools/scripts/build.bash -t testing -c`

# ZeroPilot 3.5 Test Commands

`./tools/scripts/test.bash` is used to run tests in the Docker container (although you can run this on your local Linux system as well). You may manually run this script in the container using the Docker script `shell` option to do custom testing.

## Syntax

```
./tools/scripts/test.bash [options]
```

## Options

- -f <br> &nbsp;&nbsp;&nbsp;&nbsp;Specifies a filter to selectively run tests in the GoogleTest executable

## Examples

- Run with default filter <br> &nbsp;&nbsp;&nbsp;&nbsp;`./tools/scripts/test.bash`

- Run with custom filter <br> &nbsp;&nbsp;&nbsp;&nbsp;`./tools/scripts/test.bash -f <filter>`
