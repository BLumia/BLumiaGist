## Rust Starter Note

### 前注

Rust 官方提供一本叫 [TRPL(The Rust Programming Language)](https://doc.rust-lang.org/book/) 的手册（这里有一个较为活跃的 [中文翻译版本](https://kaisery.github.io/trpl-zh-cn/) ），希望系统学习的话应该看那个。另外，通过 `rustup doc` 可以查看本地文档，其中就包含了 TRPL 。

### 安装 Rust

在 Linux 上安装不应该成为什么问题，Windows 的话按 TRPL 的说法，通过 `rustup` 装好 Rust 工具链之后，作为依赖还需要装 [Visual Studio 2017 生成工具](https://www.visualstudio.com/zh-hans/downloads/) ，不过本来有 VS 在电脑上，想防止装上多余的东西的话，只需要确认 **VC++ 工具集**、**Windows 通用 CRT SDK** 和 **Windows SDK** 装好了就可以了（说法来自[这里](https://github.com/rust-lang-nursery/rustup.rs/issues/1003#issuecomment-289809890)）。

### 构建和项目管理工具

Rust 的编译器是 `rustc` ，似乎没有提供自己的链接器（比如 windows 上要装 VC++ 工具集，会用到里面的 `link.exe`），项目管理工具（兼作包管理工具）则是 `cargo` 。

``` shell
$ rustc main.rs # 编译单个文件
$ cargo new hello_cargo # 使用 cargo 创建名为 hello_cargo 的应用程序项目
$ cargo build # 需在项目目录下，构建项目
$ cargo run # （编译并）运行项目
```

作为官方提供的项目管理工具，推荐在小的项目也使用 Cargo 而不是使用 `rustc` 手动构建。Cargo 还提供内建的简单 git 支持（创建项目时还会帮你初始化 git ）。Cargo 常用的指令如下：

``` shell
$ cargo new hello_cargo --bin # 其中 bin 代表使用 binary 项目模板创建项目（缺省），对应的，还有 --lib 表示 library
$ cargo check # 进行编译检查但不进行编译
$ cargo build # 使用 --release 参数可构建 release 版
$ cargo run # （编译并）运行项目
$ cargo doc --open # 构建该项目的文档并在浏览器中打开（其中包含项目所依赖的 crate 的文档，所以很有用）
```

Rust 的第三方代码包/库称作 *creat* ，二进制 crate 生成一个可执行文件，库 crate 则可以作为依赖被使用。对于由 cargo 创建的项目，依赖记录在项目目录下 `Cargo.toml` 内的 `[dependencies]` 段中。如：

``` toml
[dependencies]

rand = "0.3.14"
```

这种版本记法称为语义化版本，`0.3.14` 事实上是 `^0.3.14` 的简写，它表示 “任何与 0.3.14 版本公有 API 相兼容的版本”。在更新该文件后执行 `cargo build` 时即会按照新的依赖同步获取所需的 crate 。另外在首次运行 `cargo build` 时会创建一个叫 `Cargo.lock` 的文件（该文件仅应当被 cargo 管理，我们不应当编辑它）来记录我们实际使用的版本（由 Cargo.toml 内的依赖版本记录计算得到），当我们使用 `cargo update` 时会显示的升级我们实际使用的 crate 并（在成功的情况下）帮我们更新 `Cargo.lock` 的内容（注意：这不会更新 `Cargo.toml` 的内容）。

换句话说， `Cargo.toml` 是理论支持的版本，而 `Cargo.lock` 是记录了本地开发所使用的版本。不过目前我有个问题，明显 `.lock` 中记录的内容很可能和所用的操作系统相关（比如该例子中 `rand` 在 Windows 下会依赖 `winapi` 这个 crate ）。如果对于跨平台的应用，该文件是否应当被 git 跟踪？

### 语法速览 - 猜猜看程序

``` rust
// TRPL 的 猜猜看 示例 2-1
use std::io;

fn main() {
    println!("Guess the number!");

    println!("Please input your guess.");

    let mut guess = String::new();

    io::stdin().read_line(&mut guess)
        .expect("Failed to read line");

    println!("You guessed: {}", guess);
}
```

使用 `use` 引入使用到的库，这个例子中 io 库来自标准库（`std`）。

使用 `fn` 声明函数。Rust 程序的入口函数即 `main()` ，另外即便函数只有一行时 `{}` 也是不可省略的。另外，如 `println!` 这类的，由感叹号结尾的表示这是一个宏而非函数。

使用 `let` 关键字声明变量，但 rust 中变量默认是不可变（immutable）的，故需要 `mut` 声明可变（mutable）的变量。另注意这里是使用了类型推断，故没有显式的声明类型。

String::new() 中，`::new()` 是 String 的一个 **关联函数**（associated function），在其它语言中即 **静态函数** 或称 **静态方法**。

`read_line(&mut guess)` 中 `&` 表示这个参数是一个 **引用**（reference），默认也是不可变的故需要使用 `mut` 。 Rust 的引用好像和 C 的工作方式并不一样，待弄清楚。

`read_line` 返回一个 [`Result`](https://doc.rust-lang.org/std/result/enum.Result.html) 泛型的值（这里是 [`io::Result`](https://doc.rust-lang.org/std/io/type.Result.html)）。Result 的类型是 **枚举**（enumerations）（但它的意思和别的语言的枚举也有很大区别...）。枚举类型持有固定集合的值，这些值被称为枚举的 **成员**（variants）。对于 `Result`，它的成员是 `Ok` 或 `Err`。

`io::Result` 的实例拥有 `expect` 方法，故 `io::Result` 的实例值为 `Err` 时，`expect` 会导致崩溃。简单的处理办法则是使用 `.expect()` ，出现异常时输出提示，任其崩溃。另外，此类表明可能导致异常的返回值可以不做检查处理（即不写这里的 `.expect()`），但编译器会警告这里存在潜在的问题。

输出那行没什么好解释的，只需要知道 `!` 意味着 `println!` 是个宏而不是个函数。

``` rust
// TRPL 的 猜猜看 示例 2-3
// 需要 rand = "^0.3.14" 依赖
extern crate rand;

use std::io;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess the number!");

    let secret_number = rand::thread_rng().gen_range(1, 101);

    println!("The secret number is: {}", secret_number);

    println!("Please input your guess.");

    let mut guess = String::new();

    io::stdin().read_line(&mut guess)
        .expect("Failed to read line");

    let guess: u32 = guess.trim().parse()
        .expect("Please type a number!");

    println!("You guessed: {}", guess);
	
	match guess.cmp(&secret_number) {
        Ordering::Less => println!("Too small!"),
        Ordering::Greater => println!("Too big!"),
        Ordering::Equal => println!("You win!"),
    }
}
```

注意先去 `Cargo.toml` 把 rand 这个依赖加上，方法上面有。

通过 `extern crate <crate_name>` 标注引入外部 crate （同时还会调用对应的 `use <crate_name>`）。对于该例子，相当于自动 `use rand` 了。

由于我们需要使用 `Rng` 这个 trait ，其定义了随机数生成器应实现的方法，想使用这些方法则此 trait 必须在作用域中，故我们需要 `use rand::Rng`。

`let guess: u32` 则为显式的类型声明，由于字符串的 `parse()` 可以返回多种数字类型，故这里需要显示的指出类型为 `u32` （32位无符号整型，这类型名字不知道是不是来自 linux 内核用的那些..）。

Rust 允许 **隐藏**（shadow），故上面第二次的 `u32` 类型的 `guess` 隐藏了第一次声明的 `guess` ，这在需要变量类型转换的时候比较有用（不用搞新变量了）。

另外需要注意的是，上面所述的 parse() 的返回值其实也是个 Result 枚举，然后 `.except()` 会在返回的是 `Result::Ok` 的情况下返回 `Ok` 这个成员里的值（现在知道这个枚举的独特之处了吧）。

`cmp` 方法返回了 `Ordering` 类型的枚举。后面的 `match` 则是根据枚举的情况做对应的处理，类似其他语言的 `switch-case` 。

``` rust
// TRPL 的 猜猜看 示例 2-6
extern crate rand;

use std::io;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess the number!");

    let secret_number = rand::thread_rng().gen_range(1, 101);

    loop {
        println!("Please input your guess.");

        let mut guess = String::new();

        io::stdin().read_line(&mut guess)
            .expect("Failed to read line");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        println!("You guessed: {}", guess);

        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }
    }
}
```

`loop` 提供了一个无限循环。可以使用 `break` 跳出循环。

已经知道了 Result 是一个枚举，故我们可以使用 match 来处理异常。最终 `let guess: u32 = ` 的右侧的值会是 `match` 所 match 的内容（一个表达式，关于表达式请见下面的描述）。

以上涵盖了 Rust 的最基础的一些语法。

### 语法速览 - 常规

Rust 是静态类型语言，使用 let 声明变量。 Rust 可以进行类型推断，但无法推断时需要显式的声明类型。

Rust 的 char 类型代表了一个 Unicode 标量值。

Rust 的符合类型包含元组（tuple）和数组（array）。元组是一个将**多个**（没说三个）其他类型的值组合进一个复合类型的主要方式。数组中的每个元素的类型必须相同且数组是定长的。

``` rust
fn main() {
    let tup = (500, 6.4, 1); // 相当于：let tup: (i32, f64, u8) = (500, 6.4, 1);
    let (x, y, z) = tup; // 这叫做 解构（destructuring）
    println!("The value of y is: {}", y);
	let six_point_four = tup.1; // 可以使用点号后跟值的索引来直接访问
	
	let a = [1, 2, 3, 4, 5]; // 数组
    let first = a[0];
    let second = a[1];
}
```

Rust 中函数的声明位置是无所谓的。有返回值的函数在一个箭头 `->` 后声明返回值的类型，函数的返回值等同于函数体最后一个表达式的值。

``` rust
fn main() {
    another_function(5, 6);
	let x = five();
    println!("The value of x is: {}", x);
}

fn another_function(x: i32, y: i32) {
    println!("The value of x is: {}", x);
    println!("The value of y is: {}", y);
}

fn five() -> i32 {
    5
}
```

Rust 是个基于表达式的语言，函数调用是一个表达式。宏调用是一个表达式。创建新作用域的大括号（代码块）`{}` 也是一个表达式。函数和表达式都会返回值。如果一个 `{}` 内没有表达式而都是语句，那 `{}` 构成的表达式将返回一个空的元组（这什么设计..）

``` rust
fn main() {
    let y = {
        let x = 3;
        x + 1 // 这里没有分号，如果在表达式的结尾加上分号，他就变成了语句，{} 这段返回的就是个空元组了。
    };
    println!("The value of y is: {}", y);
}
```

Rust 的 `if` 后面不用括号，但条件必须是 bool 类型，非 bool 类型的表达式不会被转换为 bool 类型。另， `if` 语句也是表达式。此外，`if` 若是多分支的，则各分支的返回值类型应当是相同的，

Rust 的 `while` 循环条件同样不需要括号，但它不是表达式（不太确定，尝试得到的结果是会提示 expected type `()` ，当然也可以把结果（空元组）赋值给变量，然而...好像没什么卵用吧）。另外，使用 `while` 循环通过下标迭代数组会更慢，因为编译器增加了运行时代码来对每次循环的每个元素进行条件检查。

``` rust
fn main() {
    let condition = true;
    let number = if condition {
        5
    } else {
        6
		//"six"
    };
    println!("The value of number is: {}", number);
	
	let a = [10, 20, 30, 40, 50];
    let mut index = 0;
    while index < 5 {
        println!("the value is: {}", a[index]);
        index = index + 1;
    }
}
```

Rust 的 `for` 循环如下，提供一个 `Range` 供遍历。

``` rust
fn main() {
    let a = [10, 20, 30, 40, 50];
    for element in a.iter() {
        println!("the value is: {}", element);
    }
	
	for number in (1..4).rev() { // rev 反转一个 Range 里的东西
        println!("{}!", number);
    }
    println!("LIFTOFF!!!");
}
```