# cilix_rdc

## 介绍

> 基于 libuv 的 RDC（远程设备调用）框架，配合 [gilix](https://github.com/lindorof/gilix) 服用。

- 一切从简，把复杂度放在 [gilix](https://github.com/lindorof/gilix) 
- cilix_rdc 所有逻辑只为远程设备调用服务
- 统一并简化 C 设备库的接口参数
- 固定按照 tcp_sys 协议传输

> rdc 位于 [gilix](https://github.com/lindorof/gilix) 下游，由于万恶的基于 C 的 lfs 规范存在，所以上游需要 spi_ap 这个 C 库来配合

- 一切从简，只为符合万恶的基于 C 的 lfs 接口
- 固定按照 tcp_sys 协议传输，与 [gilix](https://github.com/lindorof/gilix) 交互
- 复杂度放在 [gilix](https://github.com/lindorof/gilix) ，因此 reg/dereg 的逻辑不用 spi_ap 管理
- 因此 spi_ap 的核心工作是数据编解码和传输

## 编译和适配

> 编译

cilix_rdc 基于 C 实现，使用 CMake 编译：

```shell
cmake .
make
```

> 成果

编译成果存储路径如下，由 CMake 自动检测并生成文件夹名称：

- libs
    - Linux
        - x86_64
        - aarm64
    - Darwin
        - x86
        - ...
    - ...

> 适配

目前已编译、适配、并测试过的 OS+CPU 如下，其它系统下编译时可能涉及轻微的代码修正：

- OS
    - Ubuntu
    - 麒麟OS
    - 统信UOS
    - MacOS
- CPU
    - x86_64
    - aarm64

---

*lindorof . 2021.12.08* 

