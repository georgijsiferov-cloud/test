# Adaptix Framework - Beacon Agent

这个仓库包含Adaptix Framework的Beacon Agent系统实现。

## 项目结构

```
.
├── README.md                    # 本文档
├── .gitignore                   # Git忽略文件配置
└── modern_cpp_beacon/           # 现代C++实现的Beacon系统
    ├── CMakeLists.txt           # CMake构建配置
    ├── README.md                # 详细的实现文档
    ├── include/                 # 头文件
    │   └── adaptix/beacon/      # Beacon相关头文件
    └── src/                     # 源代码实现
```

## modern_cpp_beacon 目录

这是一个完全基于现代C++17特性实现的Beacon Agent系统，完全遵循Google C++代码规范。

### 主要特点

✅ **现代C++特性**
- 智能指针（`std::unique_ptr`、`std::shared_ptr`）
- 移动语义
- 强类型枚举（`enum class`）
- `std::optional` 用于可选值
- RAII资源管理
- 标准线程库和原子操作

✅ **Google C++规范**
- 正确的命名约定（PascalCase类名、snake_case函数名）
- 头文件保护
- `.h`/`.cc` 文件扩展名
- Const正确性
- 显式删除不需要的特殊成员函数

✅ **架构设计**
- 清晰的职责分离
- 接口与实现分离
- 可扩展的传输层
- 灵活的任务执行引擎

### 快速开始

```bash
# 进入目录
cd modern_cpp_beacon

# 构建项目
mkdir build && cd build
cmake ..
make

# 运行示例
./beacon_agent
```

### 详细文档

- [`SUMMARY.md`](SUMMARY.md)：现代C++重构总结
- [`modern_cpp_beacon/README.md`](modern_cpp_beacon/README.md)：项目说明
- [`modern_cpp_beacon/ARCHITECTURE.md`](modern_cpp_beacon/ARCHITECTURE.md)：架构详情
- [`modern_cpp_beacon/EXAMPLES.md`](modern_cpp_beacon/EXAMPLES.md)：使用示例

## 技术栈

- **语言**: C++17
- **构建系统**: CMake 3.14+
- **编译器**: GCC 7+、Clang 5+、MSVC 2017+
- **线程库**: pthread (Unix/Linux)

## 设计原则

1. **类型安全**: 避免使用void*，使用模板和强类型
2. **内存安全**: 使用RAII和智能指针，避免内存泄漏
3. **线程安全**: 使用现代并发原语
4. **可维护性**: 遵循Google代码规范，代码清晰易读
5. **可扩展性**: 基于接口的设计，易于扩展新功能

## 与传统C/C++混合编程的区别

| 特性 | 传统C/C++混合 | 现代C++ |
|------|---------------|---------|
| 内存管理 | malloc/free, new/delete | 智能指针、RAII |
| 错误处理 | 错误码、NULL指针 | std::optional、异常 |
| 线程 | pthread原始API | std::thread、std::mutex |
| 类型安全 | void*、C风格转换 | 模板、C++转换 |
| 资源管理 | 手动释放 | RAII自动管理 |
| 枚举 | enum | enum class |
| 可选值 | NULL/-1/魔数 | std::optional |

## 贡献

欢迎提交Issue和Pull Request来改进这个项目。

## 许可证

MIT License

---

**注意**: 这是一个参考实现，用于展示现代C++最佳实践。在生产环境中使用前，请确保添加适当的安全措施、错误处理和测试。
