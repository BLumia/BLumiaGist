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

Rust 的复合类型包含元组（tuple）和数组（array）。元组是一个将**多个**（没说三个）其他类型的值组合进一个复合类型的主要方式。数组中的每个元素的类型必须相同且数组是定长的。

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

关于所有权，Rust 的规则是：

 - Rust 中每一个值都有一个称之为其 **所有者**（owner）的变量。
 - 值有且只能有一个所有者。
 - 当所有者（变量）离开作用域，这个值将被丢弃。

就是说，对于一个值，仅可能存在一个对其的引用（叫做所有者的变量），即一个值仅能有一个所有者。然后变量在作用域失效后即销毁（调用 `drop` ）。这样保证内存安全。其中，所有者仅只能有一个这一点确保了内存不会被多次释放，下面的例子中，字符串的所有权被移动给了 `s2` ，此时 `s1` 就失效不能再使用了。
 
对于赋值时的所有权转让，仅仅在非 `Copy` trait 注解的类型上发生（比如 `String` ）。具有 Copy 注解的类型通常有固定的长度，使用时是分配在栈上的，故创建拷贝的速度很快，没有理由进行额外的内存管理措施（所有权那一套）。而非 `Copy` 注解的类型通常则分配在堆上，其长度不固定，故使用此类内存管理措施。
 
``` rust
fn main() {
    let s1 = String::from("hello");
    let s2 = s1; // 如果写成 s1.clone() 就不一样了

    println!("{}, world!", s1); // 此时 s1 已经失效了
}
```

值得一提的是，对于非 `Copy` 对象，在作为参数传递给函数的时候，所有权该拿走的还是会被拿走。

``` rust
fn main() {
    let s1 = String::from("hello");

    let len = calculate_length(&s1); // &String s 指向 String s1, s1 实际指向 "hello" 值的位置

    println!("The length of '{}' is {}.", s1, len);
}

fn calculate_length(s: &String) -> usize {
    s.len()
}
```

为了防止对于像 String 这样非的 `Copy` trait 类型在作为参数传递给函数的时候所有权被拿走并由于函数生命周期的结束而被销毁，做法之一是在函数返回的时候把所有权再交给新的变量，另外的做法就是在传递参数时只传递一个引用而不是传递所有权。

`&s1` 语法允许我们创建一个 指向 值 `s1` 的引用，但是并不拥有它。因为并不拥有这个值，当引用离开作用域时其指向的值也不会被丢弃。我们将获取引用作为函数参数称为 **借用**（borrowing）。

当需要改变引用时，可以使用 `mut` 关键字，当然，所要借用的变量也需要是可变的，类似这样： `some_string: &mut String` 。

为了避免数据竞争， **在特定作用域中的特定数据有且只有一个可变引用** 。另外，为了避免不可变的值被意外的改变，不允许同时使用不可变引用和可变引用（因为如果这样的话，可变引用可以修改值，而这时不可变引用的值就...变了！）。

即：

 - 在任意给定时间，只能 拥有如下中的一个：
    * 一个可变引用。
    * 任意数量的不可变引用。
 - 引用必须总是有效的。

``` rust
// fn first_word(s: &String) -> &str { 参数传递为 String 的引用
fn first_word(s: &str) -> &str { // 参数传递为 string 的 slice ，返回值也是 string slice。
    let bytes = s.as_bytes();
    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[0..i];
        }
    }
    &s[..]
}
fn main() {
    let my_string = String::from("hello world");
    let word = first_word(&my_string[..]); // 根据函数参数，仅可用于 slice
    let my_string_literal = "hello world"; // 字符串字面值其实就是个字符串 slice
    let word = first_word(&my_string_literal[..]); // 所以可以这样
    let word = first_word(my_string_literal); // 也可以这样
    
    let a = [1, 2, 3, 4, 5];
    let slice = &a[1..3]; // 类型是 &[i32]
}
```

Slice 是另一个 **没有所有权** 的数据类型，允许引用集合中一段连续的元素序列。Slice 的类型记法好像很奇怪，前面的 `&` 我目前理解为表示它不具所有权（是个引用），而 String 的 slice 类型叫 `&str` 不太清楚原因。对于其它类型的 Slice ，例如上面的，为 `&[i32]` 。

``` rust
#![allow(unused)]
struct User { // 这是一个结构体，其内成员称为字段
    username: String,
    email: String, // 访问字段的指使用点号，user1.email
    sign_in_count: u64,
    active: bool,
}

fn main() {
    let user1 = User { // 若需要可变，则应该使用 `let mut user1`
        email: String::from("someone@example.com"),
        username: String::from("someusername123"),
        active: true,
        sign_in_count: 1,
    };
    
    let user2 = User {
        email: String::from("another@example.com"),
        username: String::from("anotherusername567"), // 试着注释掉这一行，那么在下一行，username的所有权就消失了
        ..user1 // 结构体更新语法，表示其余值来自 user1 变量中实例的字段
    };
    
    let user3 = build_user(user2.email, user2.username); // 前两个字段的所有权被拿走了！
    
    println!("Email {}", user3.email);
    // println!("Email {}", user2.email); 所有权被拿走了！不能这么做
    println!("Sign-in Count {}", user2.sign_in_count); // 但是这样可以
}

fn build_user(email: String, username: String) -> User { // 别忘了，这里传的不是引用，会拿走所有权
    User {
        email, // 变量与字段同名的话，可以使用 字段初始化简写语法
        username,
        active: true,
        sign_in_count: 1,
    }
}
```

如上定义了 **结构体** `User`，结构体包含若干字段。结构体的原型不能包含默认值，构建结构体时也应当提供所有字段的值。结构体另有一些简单的记法，参见上述代码中的注释即可。

另外，上面的代码中使用了 `String` 这种自身拥有所有权的类型而不是引用（`&str`）。结构体可以储存被其他对象拥有的数据的引用，不过这么做的话需要用上 **生命周期** （lifetimes），以确保结构体引用的数据有效性跟结构体本身保持一致（后述）。

比较奇怪的是，如上面注释所述，并不能保证一个结构体实例的所有字段的所有权始终由结构体实例（的字段本身）持有。尽管这样可能造成的错误可以在编译期被推断而报告错误，但这种行为还是很奇怪。

``` rust
fn main() {
    struct Color(i32, i32, i32);
    struct Point(i32, i32, i32);

    let black = Color(0, 0, 0);
    let origin = Point(0, 0, 0);
}
```

可以定义与元组类似的结构体，称为 **元组结构体** 。有着结构体名称提供的含义，但没有具体的字段名，只有字段的类型。但需要注意上面代码中 black 和 origin 值是 **不同** 的类型，因为它们是不同的元组结构体的实例。

也可以定义一个没有任何字段的结构体！它们被称为 类单元结构体（unit-like structs）因为它们类似于 `()`，即 unit 类型。类单元结构体常常在你想要在某个类型上实现 trait 但不需要在类型内存储数据的时候发挥作用。


``` rust
#[derive(Debug)] // 增加注解来派生 `Debug` trait
struct Rectangle {
    width: u32,
    height: u32,
}

fn main() {
    let rect1 = Rectangle { width: 30, height: 50 };

    println!("rect1 is {:?}", rect1); // 可以使用 `{:#?}` 得到更好看的调试输出
}
```

`println!` 宏能处理很多类型的格式，`{}` 默认告诉 println! 使用被称为 `Display` 的格式，而结构体并没有提供一个默认的 `Display` 实现。加入 `:?` 指示符（使用 `{:?}`）告诉 println! 我们想要使用叫做 `Debug` 的输出格式。`Debug` 是一个 trait，允许我们在调试代码时以一种对开发者有帮助的方式打印出结构体。

Rust 确实 包含了打印出调试信息的功能，不过我们必须为结构体显式选择这个功能。为此，在结构体定义之前加上 `#[derive(Debug)]` 注解。

``` rust
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle { // Rectangle 的实现块
    fn area(&self) -> u32 {
        self.width * self.height
    }
    
    fn square(size: u32) -> Rectangle {
        Rectangle { width: size, height: size }
    }

    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}

fn main() {
    let rect = Rectangle {
        width: 10,
        height: 20
    };
    
    let rect2 = Rectangle::square(5); // 使用关联函数
    
    println!("{} {}", rect.area(), rect.can_hold(&rect2)); // 使用方法
}
```

