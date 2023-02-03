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

# Keyboard Shortcuts

 - `ctl - s`: save file
 - `ctl - w`: close file
 - `ctl - e`: save as
 - `del` or `backspace`: delete nodes
 - `shift` + `mouse left pressed move`: select nodes

# Support List

 - [x] Edit input/output
   - [x] Edit `name` & `data_type` & `dim`
   - [x] Add & Remove
 - [x] Edit node
   - [x] Edit `name` & `op_type` & `input` & `output`
   - [x] Add & Remove
   - [x] Edit & Add & Remove `attribute`
     - [ ] Edit Tensor Object
 - [ ] Built-in script
   - [ ] About Dims
     - [ ] Set BatchSize for All Node/Value
     - [ ] Shape Inference (By ONNX)
   - [ ] Check whether the node/graph is correct
 - [ ] Functional
   - [ ] Search Node
   - [ ] Switch Node display (`name` or `op_type`)
 
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
