## Maven Starter Note

### 创建项目

可以在 **交互模式** （InteractiveMode ）下创建项目，

```shell
mvn archetype:generate
```

交互模式使用默认值即可，但默认情况下 DarchetypeArtifactId 列表太多了，很不爽。所以可以考虑使用 **批处理模式**  （BatchMode） 。使用 `-B` 参数即 BatchMode 。

```shell
# 来自 Maven Getting Start Guide 的示例，若无说明，下同
mvn -B archetype:generate \
  -DarchetypeGroupId=org.apache.maven.archetypes \
  -DgroupId=com.mycompany.app \
  -DartifactId=my-app
```

应该等效于：

```shell
# 来自 https://maven.apache.org/archetype/maven-archetype-plugin/examples/generate-batch.html
mvn archetype:generate -B -DarchetypeGroupId=org.apache.maven.archetypes \
  -DarchetypeArtifactId=maven-archetype-quickstart \
  -DarchetypeVersion=1.1 \
  -DgroupId=com.company \
  -DartifactId=project \
  -Dversion=1.0-SNAPSHOT \
  -Dpackage=com.company.project
```

如果单纯的使用 `mvn -B archetype:generate` 会提示缺少 `groupId`， `artifactId` 和 `package`。

### POM 文件

创建完毕后目录下会有 `pom.xml` ，该文件包含了 **项目对象模型（Project Object Model， POM）**，大概长这个样子：

``` xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
                      http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.mycompany.app</groupId>
  <artifactId>my-app</artifactId>
  <packaging>jar</packaging>
  <version>1.0-SNAPSHOT</version>
  <name>Maven Quick Start Archetype</name>
  <url>http://maven.apache.org</url>
  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.11</version>
      <scope>test</scope>
    </dependency>
  </dependencies>
</project>
```

- `project` 是 pom.xml 的顶层元素
- `modelVersion` 包含 POM 所用的版本，不用改它
- `groupId` 标识创建该项目的组织的唯一标识符。
- `artifactId` 是项目名称，也会影响打包出来的 JAR 文件名，比如 `myapp-1.0.jar`。
- `packaging` 打包成什么，比如可以是 JAR，WAR，EAR 之类的。该项也会影响项目构建过程的的生命周期。
- `version` 如果需要快照版，加 `-SNAPSHOT` 就行了。比如 `1.0-SNAPSHOT`。
- `url` 在哪儿能找到这个项目，可以放项目官网。
- `description` 可选的吧，上面那个示例就没有。

一个有意思的是，依赖描述节点 `dependency` 下的 `scope` 可以描述该依赖的作用域，比如上面的示例中，junit 仅用于 mvn test ，发布中则不会依赖这个。 

### 构建项目

```shell
mvn compile
```

若要编译运行单元测试，只需 

```shell
mvn test # 如果只需要编译而不运行单元测试，应执行 `mvn test-compile`
```

### 打包

构建 JAR 包的方法就是

```shell
mvn package
```

POM 文件写了所以是 JAR 包。若需要把 JAR 包安装到本地仓库（默认位于 `${user.home}/.m2/repository`），则：

```shell
mvn install
```

若要清空构建，可以

``` shell
mvn clean
```

这个指令会删掉构建生成的 `target` 文件夹。

Maven 还能直接根据 POM 生成一个网站....

``` shell
mvn site
```

不过似乎没啥用...

### 使用插件

若要使用插件，编辑 POM 描述文件的 `<plugins>` 段，它位于 `<build>` 下，`<build>` 则位于根 `<project>` 下。

``` xml
...
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-compiler-plugin</artifactId>
      <version>3.3</version>
      <configuration>
        <source>1.5</source>
        <target>1.5</target>
      </configuration>
    </plugin>
  </plugins>
</build>
...
```

### 一次构建多个项目

首先需要在项目目录的上一层创建一个父 POM 文件，文件结构大概会是这样

```shell
+- pom.xml # 父 POM，需要创建的就是这个
+- my-app # 子项目 1
|  +- pom.xml
|  +- blahblah
+- my-webapp # 子项目 2
   +- pom.xml
   +- blahblah
```

文件内容大概像这样：

``` xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
                      http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
 
  <groupId>com.mycompany.app</groupId>
  <artifactId>app</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>pom</packaging>
 
  <modules>
    <module>my-app</module>
    <module>my-webapp</module>
  </modules>
</project>
```

此外还需要编辑每个子项目的 POM 文件，在根 `<project>` 节点下添加 `<parent>` 段。

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
                      http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <parent>
    <groupId>com.mycompany.app</groupId>
    <artifactId>app</artifactId>
    <version>1.0-SNAPSHOT</version>
  </parent>
  ...
```

然后就可以在父层执行 `mvn clean install` 了。

### 配置 Maven

Maven 全局配置文件在 `{M2_HOME}/conf/settings.xml`，对于用户，可以编辑 `${HOME}/.m2/setting.xml`（不存在就创建一个就是了）。

#### 使用镜像

Maven 会根据配置文件中 `<mirrors>` 节点下的 `<mirror>` 节点来匹配镜像，匹配方法是拿 repository 的 id 和 `mirrorOf` 的规则进行匹配，匹配成功后使用该 `<mirror>` 指定的镜像，不再继续尝试匹配。

```xml
<!-- 以下示例均来自 https://maven.apache.org/guides/mini/guide-mirror-settings.html -->
<settings>
  ...
  <mirrors>
    <mirror>
      <id>UK</id>
      <name>UK Central</name>
      <url>http://uk.maven.org/maven2</url>
      <mirrorOf>central</mirrorOf>
    </mirror>
  </mirrors>
  ...
</settings>
```

上面示例将应用到所有 id 为 central 的 **repository** 。 central 表示中央仓库。

`mirrorOf` 可以使用逗号分割多个 repository 名称（或规则），比如  `repo1,repo2` 。规则也可以为 `*` ，这将匹配所有 repository 的 id，即导致获取所有 repository 时都会使用该 mirror 。对于 `*,!repo1` 这个示例，将会匹配除了 repo1 外的所有 repository 使用该规则。

mirror 节点的 id 是干嘛的我还真没搞清楚

profiles.. todo: [ref](https://maven.apache.org/guides/mini/guide-configuring-maven.html) 