不同于别的语言中的叫法，Rust 中称乎 位于结构体的上下文中被定义的~~成员函数~~ 为 **方法** （Method），而 位于结构体的上下文中被定义的~~静态函数~~ 则被称为**关联函数**（associated functions）。其中~~成员函数~~方法（的原型中）的第一个参数总是 `&self` ，表示调用该方法的结构体实例。而~~静态函数~~关联函数则不以 self 作为第一个参数，原因不说也知道了..

有趣的事实：对于 C++ ，当调用指针对象的成员函数时，我们需要 `->` 来对指针进行解引用（`object->something() 就像 (*object).something()`），而 Rust 中有自动引用和解引用的功能，进行方法调用时， Rust 会自动增加 `&`、`&mut` 或 `*` 以便使符合方法的签名。

调用方法通过点号即可，而调用关联函数则通过 `::` 进行。他们的声明被放在 `impl` 块中，而一个结构体也可以有多个 `impl` 块存在。

``` rust
fn main() {
    enum IpAddrKind {
        V4,
        V6,
    }
    let four = IpAddrKind::V4;
    let six = IpAddrKind::V6;
}
```

枚举可以像大多语言那样使用，就如上面的例子一样。然而别忘了在“猜猜看”中涉及到的枚举的特性（下述）。

``` rust
fn main() {
    enum Message {
        Quit, // 没有关联任何数据
        Move { x: i32, y: i32 }, // 包含一个匿名结构体
        Write(String), // 包含单独一个 String
        ChangeColor(i32, i32, i32), // 包含三个 i32
    }

    impl Message {
        fn call(&self) { // &self 就是枚举的值了
            // method body would be defined here
        }
    }

    let m = Message::Write(String::from("hello"));
    m.call();
}
```

Rust 的枚举的每个成员都可以包含（或称 **关联** ）若干个任意类型的变量。另外如同结构体一样，枚举也可以有~~成员函数~~方法。

``` rust
//enum Option<T> {
//    Some(T),
//    None,
//}
fn main() {
    let some_number = Some(5);
    let x: i8 = 5;
    let y: Option<i8> = None;//Some(5);
    
    // let sum = x + y; 编译器会告诉你不能这样做
    let sum = x + match y {
        Some(x) => x,
        None => 0
    };
    
    println!("{}", sum);
}
```

为了避免不可意料的错误，Rust 并没有 `null` 这个概念，但我们有时仍需表达 null 这种状态（一个 nullable 的值），这时我们可以使用 Option 枚举。Option 和它的成员均位于 prelude 中，所以我们不需要显式引用作用域就可以使用他们了。

如上面注释所属的原型，其包含一个 Some 成员（持有一个泛型 `T` ，泛型后述，可理解为 C++ 的模板类型）和一个 None 成员。上面提到没有 null 概念是为了防止不可意料的错误，而使用枚举时自然编译器不会允许一个枚举值直接参与运算或进行类似的操作，进而在编译期就避免了问题发生。如上面代码，我们不能直接进行 `x + y` 运算。在这个例子中使用了 `match` 处理 y 的不同情况。

另外，使用 Some 时编译器可以轻松的推断类型，而使用 None 时则自然不能够推断。故当使用 None 时我们需要显式的给变量声明类型。

``` rust
fn main() {
    let some_u8_value = Some(3u8);
    
    match some_u8_value {
        Some(3) => println!("three"),
        _ => (),
    }
    
    if let Some(3) = some_u8_value {
        println!("three");
    }
    
    let somevalue = if let Some(i) = some_u8_value {
        println!("{}", i); i
    } else {
        0
    };
    
    println!("{}", somevalue);
}
```

如同其它语言的 `switch-case` 一样，Rust 使用 `match` 进行模式匹配，但有一些不同之处。

`match` 后所跟的可以是任何类型，而不像 `if` 后必须跟布尔类型。分支的匹配规则（称为**模式**）则必须涵盖所有的可能性，并使每一个分支都返回一个值作为 `match` 表达式的返回值。最终 `match` 会试图根据所有模式依次匹配直到找到第一个能够匹配的模式并返回其后的表达式得到的值。注意，分支的返回值类型要求是相同的。

当不能或者不便枚举所有类型/情况的情况下，模式可以使用通配符 `_` ，类似其他语言中 `switch` 的 `default` 关键字。

`if let` 相当于 `match` 只针对一个模式时的语法糖，后跟由等号分割的一个模式和一个表达式（顺序不可颠倒）。也可以加 else 来做不匹配所给定的模式时的情况。另外 `let if` 后面可以不刻意的返回值（但别忘了花括号构侧的依然是表达式，只不过值是 `()`）。

`if let` 比较奇怪的地方大概就是这个等号不是做赋值的作用的（而是存粹的一个分隔符，尽管语义上可以表示判断是否相等）。不过对于前面涉及到的例子，等号也不是做赋值的作用，其中对于带 `Copy` trait 的变量是做拷贝，而不带的则是做所有权的转让，以及传递引用。

``` rust
// 该例子不易表述，请配合参阅 TRPL Ch.7-1 
// 此段无法编译执行
mod client {
    fn connect() {
    }
}

mod network {
    fn connect() {
    }

    mod server {
        fn connect() {
        }
    }
}
```

``` plain
// 假设 $ cargo new communicator --lib
// 这是模块结构
communicator
 ├── client
 └── network
     └── server
// 这是如果分散到不同文件内定义模块时文件的结构
└── src
    ├── client.rs
    ├── lib.rs
    └── network
        ├── mod.rs
        └── server.rs
```

可以通过 cargo 的 `--lib` 参数来创建一个库项目（上面说过的）。

Rust 使用 `mod` 关键字表示模块，后面跟名字和大括号表示模块内的东西。如上面则表示了库（假定叫）communicator 内的模块结构。

cargo 管理的项目，模式使用 `src/lib.rs` 作为库的入口，我们可以在一个文件中完成所有模块结构的实现，也可以分散到不同的文件中。

使用 `mod` 后跟名字和分号的时候，表示包含同级目录下的 `名字.rs` 或者 `名字/mod.rs` 到当前文件。后者对应包含子模块的模块，前者对应不包含子模块的模块。

因为上面说的是包含，可以当成 php 的 `require()` 或者 c 的 `#include` 之类。也因此，在被包含的文件内，不再需要重新定义当前模块的名字，直接写实现就是了。

另外编译时 Rust 会警告没被使用的函数。

``` rust
// src/lib.rs
pub mod client;
pub mod network;

// src/network/mod.rs
pub fn connect() {
}
mod server;
```

Rust 使用 `pub` 关键字表示函数或模块是公有的。因为其是开放的，故其它外部程序就可以使用这些函数或模块，此时编译器就不会因为这些函数或模块没有被使用而发出警告。

Rust 的模块访问可见性这个应该不用记，猜也猜得到的规则。

``` rust
pub mod a {
    pub mod series {
        pub mod of {
            pub fn nested_modules() {}
        }
    }
}

enum TrafficLight {
    Red,
    Yellow,
    Green,
}

use TrafficLight::*;
use a::series::of;

fn main() {
    of::nested_modules(); // a::series::of::nested_modules();
    let red = Red; // TrafficLight::Red
}
```

不难看出 这里的 `use` 以及 `::` 是什么意思。以及，类似别的语言，`use` 关键字只将指定的模块引入作用域；它并不会将其子模块也引入。这就是为什么想要调用 `nested_modules` 函数时仍然必须写成 `of::nested_modules` 。

这里的 `*` 是 **glob 运算符** （glob operator），会将某个命名空间下的所有名称都引入作用域。

当需要访问上层模块内的东西时，可以使用 `super` 关键字，如 `super::client::connect();` ，`super` 也可以用在 use 中，如 `use super::client;`。

``` rust
let v: Vec<i32> = Vec::new();
let v = vec![1, 2, 3];
```

