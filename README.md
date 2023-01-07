# OnnxEditor

![main workflow](https://github.com/OYCN/OnnxEditorV2/actions/workflows/all.yml/badge.svg)

> Developing
>
> You can download runable file from release
>
> OR in the action artifacts
>
> OR build on your computer
>
> Not we just support ubuntu in actions
>
> After testing, it can work on ubuntu 20.04 and 22.04
>
> Now, we do not recommend to use it to build onnx from scratch
>
> Please save it frequently to prevent lost your modifications due to program FATAL

# TODO List

 - [x] rename op name
 - [x] rename op type
 - [x] rename io name
 - [x] rename node io
 - [x] add node
 - [x] remove node
 - [x] remove Input/Output
 - [x] add/edit Input/Output
    - [x] display & edit dim
 - [ ] add constant value
    - [ ] display & edit data
 - [ ] display & edit attribute

# Known Bug

 - Some obj is null in initial state
 - Not limit the empty name of node/io
 - UI about edit IO in node will display error in light mode
 
 # Build
 
 ## On ubuntu:
 
 ```
 # build in ./build
 # ccache save to ./ccache
 ./script/build_static_in_docker.sh 
 # run
 ./build/main
 ```
 
 ## On macos:
 
 ```
 ./script/init_submodules.sh
 ./script/build_qt.sh
 ./script/build_self.sh
 # run
 ./build/main
 ```
