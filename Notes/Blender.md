注：版本 2.8x

 - 相机飞行模式
   Shift+` 即可
 - 移动变焦（Dolly Zoom）
   https://docs.blender.org/manual/en/dev/editors/3dview/navigate/navigation.html#dolly-zoom

   Ctrl+Shift+中键上下拖动。与滚轮的放大缩小不是一回事。另，若为解决普通滚轮的放大缩小局限性问题，也可以去设置里开启自动深度（Auto Depth）选项。另见：https://blender.stackexchange.com/questions/644/why-does-the-zoom-sometimes-stop-at-a-point
 - 移动视口以显示当前激活的物体（zoom to selected）
   小键盘点（numpad .）
 - 移动视口中心到当前点选的位置
   Alt+鼠标中键点击，会移动视口中心到点选位置上。另外，Alt+鼠标中键各方向拖动 可以快速切换到正、侧、顶等视图，和 numpad 1/3/7/9 效果一样。
 - 移动 3d 光标：
   Shift+S 打开 Snap 环形菜单会有调整光标位置的相关选项。另在 3d 光标模式下（左侧模式选中 3d 光标图标）可以直接拖动 3d 光标来移动位置。按下 Shift+C 可以重置 3d 光标的位置并同时重置视口。
 - 切换各种编辑模式：
   Tab 切换 Edit 和 Object 模式，Ctrl+Tab 打开 Mode 环形菜单以变选择所有可供切换的模式。部分情况可能没有环形菜单，比如在选中骨骼时，Ctrl+Tab 会切换 Pose 模式，Tab 会切换 Edit 模式。
 - 切换渲染/着色模式：
   按 Z 打开 Shadings 环形菜单，Shift+Z 在网格（wireframe）模式和 Solid 模式中切换
 - 禁止选中
   在 Outliner 中点击相关图标即可，对于需要同时禁用子物体的情况，按住 Shift 并点击。
 - 顶点，边缘，面的相关快捷菜单
   Ctrl+V(ertix)/E(dge)/F(ace)，或者直接右键菜单。
 - UV 展开和贴图

   编辑模式直接全选然后在 UV Editor 视图就能看到自动生成的 UV 展开图了，若需手动标记接缝，选中对应的边后在边缘菜单（Ctrl+E）里 Mark Seam，然后按 U 打开 UV Mapping 菜单，选择 Unwrap 展开。

   在 UV Editor 中直接打开图片不会自动使得物体本身具备此贴图材质，所以需要在材质选项中，在 Base Color 中选择图片纹理（Image Texture）然后选择和 UV Editor 中打开的对应的贴图图片。
 - 应用
   Ctrl+A 打开应用菜单。对物体操作进行的各种变换（位移，缩放，旋转等）需要经过此操作来使其应用到物体的自身数据，此操作应当在对骨骼生成rig【前】进行。
 - 常用操作
   - 贴靠（Snap）：按住Ctrl
   - 环形选择（Loop Select）：Alt+点击
   - 根据选中的边/点创建面：F
   - 分割（Rip，撕裂）所选的点/边，并移动分割后的边：V
   - 分割（Separate）物体，P
   - 连接（Bridge，桥接）所创建的两个环形边缘：Ctrl+E > Bridge Edge Loop
   - 连接（Connect）所选的点：J（join）
   - 合并（Merge）所选的点：Alt+M
   - 调整轴心点（Pivot point）：（大键盘 .）
 - 实用的插件
   - [内附] Rigging: Rigify 提供了一些骨骼模板
   - Mesh: Auto Mirror 提供了设置镜像操作的快捷入口（在n panel）
   - Object: Bool Tools 提供了快速布尔操作的方式（在n panel）
   - [非内附] Screencast Key 或 Shortcut VUr 提供了按键操作的屏显。前者曾为旧版 blender 内附的插件
   - SimpleLattice 简易晶格插件，选中物体后右键即可按对应物体的轮廓快速创建晶格并自动附加修改器： https://github.com/BenjaminSauder/SimpleLattice
 - 实用的设置
   - 编辑模式下，右上角可以启用自动合并顶点的功能
