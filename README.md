# 操作系统课程设计 — 基础必做部分

2026计算机科学与技术1班 | 操作系统课程设计基础必做模块（70%）

---

## 项目概述

本项目采用 **C99 + Linux POSIX** 实现操作系统课程设计的四个基础模块：

| 模块 | 内容 | 可执行文件 |
|:---|:---|:---|
| **处理机调度** | FCFS、SJF（非抢占/抢占）、时间片轮转(RR)、优先级调度（非抢占/抢占） | `scheduler` |
| **内存管理** | FIFO/LRU页面置换、首次适应(FF)/最佳适应(BF)动态分区 | `memory` |
| **进程同步** | 生产者-消费者、读者-写者、哲学家进餐（死锁避免） | `sync` |
| **文件系统** | 简易类Unix文件系统：inode、目录、位图空闲管理 | `filesystem` |

---

## 编译与运行

### 环境要求
- Linux 操作系统
- GCC 编译器（支持 C99）
- Make 构建工具
- POSIX 线程库（pthread）

### 一键编译全部模块
```bash
make all
```

### 单独编译某个模块
```bash
make scheduler    # 处理机调度
make memory       # 内存管理
make sync         # 进程同步
make filesystem   # 文件系统
```

### 运行交互式程序
```bash
./scheduler/scheduler
./memory/memory
./sync/sync
./filesystem/filesystem
```

### 运行自动测试
```bash
make test
```

或单独运行：
```bash
./scheduler/scheduler test
./memory/memory test
./sync/sync test
./filesystem/filesystem test
```

### 清理构建产物
```bash
make clean
```

---

## 各模块详细说明

### 1. 处理机调度 (scheduler/)

**支持算法**：
- **FCFS**（先来先服务）
- **SJF**（短作业优先）— 非抢占与抢占两种模式
- **RR**（时间片轮转）— 可配置时间片大小
- **Priority**（优先级调度）— 非抢占与抢占两种模式

**功能**：
- 手动输入进程参数（到达时间、执行时间、优先级）
- 加载3组预设测试用例，方便对比算法性能
- 输出 ASCII 甘特图与详细时间指标表
- 汇总对比各算法的平均周转时间与平均等待时间

**示例输出**：见测试运行结果中的甘特图与对比表。

---

### 2. 内存管理 (memory/)

#### 2.1 页面置换 (paging.c)
**算法**：FIFO、LRU  
**功能**：
- 输入页面访问串和物理帧数量
- 逐步展示每次访问的命中/缺页状态、页表内容
- 自动统计缺页次数与缺页率

#### 2.2 动态分区管理 (partition.c)
**算法**：首次适应(FF)、最佳适应(BF)  
**功能**：
- 模拟内存分配与回收
- 自动合并相邻空闲分区
- 输出分区表与 ASCII 内存布局图

---

### 3. 进程同步 (sync/)

采用 **POSIX 线程 + 信号量 + 互斥锁** 实现。

#### 3.1 生产者-消费者
- 多生产者 + 多消费者线程
- 有界循环缓冲区
- 信号量 `empty_slots` / `filled_slots` + 互斥锁 `mutex`

#### 3.2 读者-写者
- 读者优先策略
- 使用读者计数器 + 队列锁控制并发

#### 3.3 哲学家进餐
- 5位哲学家、5支筷子
- **死锁避免方案**：奇偶号哲学家采取不同的拿筷顺序
- 随机思考/进餐时间，模拟真实并发场景

---

### 4. 文件系统 (filesystem/)

采用 **内存模拟磁盘** 的简化类 Unix 文件系统设计。

**核心结构**：
- **超级块(SuperBlock)**：记录 inode/块总数与空闲数
- **inode**：管理文件/目录元数据，支持10个直接块指针
- **目录项(DirEntry)**：文件名到 inode 的映射
- **位图(Bitmap)**：空闲数据块管理

**支持命令**：
| 命令 | 说明 |
|:---|:---|
| `touch <name>` | 创建空文件 |
| `write <name> <data>` | 写入文件内容 |
| `cat <name>` | 读取文件内容 |
| `mkdir <name>` | 创建目录 |
| `cd <name>` | 切换目录（支持 `..` 回根目录） |
| `ls` | 列出当前目录内容 |
| `rm <name>` | 删除文件/空目录 |
| `df` | 显示磁盘使用情况 |
| `quit` / `exit` | 退出 |

---

## 项目结构

```
os_assignment/
├── Makefile                 # 顶层构建
├── README.md                # 本文件
├── common/                  # 公共库
│   ├── common.h             # 宏、颜色输出、安全内存分配
│   ├── list.h / list.c      # 通用双向链表
│   └── utils.h / utils.c    # 输入解析、格式化打印
├── scheduler/               # 处理机调度
│   ├── scheduler.h / .c     # 调度算法实现
│   ├── main.c               # 交互式入口
│   └── Makefile
├── memory/                  # 内存管理
│   ├── paging.h / .c        # 页面置换
│   ├── partition.h / .c     # 动态分区
│   ├── main.c
│   └── Makefile
├── sync/                    # 进程同步
│   ├── producer_consumer.c  # 生产者-消费者
│   ├── readers_writers.c    # 读者-写者
│   ├── philosophers.c       # 哲学家进餐
│   ├── main.c
│   └── Makefile
└── filesystem/              # 文件系统
    ├── fs.h / .c            # 文件系统核心
    ├── disk.h / .c          # 磁盘块模拟
    ├── bitmap.h / .c        # 空闲位图
    ├── main.c
    └── Makefile
```

---

## 代码规范

- **标准**：C99
- **编译选项**：`-Wall -Wextra -Werror -std=c99 -O2`
- **错误处理**：所有 `malloc` / 系统调用均检查返回值
- **内存安全**：使用 `common/` 中的 `safe_malloc` / `safe_calloc` 封装
- **并发安全**：pthread 互斥锁 + POSIX 无名信号量

---
