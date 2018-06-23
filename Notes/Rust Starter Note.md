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