Rust 标准库中包含一系列被称为 [**集合**](https://doc.rust-lang.org/std/collections)（collections）的非常有用的数据结构，其中常用的包括 vector ， string 和 hash map 。

部分集合类型使用泛型来实现，如 vector 。另外集合类型的所有权也是比较需要特别关注的地方。

``` rust
fn main() {
    let mut v = Vec::new();
    v.push(5);
    v.push(6);
    v.pop();
    let does_not_exist = v.get(404);
    
    println!("{}", &v[0]); // 传递的是一个引用
    println!("{}", v[0]); // 传递一个拷贝（对于 copy trait 的类型）
    println!("{:#?}", does_not_exist);
    println!("{}", match does_not_exist {
        Some(&x) => x,
        None => 61
    });
}
```

这一段和上一段示例代码一起看。可以使用 `Vec::new()` 构建 vector , Rust 也提供了宏 `vec!` 来构建 vector 。由于可以通过操作（比如 push ）推断泛型的类型，故显式的声明类型并不总是必要的。

访问 vector 的元素可以使用两种方式，使用 `&` 和 `[]` 返回一个引用，或者使用 `get()` 方法以索引作为参数来返回一个 `Option<&T>` 。其中前者注意 `&` （暂不清楚不写 `&` 是不是就是按照是否是 copy trait 类型来决定得到的是所有权还是拷贝），后者需要注意的则是传递的 Option 内的是 `&T` 而不是 `T` 。两种访问方式中，前者如果访问越界的下标则会导致崩溃，后者访问则不会崩溃以便做错误处理。

vector 有 push 有 pop ，干啥用的用脚后跟想一想。

``` rust
fn main() {
    let mut v = vec![100, 32, 57];
    for i in &mut v {
        *i += 50;
        println!("{}", i);
    }
    
    #[derive(Debug)]
    enum SpreadsheetCell {
        Int(i32),
        Text(String),
    }

    let row = vec![
        SpreadsheetCell::Int(3),
        SpreadsheetCell::Text(String::from("blue")),
    ];
    
    println!("{:#?}", &row[0]);
}
```

可以像数组一样遍历 vector ，对于需要改变值的情况，依然需要标 `mut` 。

另外， vector 里的东西的类型可以是枚举，故可以通过这种形式实现一个 vector 存多种不同类型的东西。

``` rust
fn main() {
    let mut str0 = String::new();

    let str_slice = "initial contents";
    let str1 = str_slice.to_string();
    let str2 = String::from("initial contents");
    let str3 = str1 + &str2; // 注意，str1的所有权这就没了..
    str0.push('f');
    str0.push_str("uck");
    str0.push_str(&str3);
    let str4 = format!("{}-{}-{}", str_slice, str0, str3);
    
    println!("{}", str4); // println! 这个宏似乎并不会拿所有权
}
```

Rust 的核心语言中的字符串其实只有 `str` （字符串 slice）这么一种（通常以被借用的形式 `&str` 出现）。称作 `String` 的类型是由标准库提供的，而没有写进核心语言部分。

String 有 `push` 来往里追加字符类型，和 `push_str` 追加 `String` 或者 `&str`。

String 重写了 `+` 运算符，但注意上面示例中的加号前后并非故意示例所有权而这样写，而是必须这样写，原因是其原型类似 `fn add(self, s: &str) -> String {` （这并不是标准库中实际的签名，标准库中的 `add` 使用泛型定义）。

``` rust
fn main() {
    let fsck_str = String::from("fsck");
    let fsck = fsck_str.as_str();
    let woc = "卧槽";
    let f = &fsck[0..1];
    //let panic = &woc[0..1]; // 会导致崩溃！
    
    println!("{}", f);
    for c in woc.chars() {
        println!("{}", c);
    }
}
```

String 是一个 `Vec<u8>` 的封装，但不支持下标索引。由于 Rust 的字符串（无论 `str` 还是标准库中的 `String`）都存储的是 utf-8 字符串，故下标索引有时候会返回非期望的内容导致错误，所以这是 Rust 选择 String 不支持下标索引的原因之一...（ps. ~~`&"hello"[0]` 会返回 104 而不是 h~~ 这个说法应该是错的，`str` 也不能这样索引）。

当需要类似的下标访问需求时，我们则需要操作一个 `&str` 而不是 `String` 了。使用 `String::as_str()` 或 `String::as_mut_str()` 等之类的方式得到 `&str` 以便操作。但需要注意，依然不能通过下标直接访问，而是要接着 slice 。

接着 slice 依然是很有可能导致崩溃的，如上面例子中那行被注释掉的语句如果取消注释就会导致程序在运行时 panic ...

也可以使用 `.chars()` 来得到每个字母（字形簇），也可以使用 `.bytes()` 得到每个字节...

``` rust
fn main() {
    use std::collections::HashMap;

    let teams  = vec![String::from("Blue"), String::from("Yellow")];
    let initial_scores = vec![10, 50];
    
    let scores_from_collect: HashMap<_, _> = teams.iter().zip(initial_scores.iter()).collect();
    let mut scores = HashMap::new();
    
    scores.insert(String::from("Blue"), 10);
    scores.insert(String::from("Blue"), 25); // 覆盖
    scores.insert(String::from("Yellow"), 50);
    scores.entry(String::from("Blue")).or_insert(50); // 检查是否存在，不存在则插入
    
    let team_name = String::from("Blue");
    let _score = scores.get(&team_name);
    
    println!("{:?}", scores_from_collect);
    for (key, value) in &scores {
        println!("{}: {}", key, value);
    }
}
```

HashMap 由于不常用因而没有内置到 prelude 中，所以需要自己 use ，废话是，所有的键必须是相同类型，值也必须都是相同类型。

除了上面所示的方法之外还可以通过包含了元组（作为键值对）的 vector 来构建，通过调用 vector 的 `collect` 方法来得到一个集合类型（其类型就可以是 HashMap ）。由于类型并不是确定的，故需要显式的声明类型为 `HashMap<_, _>` 。

另外，无论是键还是值，传递给 HashMap 的值的所有权会被移交。

entry 是一个枚举，表示可能存在或不存在的一个值。`or_insert` 则会在不存在的时候插入并返回一个可变引用（`&mut V`），我们可以使用 `*` 解引用并更新变量的值。

``` rust
// 此段无法编译执行
use std::fs::File;

fn main() {
    panic!("crash and burn");
    
    let f = File::open("hello.txt");
    let f = match f {
        Ok(file) => file,
        Err(error) => {
            panic!("There was a problem opening the file: {:?}", error)
        },
    };
    
    let f = File::open("hello.txt").unwrap();
    
    let f = File::open("hello.txt").expect("Failed to open hello.txt");
    
    fn read_username_from_file() -> Result<String, io::Error> {
        let f = File::open("hello.txt");
        let mut f = match f {
            Ok(file) => file,
            Err(e) => return Err(e),
        };
        let mut s = String::new();
        match f.read_to_string(&mut s) {
            Ok(_) => Ok(s),
            Err(e) => Err(e),
        }
    }
    
    fn read_username_from_file() -> Result<String, io::Error> {
        let mut s = String::new();
        File::open("hello.txt")?.read_to_string(&mut s)?; // 与下面两行效果一致
        // let mut f = File::open("hello.txt")?;
        // f.read_to_string(&mut s)?;
        Ok(s)
    }
}
```

在遇到 `panic!` 时程序会 panic （废话），此时默认情况程序会回溯栈并清理它遇到的每一个函数的数据，当然也可以选择直接终止（abort）并让操作系统处理回收程序被分配的空间，可以在 cargo 配置文件的 profile 段（比如 `[profile.release]` ）内增加 `panic = 'abort'` 配置。

当我们需要可以处理的异常的时候，应当使用 `Result` 枚举而不是立即 panic 。我们可以使用 `match` 来常规的处理内容，也可以使用能够自定义 panic 提示的 `expect()` 或者使用默认 panic 提示的 `unwrap()` ，他们会在非错误时返回 `Result` 中 `Ok` 的值，而在错误时 panic 。

需要提供错误处理支持时，也只需返回 Result 枚举，除了常规的返回枚举内容之外，可以使用 `?` 简化写法。 `?` 在 `Ok` 时返回 `Ok` 中的值，在 `Err` 时则直接把 `Err` 作为 `?` 所在函数的返回值（这时相当于写了 `return`）。因而， `?` 只能使用在类型为 `Result` 枚举的函数内。

``` rust
struct Point<T> {
    x: T,
    y: T,
}

impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}

impl Point<f32> {
    fn distance_from_origin(&self) -> f32 {
        (self.x.powi(2) + self.y.powi(2)).sqrt()
    }
}

fn main() {
    let p = Point { x: 5, y: 10 };
    println!("p.x = {}", p.x());
}
```

泛型用法如此，对于一个泛型，可以针对某个实际的类型单独进行实现，如上的 `distance_from_origin()` 段为 `Point<f32>` 仅有的方法，而 `x()` 则是 `Point<T>` 都可用的。

`impl<T>` 中的 `<T>` 声明了泛型以便在后面使用。

泛型并没有额外的性能开销，因为 Rust 在编译时就做了 **单态化** （monomorphization），为所有使用某个泛型的类型生成一份实现，最终实际则直接调用该实现。

``` rust
pub trait Summarizable {
    fn summary(&self) -> String;
}

pub struct Tweet {
    pub author: String,
    pub content: String,
}

impl Summarizable for Tweet {
    fn summary(&self) -> String {
        format!("{}, by {}", self.content, self.author)
    }
}

fn main() {
    let tweet = Tweet {
        author: String::from("horse_ebooks"),
        content: String::from("of course, as you probably already know, people")
    };
    
    println!("1 new tweet: {}", tweet.summary());
}
```

trait 类似其它语言的接口类的概念，提供若干需要实现的函数，也可以给某个~~接口~~函数提供默认的实现。

使用 `trait` 关键字声明一个 trait ，使用 `impl <trait name> for <struct name>` 来实现某个 trait 。

trait 可以提供默认的实现，但是在重载过的实现中调用默认实现是不可能的。

``` rust
// fn some_function<T: Display + Clone, U: Clone + Debug>(t: T, u: U) -> i32 {
// 上下两种记法等效
// fn some_function<T, U>(t: T, u: U) -> i32
//     where T: Display + Clone,
//           U: Clone + Debug
// {

fn largest<T: PartialOrd + Copy>(list: &[T]) -> T {
    let mut largest = list[0];

    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }

    largest
}

fn main() {
    let number_list = vec![34, 50, 25, 100, 65];
    let result = largest(&number_list);
    println!("The largest number is {}", result);

    let char_list = vec!['y', 'm', 'a', 'q'];
    let result = largest(&char_list);
    println!("The largest char is {}", result);
}
```

可以对泛型使用 trait 作为类型约束，称为 **trait bound** 。作为限制时如需要表示某个 ~~类~~ 结构体需要同时实现多个 trait 的话，使用 `+` 连接多个 trait 即可。在表示上过长可能影响阅读时，可以使用 `where` 把约束写在后面。

也可以对任何实现了特定 trait 的类型有条件的实现 trait。对任何满足特定 trait bound 的类型实现 trait 被称为 _blanket implementations_ 。如，标准库为任何实现了 `Display` trait 的类型实现了 `ToString` trait。这个 impl 块的声明看起来像 `impl<T: Display> ToString for T {` 。

``` rust
use std::fmt::Display;

fn longest_with_an_announcement<'a, T>(x: &'a str, y: &'a str, ann: T) -> &'a str
    where T: Display
{
    println!("Announcement! {}", ann);
    longest(x, y)
}

fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

fn main() {
    let s: &'static str = "I have a static lifetime.";
    let string1 = String::from("long string is long");
    let string2 = String::from("xyz"); // 试试注释此行并取消注释下面的那一行
    let result;
    {
        // let string2 = String::from("xyz"); // 说的是这行
        result = longest_with_an_announcement(string1.as_str(), string2.as_str(), "...");
    }
    println!("The longest string is {}", result);
}
```

注解 `'a` 表示一个生命周期 `a` ，表示其修饰的变量具有生命周期 `a` 。对于上例中的 `longest()` 函数，参数 `x` 和 `y` 以及其返回值的生命周期均为 `a` 。

上面的例子如果按照注释操作的话会无法通过编译，原因是 `longest_with_an_announcement()` 函数的参数和返回值是同样生命周期的，但其参数 `string2` 的生命周期和 `result` 明显不一样长（在引用 `result` 仍有效时 `string2` 已经无效了。生命周期的声明在此避免了此类可能产生潜在错误的情况（把错误提到了编译期）。

`'static` 生命周期存活于整个程序期间。所有的字符串字面值都拥有 `'static` 生命周期。

函数的参数和返回值的生命周期在编译器可以推断出来的情况下是可以省略的，规则如下：

 - 每一个是引用的参数都有它自己的不同于其它参数的生命周期参数，他们称为 **输入生命周期参数** 。例子：`fn foo<'a, 'b>(x: &'a i32, y: &'b i32)` 。
 - 如果只有一个输入生命周期参数，那么它被赋予所有输出生命周期参数。例子：`fn foo<'a>(x: &'a i32) -> &'a i32` 。
 - 如果有多个输入生命周期参数，而由于它是关联函数而包含参数 `&self` 或 `&mut self`，则 `self` 的生命周期被赋给所有输出生命周期参数。
 
``` rust
pub fn greeting(name: &str) -> String {
    format!("Hello {}!", name)
}

pub fn add_two(a: i32) -> i32 {
    a + 2
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_adds_two() {
        assert_eq!(4, add_two(2));
    }
	
	#[test]
    fn greeting_contains_name() {
        let result = greeting("Carol");
        assert!(
            result.contains("Carol"),
            "Greeting did not contain name, value was `{}`", result
        );
    }
}
```

使用 `cargo test` 来运行测试，测试会根据工程中的注解和所用参数（如果有，后述）执行各相应测试并汇报结果。

可以使用 `assert_eq!`， `assert_ne!` 和 `assert!` 宏帮助测试，当不满足时这些宏内部会使用 `panic!` 。我们实际测试也是根据我们的测试用例是否 panic 来判定是否通过的。

未单独配置的情况下，成功的测试用例的 stdout 不会被输出（即会被捕获），失败的则会。

也可以向 `assert!`、`assert_eq!` 和 `assert_ne!` 宏传递可选的失败信息参数，在测试失败时将自定义失败信息一同打印出来。失败信息参数实际是会被传给 `format!` 宏，故用法一致。

对于理应 panic 的用例，可以在 `#[test]` 注解后添加 `#[should_panic]` 注解表示该用例应该 panic 。

``` rust
pub fn add_two(a: i32) -> i32 {
    a + 2
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn add_two_and_two() {
        assert_eq!(4, add_two(2));
    }

    #[test]
    fn add_three_and_two() {
        assert_eq!(5, add_two(3));
    }

    #[test]
    fn one_hundred() {
        assert_eq!(102, add_two(100));
    }
	
    #[test]
    #[ignore]
    fn expensive_test() {
        // code that takes an hour to run
    }
}
```

``` bash
$ cargo test -- --test-threads=1 # 单线程跑所有的测试，相当于不使用任何并行机制
$ cargo test -- --nocapture # 不对成功的测试进行对 stdout 输出的捕获
$ cargo test add # 对于上例，会执行所有函数名由 add 开头的测试用例
$ cargo test add_two_and_two # 指定执行某个测试
$ cargo test -- --ignored # 执行标注了 ignore 的测试
```

cargo 运行测试的命令类似于 `cargo test <传递给 cargo test 的参数> -- <传递给测试二进制文件的参数>`，不过实际上传递给测试二进制文件的参数好像依然是 cargo test 在处理（比如原程序不接受任何命令行参数的时候使用 `cargo test -- --help` 依然能得到一些帮助输出，而执行 `<二进制文件名> --help` 却并没有什么用），不太清楚区别。

运行 `cargo test --help` 会告诉你 cargo test 的相关参数，而运行 `cargo test -- --help` 则会告诉你可以在分隔符 `--` 之后使用的相关参数。（这不还是都是给 cargo test 用的么）

被标注 ignore 的测试在执行测试时会被忽略，而使用 `--ignored` 则可以单独执行这些测试。

可以通过函数名前缀来对测试潜在的分组，并使用指定这个前缀来执行这组测试。但只能一次执行一组测试，即 cargo test 后只能跟一个名字作为待测试的用例名或组（前缀）名。

`#[cfg(test)]` 注解告诉 Rust 只在执行 cargo test 时才编译和运行测试代码，而在运行 cargo build 时不这么做。

代码内使用测试注解并位于 tests 模块下的即单元测试，因为单元测试依然位于整体代码内，故私有函数依然是可以被单元测试访问的。

``` rust
// tests/integration_test.rs
// 假定是库 adder 的集成测试
// $ cargo test --test integration_test # 运行该集成测试
extern crate adder;

#[test]
fn it_adds_two() {
    assert_eq!(4, adder::add_two(2));
}
```

集成测试只适用于库 crate ，二进制项目没办法被 `extern crate` 故无法这样测试。

集成测试对于待测试的库则是完全外部的，等同于使用待测库的其它普通程序。集成测试应位于工程目录的 test 子目录内，当存在时 `cargo test` 也会执行集成测试。

每一个 `tests` 目录中的文件都被编译为单独的 crate 并被认为是集成测试模块而出现在测试结果中，使用 `<模块名称>/mods.rs` 规则的则不会被认为是集成测试模块，这在测试需要非测试作用的公共工具模块时很有用。

可以通过指定测试函数的名称作为 `cargo test` 的参数来运行特定集成测试。也可以使用 `cargo test` 的 `--test` 后跟文件的名称来运行某个特定集成测试文件中的所有测试

``` rust
fn main() {
    let x = 4;
    let equal_to_x = |z| z == x;                // 捕获了环境内的变量 x 。
    assert!(equal_to_x(4));

    fn  add_one_v1   (x: u32) -> u32 { x + 1 }  // 是函数
    let add_one_v2 = |x: u32| -> u32 { x + 1 }; // 是闭包
    let add_one_v3 = |x|             { x + 1 }; // 闭包可以省略参数的类型声明
    let add_one_v4 = |x|               x + 1  ; // 闭包体只有一行时也可以省略大括号
    
    println!("value = {0}, {1}, {2}", add_one_v1(1), add_one_v3(1), add_one_v4(1));
}
```

在 C++ 中有 lambda 函数，类似的，在 Rust 中可以使用**闭包**（closures）。类似 lambda ，每一个闭包实例有其自己独有的匿名类型。

闭包允许省略参数的类型声明，前提是类型可以被推断出来。省略类型不代表它可以类似泛型那样使用，它依然具有唯一的类型，即通过上下文的使用而被推断的类型。如果通过上下文无法推断闭包参数和/或返回值的类型，将会编译失败。

和 C++ 的 Lambda 表达式一样，闭包可以捕获变量，但由于 Rust 的所有权机制，一个 Rust 闭包则可被分为三种不同的种类并对应到三种 trait 上，分别为 `Fn` （对于捕获的变量获取不可变的借用），`FnMut` （获取可变的借用值） 和 `FnOnce` （获取所有权）。其中 `FnOnce` 由于被获取所有权后变量将在闭包被第一次调用后结束其生命周期而不再可用，故如果一个闭包只实现了 `FnOnce` ，则此类闭包只能被调用一次（Once）。

对于上述的三种闭包类型对应到的三个 trait ，闭包根据其本身的使用情况而对应到一个或多个 trait 。例如一个实现了 `Fn` 的闭包也必然实现了 `FnMut` 和 `FnOnce` ，而实现了 `FnMut` 的闭包也必然实现了 `FnOnce` 。

可以通过结合使用泛型和 Trait bound 来使用闭包，如 `struct Cacher<T> where T: Fn(u32) -> u32` 或记作 `struct Cacher<T: Fn(u32) -> u32>` 。

闭包捕获变量不需要像 C++ Lambda 一样声明要捕获的变量以及以何种方式进行捕获，而根据实际使用情况自行推断。如果我们希望强制闭包获取其使用的环境值的所有权，可以在参数列表前使用 move 关键字（如 `let equal_to_x = move |z| z == x;`）。这个技巧在将闭包传递给新线程以便将数据移动到新线程中时最为实用。

``` rust
struct Counter {
    count: u32,
}

impl Counter {
    fn new() -> Counter {
        Counter { count: 0 }
    }
}

impl Iterator for Counter {
    type Item = u32;

    fn next(&mut self) -> Option<Self::Item> {
        self.count += 1;

        if self.count < 3 {
            Some(self.count)
        } else {
            None
        }
    }
}

fn main() {
    let mut counter = Counter::new();

    assert_eq!(counter.next(), Some(1));
    assert_eq!(counter.next(), Some(2));
    assert_eq!(counter.next(), None);
	
    let v1: Vec<i32> = vec![1, 2, 3];
    let v2: Vec<_> = v1.iter().map(|x| x + 1).collect();
    assert_eq!(v2, vec![2, 3, 4]);
}
```

迭代器（iterator）负责遍历序列中的每一项和决定序列何时结束的逻辑。Rust 中迭代器都实现了一个叫做 `Iterator` 的定义于标准库的 trait 。

通过实现 `Iterator` 即可使一个类型可被迭代。迭代器需要至少实现 `next()` 。迭代器是惰性的（调用 `next()` 才会移到下一个需访问的元素，而调用 `next()` 被称为在消费迭代器）。

`iter()` 方法生成一个不可变引用的迭代器。上例中，如果我们需要一个获取 v1 所有权并返回拥有所有权的迭代器，则可以调用 `into_iter` 而不是 `iter`。类似的，如果我们希望迭代可变引用，则可以调用 `iter_mut` 。

迭代器的 filter 方法获取一个使用迭代器的每一个项并返回布尔值的闭包。如果闭包返回 true，其值将会包含在 filter 提供的新迭代器中。如果闭包返回 false，其值不会包含在结果迭代器中。

map 方法使用闭包来调用每个元素以生成新的迭代器。

collect 收集迭代器的值并构成一个新的集合类型。

``` bash
$ cargo build
    Finished dev [unoptimized + debuginfo] target(s) in 0.0 secs
$ cargo build --release
    Finished release [optimized] target(s) in 0.0 secs
```

在 Rust 中 发布配置（release profiles）是预定义的、可定制的带有不同选项的配置，他们允许程序员更多的控制代码编译的多种选项。每一个配置都彼此相互独立。

Cargo 有两个主要的配置：运行 cargo build 时采用的 `dev` 配置和运行 `cargo build --release` 的 `release` 配置。`dev` 配置被定义为开发时的好的默认配置，release 配置则有着良好的发布构建的默认配置。

对于各个发布配置的实际配置选项，在 `Cargo.toml` 的 `[profile.*]` 段下（星号替换为实际的发布配置名），如 `[profile.dev]` 。

``` rust
enum List {
    Cons(i32, Box<List>),
    Nil,
}

use List::{Cons, Nil};

fn main() {
    let list = Cons(1,
        Box::new(Cons(2,
            Box::new(Cons(3,
                Box::new(Nil))))));

    let b = Box::new(5);
    println!("b = {}", b);
}
```

Rust 也存在指针的概念，其中最常见的就是 **引用** （_reference_， `&`） 了。而 **智能指针** 则是一类表现类似指针而包含额外元数据和功能的数据结构。普通引用和智能指针的一个额外的区别是，引用是一类只借用数据的指针，而大部分情况，智能指针 **拥有** 他们指向的数据。

智能指针常使用结构体来实现，他们实现了 `Deref` （使其表现像引用一样）和 `Drop` （离开作用域时运行的代码）trait。

`String` 和 `Vec<T>` 其实也是智能指针。

`Box<T>` 允许你将一个值放在堆上而不是栈上。

Rust 需要在编译时知道类型占用多少空间，故在枚举或结构体中尝试使用自身作为成员（来试图构成递归类型）会导致报错。但使用 `Box<T>` 后其类型所占的空间就确定了。上例的 List 即通过 `Box<T>` 构成 cons list 这种递归类型。

（注：很容易可以联想到单链表，然而使用 `Cons(i32, &List)` 会提示确少生命周期修饰符，使用 `Cons(i32, &'a List)` 则提示生命周期未定义，而 `enum List<'a>` 后则又开始提示确少生命周期修饰符。原因尚未搞懂）

``` rust
use std::ops::Deref;

struct MyBox<T>(T);

impl<T> MyBox<T> {
    fn new(x: T) -> MyBox<T> {
        MyBox(x)
    }
}

impl<T> Deref for MyBox<T> {
    type Target = T;

    fn deref(&self) -> &T {
        &self.0
    }
}

fn hello(name: &str) {
    println!("Hello, {}!", name);
}

fn main() {
    let x = 5;
    let y = MyBox::new(x);

    assert_eq!(5, x);
    assert_eq!(5, *y);
    
    let m = MyBox::new(String::from("Rust"));
    hello(&m);
    hello(&(*m)[..]); // 如果没有 解引用强制多态 特性，则上面这行需要这样写
}
```

可以使用解引用符号（`*`）像普通指针一样访问智能指针的值本身。智能指针通过实现 `Deref` trait 实现解引用。

当对智能指针使用解引用符号时，实际 Rust 在底层运行了 `*(y.deref())` 。

**解引用强制多态**（deref coercions）即当尝试对智能指针使用 `&` 时（例如 `&MyBox<String>` ）， Rust 会将其转换为其原始类型的引用（即 `&String`）处理。当然这个过程可以是链式传递的，例如标准库中提供的 `String` 的 Deref 实现会返回字符串 slice （即 `&str`，使得 `&MyBox<String>` 可以被视作 `&str` 使用）。

对于可变类型，有 `DerefMut` trait 对应。

``` rust
struct CustomSmartPointer {
    data: String,
}

impl Drop for CustomSmartPointer {
    fn drop(&mut self) {
        println!("Dropping CustomSmartPointer with data `{}`!", self.data);
    }
}

fn main() {
    let c = CustomSmartPointer { data: String::from("my stuff") };
    let d = CustomSmartPointer { data: String::from("other stuff") };
    drop(c);
    println!("CustomSmartPointers created.");
}
```

实现 `Drop` trait （提供了 `drop()`）以提供离开作用域时的析构功能。析构顺序和声明顺序相反。

允许手动对某个变量进行提前析构，但不是执行其 `drop()` ，而应当使用 `std::mem::drop` （位于 prelude 中，故直接使用 `drop(xxx)` 即可）。

（根据文章描述，析构应该是结束作用域时如果自身还有所有权的话就无论如何都会执行的，可能是有默认实现？而使用 `std::mem::drop` **大概** 会使其取得所有权而避免重复调用 `drop()` 造成重复释放内存）

``` rust
enum List {
    Cons(i32, Rc<List>),
    Nil,
}

use List::{Cons, Nil};
use std::rc::Rc;

fn main() {
    let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
    println!("count after creating a = {}", Rc::strong_count(&a));
    let b = Cons(3, Rc::clone(&a));
    println!("count after creating b = {}", Rc::strong_count(&a));
    {
        let c = Cons(4, a.clone()); // 也可以使用 a.clone() 代替 Rc::clone(&a)
        println!("count after creating c = {}", Rc::strong_count(&a));
    }
    println!("count after c goes out of scope = {}", Rc::strong_count(&a));
}
```

Rust 存在引用计数智能指针 `Rc<T>` （注：只能用于单线程场景）。其 `clone` 方法使得引用计数器加一，而不是产生一个新的拷贝。

`Rc::strong_count` 记录了引用的数量。注意，得到的引用均为不可变引用。

``` rust
// 不完整的示例代码，此段的完整相关代码请参阅：
// https://github.com/rust-lang/book/blob/master/second-edition/src/ch15-05-interior-mutability.md
mod tests {
    use super::*;
    use std::cell::RefCell;

    struct MockMessenger {
        sent_messages: RefCell<Vec<String>>, // 注意此处是 RefCell<T>
    }

    impl MockMessenger {
        fn new() -> MockMessenger {
            MockMessenger { sent_messages: RefCell::new(vec![]) }
        }
    }

    impl Messenger for MockMessenger {
        fn send(&self, message: &str) { // 此处的 self 是不可变引用，由于是实现 trait ，并没有办法修改 self 为可变。
            self.sent_messages.borrow_mut().push(String::from(message)); // 可变借用
        }
    }

    #[test]
    fn it_sends_an_over_75_percent_warning_message() {
        // --snip--

        assert_eq!(mock_messenger.sent_messages.borrow().len(), 1);
    }
}
```

`RefCell<T>` 允许在运行时执行不可变（或可变）借用检查，使用 RefCell<T> 能够在外部值被认为是不可变的情况下修改内部值。

其 `borrow` 方法返回 `Ref` 类型的智能指针，`borrow_mut` 方法返回 `RefMut` 类型的智能指针。

`RefCell<T>` 记录当前有多少个活动的 `Ref` 和 `RefMut` 智能指针。每次调用 `borrow` ，`RefCell<T>` 将活动的不可变借用计数加一。当 `Ref` 值离开作用域时，不可变借用计数减一。像编译时借用规则一样，`RefCell<T>` 在任何时候只允许有多个不可变借用或一个可变借用。

在不符合规则时会 `panic!` 。另外由于不是编译时进行的检查而是运行时，故有一定性能开销。

``` rust
#[derive(Debug)]
enum List {
    Cons(Rc<RefCell<i32>>, Rc<List>),
    Nil,
}

use List::{Cons, Nil};
use std::rc::Rc;
use std::cell::RefCell;

fn main() {
    let value = Rc::new(RefCell::new(5));

    let a = Rc::new(Cons(Rc::clone(&value), Rc::new(Nil)));

    let b = Cons(Rc::new(RefCell::new(6)), Rc::clone(&a));
    let c = Cons(Rc::new(RefCell::new(10)), Rc::clone(&a));

    let mb1 = Rc::clone(&value); // 得到了一个新的所有者
    *mb1.borrow_mut() += 10; // 并且可以修改其值
    *value.borrow_mut() += 10; // 原值也可以

    println!("a after = {:?}", a);
    println!("b after = {:?}", b);
    println!("c after = {:?}", c);
}
```

结合 `Rc<T>` 和 `RefCell<T>` 就可以得到有多个所有者并且都可以通过它获得可变借用来修改其值。

当然，显然我们可以通过 `RefCell<Rc<List>>` 构造循环引用，并可能导致内存泄漏。

``` rust
use std::rc::{Rc, Weak};
use std::cell::RefCell;

#[derive(Debug)]
struct Node {
    value: i32,
    parent: RefCell<Weak<Node>>,
    children: RefCell<Vec<Rc<Node>>>,
}

fn main() {
    let leaf = Rc::new(Node {
        value: 3,
        parent: RefCell::new(Weak::new()),
        children: RefCell::new(vec![]),
    });

    println!("leaf parent = {:?}", leaf.parent.borrow().upgrade());

    let branch = Rc::new(Node {
        value: 5,
        parent: RefCell::new(Weak::new()),
        children: RefCell::new(vec![Rc::clone(&leaf)]),
    });

    *leaf.parent.borrow_mut() = Rc::downgrade(&branch);

    println!("leaf parent = {:?}", leaf.parent.borrow().upgrade());
}
```

我们并不总是需要强引用计数指针，例如树形结构时父子结点互相指向并不包含所有权的持有。子结点可以具有一个指向父节点的指针但并不需要强引用计数功能。 `Weak<T>` 则是我们需要的东西。

`Rc::clone` 会增加 `Rc` 实例的 `strong_count`，`Rc` 实例只在其 `strong_count` 为 0 时才会被清理。而调用 `Rc::downgrade` 并传递 `Rc` 实例的引用即可创建其值的 **弱引用**（weak reference），一个 `Weak<T>` 类型的智能指针。调用 `Rc::downgrade` 会将 `weak_count` 加一，`Rc` 类型使用 `weak_count` 来记录其存在多少个 `Weak<T>` 引用，而 `Rc<T>` 内的资源释放依然仅当强引用计数为 0 时才进行，弱引用并不影响其资源释放。

由于强引用计数为 0 时 `Weak<T>` 引用的值可能已经释放了，故使用 `Weak<T>` 的值时我们调用的 `Weak<T>` 实例的 `upgrade` 方法返回的是 `Option<Rc<T>>`。如果 `Rc` 值还未被丢弃则结果是 `Some`，已经被丢弃则结果是 `None`。由于是 `Option` ，故我们不必担心意外的处理了一个无效的指针。

``` rust
use std::thread;
use std::time::Duration;

fn main() {
    let handle = thread::spawn(|| {
        for i in 1..10 {
            println!("hi number {} from the spawned thread!", i);
            thread::sleep(Duration::from_millis(1));
        }
    });

    for i in 1..5 {
        println!("hi number {} from the main thread!", i);
        thread::sleep(Duration::from_millis(1));
    }

    handle.join().unwrap(); // 尝试注释这行
}
```

正如其它较底层的编程语言，Rust 标准库只提供了 1:1 线程模型实现。

如果我们希望强制闭包获取其使用的环境值的所有权，可以在参数列表前使用 move 关键字。move 闭包经常与 `thread::spawn` 一起使用，因为它允许我们在一个线程中使用另一个线程的数据。

``` rust
use std::thread;
use std::sync::mpsc;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();
    
    let tx1 = mpsc::Sender::clone(&tx);
    thread::spawn(move || {
        thread::sleep(Duration::from_secs(1));
        let val = String::from("hi1");
        tx1.send(val).unwrap();
    });

    thread::spawn(move || {
        let vals = vec![
            String::from("hi"),
            String::from("from"),
            String::from("the"),
            String::from("thread"),
        ];

        for val in vals {
            tx.send(val).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });
    
    let received = rx.recv().unwrap();
    println!("Got1: {}", received);

    for received in rx {
        println!("Got2: {}", received);
    }
}
```

Rust 中一个实现（线程间）消息传递并发的主要工具是 **通道**（channel）。使用 `mpsc::channel` 函数创建一个新的通道；`mpsc` 是 **多个生产者，单个消费者** （multiple producer, single consumer）的缩写。即，Rust 标准库实现通道的方式意味着一个通道可以有多个产生值的 发送（sending）端，但只能有一个消费这些值的 接收（receiving）端。

`mpsc::channel` 函数返回一个元组：第一个元素是发送端，而第二个元素是接收端。由于历史原因，tx 和 rx 通常作为 发送者（transmitter）和 接收者（receiver）的缩写。

通道的发送端有一个 `send` 方法用来获取需要放入通道的值。send 方法返回一个 `Result<T, E>` 类型，所以如果接收端已经被丢弃了，将没有发送值的目标，所以发送操作会返回错误。另外， `send` 函数获取其参数的所有权并移动这个值归接收者所有。

通道的接收端有两个有用的方法：`recv` 和 `try_recv`。 `recv` 是 receive 的缩写，会阻塞主线程执行直到从通道中接收一个值。一旦发送了一个值，`recv` 会在一个 `Result<T, E>` 中返回它。当通道发送端关闭，`recv` 会返回一个错误表明不会再有新的值到来了。 `try_recv` 不会阻塞，相反它立刻返回一个 `Result<T, E>` ：`Ok` 值包含可用的信息，而 `Err` 值代表此时没有任何消息。

也可以不显式调用 `recv` 函数而是将 rx 当作一个迭代器。对于每一个接收到的值，我们将其打印出来。当通道被关闭时，迭代器也将结束。

可以通过克隆通道的发送端做到“多个生产者”。

``` rust
use std::sync::{Mutex, Arc};
use std::thread;

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for _ in 0..10 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();

            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("Result: {}", *counter.lock().unwrap());
}
```

Rust 提供互斥器 `Mutex<T>` ，不同于 c++ 中常见的互斥锁实现，Rust 提供的互斥器持有一个变量，而不是作为一个独立的 guard （准确的说，`lock` 调用返回一个叫做 `MutexGuard` 的智能指针）。使用关联函数 `new` 来创建一个 `Mutex<T>`。使用 `lock` 方法获取锁，以访问互斥器中的数据。这个调用会阻塞当前线程，直到我们拥有锁为止。

如果另一个线程拥有锁，并且那个线程 panic 了，则 lock 调用会失败。在这种情况下，没人能够再获取锁。可以使用 `unwrap` 在遇到这种情况时使线程简单的 panic。

多线程操作时使用互斥锁防止多个线程同时访问同一数据，此时我们显然需要互斥器能够在多个现成中被访问。由于 `Rc<T>` 并非线程安全的，所以在多线程情况下应该使用线程安全的**原子引用计数**（atomically reference counted）类型 `Arc<T>` ，其具有和 `Rc<T>` 相同的 API 。

``` plain
暂无范例
```

`std::marker` 中的 `Sync 和 `Send` 这两个 trait 所表示的并发概念是内嵌于 Rust 语言中（而不是标准库中）的。 // 那这个 std...

`Send` trait 表明类型的所有权可以在线程间传递。几乎所有的 Rust 类型都是 `Send` 的，但明显存在例外：如 `Rc<T>` 并非是线程安全的，自然不能在线程间传递。任何完全由 `Send` 的类型组成的类型也会自动被标记为 `Send`。几乎所有基本类型都是 `Send` 的，除了后续将会讨论的裸指针（raw pointer）。

`Sync` trait 表明类型可以安全的在多个线程中拥有其值的引用。对于任意类型 `T`，如果 `&T`（`T` 的引用）是 `Send` 的话 `T` 就是 `Sync` 的，这意味着其引用就可以安全的发送到另一个线程。类似于 `Send` 的情况，基本类型是 `Sync` 的，完全由 `Sync` 的类型组成的类型也是 `Sync` 的。`Rc<T>` 也不是 `Sync` 的，`RefCell<T>` 和 `Cell<T>` 也不是。

``` rust
# 非可编译的完整示例，仅为代码片段
match VALUE {
    PATTERN => EXPRESSION,
    PATTERN => EXPRESSION,
}

let PATTERN = EXPRESSION; // 这个模式实际上等于 “将任何值绑定到变量 PATTERN，不管值是什么”

for PATTERN in VALUE {
    EXPRESSION;
}

let robot_name = Some(String::from("Bors"));
match robot_name {
    Some(ref name) => println!("Found a name: {}", name), // 如果不写 ref ，Some 中的值的所有权会被移走。下方的 println 也就不能再使用不再具有其所有权的值了。
    None => (),
}
println!("robot_name is: {:?}", robot_name);

let num = Some(4);
match num {
    Some(x) if x < 5 => println!("less than five: {}", x), // 匹配守护
    Some(x) => println!("{}", x),
    None => (),
}

let x = 4;
let y = false;
match x {
    4 | 5 | 6 if y => println!("yes"), // 使用 `|` 的同时进行匹配守护
    _ => println!("no"),
}

enum Message {
    Hello { id: i32 },
}

let msg = Message::Hello { id: 5 };
match msg {
    Message::Hello { id: id_variable @ 3...7 } => { // 匹配 `id` 的值同时将 `id` 的值保存到 `id_variable` 中以供使用。当然此处 `id_variable` 也可以和模式同名。
        println!("Found an id in range: {}", id_variable)
    },
    Message::Hello { id } => {
        println!("Found some other id: {}", id)
    },
}
```

Rust 中诸多基本关键字语法使用了 **模式**（pattern）语法，类如 `match` 会根据值来匹配对应的模式以执行不同的分支。但实际使用模式的不只是看上去的 `match`, `if let`, ` while let`, 还包括 `for`, `let` 以及函数参数等。

类如 `let x = 5` 这样的模式是 **不可反驳的** （irrefutable），因为 `x` 可以匹配任何值所以不可能失败。而 `if let Some(x) = a_value` 则是 **可反驳的** （refutable），比如 `a_value` 是 `None` 的时候就失败了。let 语句、 函数参数和 for 循环只能接受不可反驳的模式，因为通过不匹配的值程序无法进行有意义的工作。if let 和 while let 表达式被限制为只能接受可反驳的模式，因为根据定义他们意在处理可能的失败 —— 条件表达式的功能就是根据成功或失败执行不同的操作。理解这两种模式的意义是 —— 看懂对应情况的报错。

在模式匹配中 `&` 符号会匹配引用而不是创建引用，故当需要创建引用时，我们应该使用 `ref` （对应 `&`）和 `ref mut` （对应 `&mut`）。

匹配守卫（match guard）是一个指定与 match 分支模式之后的额外 if 条件，它也必须被满足才能选择此分支。匹配守护作用于前面对应的整个模式。例如上方使用了或运算符的例子中，匹配守护作用域 `4 | 5 | 6` 这整个模式。

at 运算符 `@` 允许我们在创建一个存放值的变量的同时测试其值是否匹配模式。使用 `@` 可以在一个模式中同时测试和保存变量值。

``` rust
use std::slice;

extern "C" { // 外部（这里是来自 C 语言的）函数
    fn abs(input: i32) -> i32;
}

#[no_mangle]
pub extern "C" fn call_from_c() {
    println!("Just called a Rust function from C!");
}

fn split_at_mut(slice: &mut [i32], mid: usize) -> (&mut [i32], &mut [i32]) { // 这个函数是安全的
    let len = slice.len();
    let ptr = slice.as_mut_ptr();

    assert!(mid <= len);

    unsafe {
        (slice::from_raw_parts_mut(ptr, mid), // slice::from_raw_parts_mut 函数获取一个裸指针和一个长度来创建一个 slice
         slice::from_raw_parts_mut(ptr.offset(mid as isize), len - mid)) // slice::from_raw_parts_mut 是不安全的，所以需要 unsafe
    }
}

fn main() {
    unsafe {
        println!("Absolute value of -3 according to C: {}", abs(-3)); // 使用外部函数总是不安全的，故需要 unsafe
    }

    let mut num = 5;
    let r1 = &num as *const i32; // 可以在安全的代码中创建裸指针
    let r2 = &mut num as *mut i32; // 但不能进行解引用操作
    
    unsafe {
        println!("r1 is: {}", *r1); // 在不安全的代码中对裸指针进行解引用
        println!("r2 is: {}", *r2);
    }

	let mut v = vec![1, 2, 3, 4, 5, 6];
	let r = &mut v[..];
	let (a, b) = split_at_mut(r, 3);

	assert_eq!(a, &mut [1, 2, 3]);
	assert_eq!(b, &mut [4, 5, 6]);
}
```

Rust 允许使用 `unsafe` 关键字标记某段代码 / 函数 / trait 是不安全的， `unsafe` 允许以下四种行为，并需要程序员自身来保证 `unsafe` 代码的安全性：

 - 解引用裸指针
 - 调用不安全的函数或方法
 - 访问或修改可变静态变量
 - 实现不安全 trait

如 C 或 C++ 中的（非智能）指针，裸指针可以为空，也不能保证指向的内存一定有效，也不能实现任何自动清理功能。因为是裸指针，也就无法应用借用规则了。故尽管 Rust 允许在安全的代码中创建裸指针（创建是安全的，不会产生问题，而使用裸指针则可能产生问题），但若需要使用裸指针（对其解引用以访问其值）则需要在 `unsafe` 的代码内做。

我们可以把函数，方法，以及 trait 声明为不安全的，而当使用或实现它们时则必须也标明 unsafe （对于使用函数，应在 unsafe 块内使用，对于实现 unsafe trait，则实现也应标记为 unsafe ）。一个安全的函数中可以包含 unsafe 块，可以以此创建不安全代码的安全抽象。

使用 `extern` 关键字来创建和使用 **外部函数接口** （Foreign Function Interface， FFI），当使用外部函数接口时，这些接口总是 unsafe 的（因为 Rust 没法检查它们），而创建的接口则是安全的。不过，创建的外部接口需要注明 `#[no_mangle]` 以阻止 Rust 对函数名进行 name mangling。

``` rust
static mut COUNTER: u32 = 0;

fn add_to_count(inc: u32) {
    unsafe {
        COUNTER += inc;
    }
}

fn main() {
    add_to_count(3);
    unsafe {
        println!("COUNTER: {}", COUNTER);
    }
}
```

全局变量在 Rust 中被称为 静态（static）变量，因而具有的是 `'static` 生命周期。静态变量是一个变量，访问时是（总是同一）地址指向的值，而常量则是固定值。

对于静态变量，必须标注变量的类型。

静态变量可以是可变的，拥有可以全局访问的可变数据，难以保证不存在数据竞争，因而访问和修改可变静态变量都是不安全的。

``` plain
暂无范例
```

可以照常声明一个生命周期 `'a`，并通过语法 `'b: 'a` 声明一个不短于 `'a` 的生命周期 `'b`。

可以对生命周期声明 bound，`struct Ref<'a, T: 'a>(&'a T);` 为 `T` 增加生命周期 bound 来指定 `T` 中的任何引用需至少与 `'a` 存活的一样久。

trait 对象的生命周期自动推断规则是：

 - trait 对象的默认生命周期是 'static。
 - 如果有 &'a X 或 &'a mut X，则默认生命周期是 'a。
 - 如果只有 T: 'a 从句， 则默认生命周期是 'a。
 - 如果有多个类似 T: 'a 的从句，则没有默认生命周期；必须明确指定。

当必须明确指定时，可以为像 `Box<Red>` 这样的 trait 对象增加生命周期 bound，根据需要使用语法 `Box<Foo + 'a>` 或 `Box<Foo + 'static>`。 // 等等，不是冒号吗？

``` rust
// 代码片段
pub trait Iterator {
    type Item;
    fn next(&mut self) -> Option<Self::Item>;
}

impl Iterator for Counter {
    type Item = u32;

    fn next(&mut self) -> Option<Self::Item> {
        // --snip--

// --------------------------
pub trait Iterator<T> { // 上面的 trait 类似这种写法，但不需要每次使用都声明 T 的具体类型
    fn next(&mut self) -> Option<T>;
}
// --------------------------
// Add trait 的原型
trait Add<RHS=Self> {
    type Output;

    fn add(self, rhs: RHS) -> Self::Output;
}
// 可以编译的例子
fn main() {
use std::ops::Add;

struct Millimeters(u32);
struct Meters(u32);

impl Add<Meters> for Millimeters {
    type Output = Millimeters; // 实现时进行类型关联，调用时就不必了

    fn add(self, other: Meters) -> Self::Output {
        Millimeters(self.0 + (other.0 * 1000))
    }
}
}
```

可以使用 `type` 关键字声明 **关联类型** （associated types），这种写法可以避免调用时不必要的类型声明。

上面 `Add` trait 的例子中使用了默认类型。除了作为默认这一本身的作用之外还可以做来扩展类型而不破坏现有代码。

Rust 不允许进行直接的类型重载，但可以通过实现指定 trait 来达到重载的目的。例如上面例子中的 `Add` trait。

``` rust
trait Animal {
    fn baby_name() -> String;
}

struct Dog;

impl Dog {
    fn baby_name() -> String { // 如果直接使用 Dog::baby_name() 则会调用这个关联函数
        String::from("Spot")
    }
}

impl Animal for Dog { // 为 Dog 实现 Animal trait
    fn baby_name() -> String { // 是关联函数（静态函数）而不是方法（成员函数）
        String::from("puppy")
    }
}

fn main() {
    println!("A baby dog is called a {}", <Dog as Animal>::baby_name()); // 完全限定语法
}
```

一个类型存在某个函数，某个 trait 存在同名的函数，此时依然可以为这个类型实现这个 trait ，只需指明所要调用的函数即可。但有时 Rust 无法计算出所要调用的是哪个实现。

上例中有一个 `Animal` trait，可以为类型 `Dog` 实现 `Animal` trait 中的 `baby_name()`，但 `Dog` 本身也有一个同名关联函数，故如果使用 `Dog::baby_name()` 则会调用 `Dog` 本身的实现，而若想调用 `Dog` 所实现的 `Animal` trait 中的实现却不能直接使用 `Animal::baby_name()` —— 因为 `Animal` 是 trait 而不是具体实现，又由于这是关联函数而不是方法，故没有 `self` 参数可供推断，所以这样调用无法计算出所希望使用的实现。使用 **完全限定语法** 即可消除这歧义。注意，不能使用 `Animal::baby_name()` 存粹是因为无法计算出所希望使用的实现，而不是因为 `Dog` 所具有的同名函数的存在 —— 即便同名函数不存在，也只能通过 `Dog::baby_name()` 调用， `Dog` 具体到了某个实现，而 `Animal` 没有。

``` rust
#![allow(unused_variables)]
fn main() {
    type Kilometers = i32;
    // type Result<T> = Result<T, std::io::Error>; // 进一步的用法
    // struct Wrapper(i32); // newtype 模式，使用没有名称的元组结构体作为 Wrapper 创建新类型

    let x: i32 = 5;
    let y: Kilometers = 5;

    println!("x + y = {}", x + y);
}
```

可以使用 `type` 创建 **类型别名** （type alias），别名将会和原名一同看待而不会当作新类型。若需要看作不同的类型看待，则应该使用被称作为 “newtype pattern” 的模式。

newtype 模式允许我们绕过 **孤儿规则**（orphan rule，即不允许对外部类型实现外部 trait。例如，不能在 `Vec` 上实现 `Display` trait，因为 `Display` 和 `Vec` 都定义于标准库中），给创造的新类型实现 trait。

``` rust
// 不能编译的代码片段
fn bar() -> ! {
    // --snip--
}

// 来自最初 猜猜看 的片段
let guess: u32 = match guess.trim().parse() {
    Ok(num) => num,
    Err(_) => continue,
};

// fn generic<T>(t: T) { //实际会被处理成：
// fn generic<T: Sized>(t: T) { // 若希望放宽这种限制，则需要： 
fn generic<T: ?Sized>(t: &T) {
    // --snip--
}
```

`!` 类型被称为 never type （在类型理论中被称为 empty type）。never type 可以强转为任何其他类型。例如在 `match` 中，`continue` 就是 never type，其不返回任何类型，而是交给外层循环逻辑处理。`panic!` 宏和 `loop` 表达式也都具有 `!` 类型。

**动态大小类型** （dynamically sized types，有时被称为 “DST” 或 “unsized types”）允许我们处理只有在运行时才知道大小的类型，例如 `str` （不是 `&str`）是一个 DST。Rust 需要知道应该为特定类型的值分配多少内存，故当我们使用 `str` 时使用的是 `&str` slice，其储存了开始位置和 slice 的长度。这是 Rust 中动态大小类型的常规用法：他们有一些额外的元信息来储存动态信息的大小。这引出了动态大小类型的黄金规则：必须将动态大小类型的值置于某种指针之后。

可以将 `str` 与所有类型的指针结合：比如 `Box<str>` 或 `Rc<str>`。而 `trait` 实际也是 DST，每一个 trait 都是一个可以通过 trait 名称来引用的动态大小类型。为了将 trait 用于 trait 对象，必须将他们放入指针之后，比如 &Trait 或 Box<Trait>（Rc<Trait> 也可以）。

而为了处理 DST，Rust 有一个特定的 trait ，即 `Sized` trait 来决定一个类型的大小是否在编译时可知。这个 trait 自动为编译器在编译时就知道大小的类型实现，且 Rust 隐式的为每一个泛型函数增加了 Sized bound。